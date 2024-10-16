//
//  Storage.hpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#ifndef Storage_hpp
#define Storage_hpp

#include <string>
#include <fstream>
#include <iostream>
#include <deque>
#include <stack>
#include <optional>
#include <functional>
#include "BlockIO.hpp"
#include "misc/Errors.hpp"
#include "queue"

namespace ECE141 {

    const int32_t kNewBlock = -1;

    //Hmmm-- what could you do with this?....
    class Storable {
    public:
        virtual StatusResult encode(std::ostream &anOutput) const = 0;

        virtual StatusResult decode(std::istream &anInput) = 0;

        virtual bool initHeader(BlockHeader &header) const = 0;
    };

    template<typename T>
    struct ByteIO {
        bool readBytes(std::istream &anInput, T &out) {
            anInput.read(reinterpret_cast<char *>(&out), sizeof(T));
            return anInput.gcount() == sizeof(T);
        };

        void writeBytes(std::ostream &anOutput, T bytes) const {
            anOutput.write(reinterpret_cast<char *>(&bytes), sizeof(T));
        };
    };

    using BlockVisitor = std::function<bool(const Block &, uint32_t)>;
    using BlockList = std::deque<uint32_t>;

    // USE: Our storage manager class...
    class Storage : public BlockIO {
    public:
        Storage() = default;

        Storage(const std::string &aName, AccessMode aMode)
                : BlockIO(aName, aMode) {};

        ~Storage() {};

        bool each(const BlockVisitor &aVisitor, size_t fromIndex = 0);
        bool eachInList(const BlockVisitor &aVisitor, size_t fromIndex); // Follows nextBlock pointer

        uint32_t writeStorable(const Storable& aStorable, int64_t startBlock=-1, BlockType type=BlockType::unknown);
        StatusResult readStorable(Storable& aStorable, uint32_t startBlock);
        uint32_t writeWhereFree(std::istream &anInput, BlockHeader aHeader, int64_t startBlock=-1);

        StatusResult writeBlock(uint32_t aBlockNumber, Block &aBlock) override;
        StatusResult readBlock(uint32_t aBlockNumber, Block &aBlock) override;

        //What if we also offered a "storable" interface?
        //StatusResult save(const Storable &aStorable, int32_t aStartPos=kNewBlock);
        //StatusResult load(std::iostream &aStream, uint32_t aStartBlockNum);

        StatusResult markBlockAsFree(uint32_t aPos);
        StatusResult markChainAsFree(uint32_t aPos);

        LRUCache<uint32_t, Block>& getCache() { return blockCache; }
    protected:
        uint32_t popFreeBlock(int64_t skipBlock=-1); //pos of next free (or new)...
        std::priority_queue<uint32_t, std::vector<uint32_t>, std::greater<uint32_t>> freeBlocks;

        uint32_t highestUsedBlock = 0;
        LRUCache<uint32_t, Block> blockCache{Config::getCacheSize(CacheType::block)};
        friend class Database;
    };

}


#endif /* Storage_hpp */
