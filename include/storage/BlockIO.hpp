//
//  BlockIO.hpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#ifndef BlockIO_hpp
#define BlockIO_hpp

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <variant>
#include <functional>
#include "misc/Errors.hpp"
#include "misc/Config.hpp"
#include "misc/BasicTypes.hpp"
#include "misc/LRUCache.hpp"

namespace ECE141 {

    const size_t kBlockSize = 1024;

    using BlockTypeType = uint32_t;
    enum class BlockType : BlockTypeType {
        metadata = 'M',
        schema = 'S',
        data = 'D',
        free = 'F',
        index = 'I',
        unknown = 'U'
    };

    //a small header that describes the block...
    struct BlockHeader {
        BlockHeader(BlockType aType = BlockType::free, uint32_t aNextBlock=0, uint32_t aPrevBlock=0, IdentifierHash aHash=0) :
            type(aType), nextBlock(aNextBlock), prevBlock(aPrevBlock), hash(aHash) {};

        BlockHeader(const BlockHeader &aCopy) = default;

        BlockHeader &operator=(const BlockHeader &aCopy) = default;

        bool operator==(const BlockHeader &aBlock) const {
            return type == aBlock.type && nextBlock == aBlock.nextBlock && hash == aBlock.hash && prevBlock == aBlock.prevBlock;
        }

        void empty() {
            type = BlockType::free;
            hash = 0;
            nextBlock = 0;
            prevBlock = 0;
            payloadSize = kBlockSize - sizeof(BlockHeader);
        }

        StatusResult read(std::istream &aStream) {
            BlockTypeType blockTypeInt;
            aStream.read(reinterpret_cast<char*>(&blockTypeInt), sizeof blockTypeInt);
            type = intToType(blockTypeInt);
            aStream.read(reinterpret_cast<char*>(&payloadSize), sizeof payloadSize);
            aStream.read(reinterpret_cast<char*>(&nextBlock), sizeof nextBlock);
            aStream.read(reinterpret_cast<char*>(&prevBlock), sizeof prevBlock);
            aStream.read(reinterpret_cast<char*>(&hash), sizeof hash);
            if (type == BlockType::unknown) {
                return {Errors::invalidBlockType};
            }
            return {};
        }

        StatusResult write(std::ostream &aStream) {
            aStream.write(reinterpret_cast<char*>(&type), sizeof type);
            aStream.write(reinterpret_cast<char*>(&payloadSize), sizeof payloadSize);
            aStream.write(reinterpret_cast<char*>(&nextBlock), sizeof nextBlock);
            aStream.write(reinterpret_cast<char*>(&prevBlock), sizeof prevBlock);
            aStream.write(reinterpret_cast<char*>(&hash), sizeof hash);
            return StatusResult{Errors::noError};
        }

        static BlockType intToType(BlockTypeType aInt) {
            switch (aInt) {
                case 'M': return BlockType::metadata;
                case 'S': return BlockType::schema;
                case 'D': return BlockType::data;
                case 'F': return BlockType::free;
                case 'I': return BlockType::index;
                default: return BlockType::unknown;
            }
        }

        // Note: Order matters!
        uint32_t nextBlock;
        uint32_t prevBlock;
        IdentifierHash hash;
        BlockType type;     //char version of block type
        uint32_t payloadSize = kBlockSize - sizeof(BlockHeader);
    };

    const size_t kPayloadSize = kBlockSize - sizeof(BlockHeader);

    //block .................
    class Block {
    public:
        Block() = default;

        Block(const Block &aCopy);

        Block &operator=(const Block &aCopy);

        bool operator==(const Block &aBlock) const {
            return header == aBlock.header && (strncmp(payload, aBlock.payload, header.payloadSize) == 0);
        }

        StatusResult write(std::ostream &aStream);

        BlockHeader header;
        char payload[kPayloadSize] = {0};
    };

    //------------------------------

    struct CreateFile {
        operator std::ios_base::openmode() {
            return std::ios::binary | std::ios::in | std::ios::out |
                   std::ios::trunc;
        }
    }; //tags for db-open modes...
    struct OpenFile {
        operator std::ios_base::openmode() {
            return std::ios::binary | std::ios::in | std::ios::out;
        }
    };
    struct modeToInt {
        std::ios::openmode operator()(CreateFile &aVal) { return aVal; }

        std::ios::openmode operator()(OpenFile &aVal) { return aVal; }
    };


    using AccessMode = std::variant<CreateFile, OpenFile>;

    class BlockIO {
    public:
        BlockIO() = default;
        BlockIO(const std::string &aName, AccessMode aMode) {
            name = aName;
            std::string thePath = Config::getDBPath(aName);

            if (std::holds_alternative<CreateFile>(aMode)) {
                dontReadTilWrite = true;
            }

            auto theMode = std::visit(modeToInt(), aMode);
            stream.clear(); // Clear flag just-in-case...
            stream.open(thePath.c_str(), theMode); //force truncate if...
            stream.close();
            stream.open(thePath.c_str(), theMode);
        }

        ~BlockIO() { if (stream.is_open()) stream.close(); }

        uint32_t getBlockCount();

        virtual StatusResult readBlock(uint32_t aBlockNumber, Block &aBlock);

        virtual StatusResult writeBlock(uint32_t aBlockNumber, Block &aBlock);

        StatusResult close() {
            if (stream.is_open()) {
                stream.close();
                return {Errors::noError};
            } else {
                return {Errors::writeError};
            }

        }

    protected:
        std::fstream stream;
        std::string name;
        bool dontReadTilWrite = false;
    };

}


#endif /* BlockIO_hpp */
