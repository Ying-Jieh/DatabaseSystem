//
//  BlockIO.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#include <cstring>
#include "storage/BlockIO.hpp"
#include "misc/Config.hpp"

namespace ECE141 {

    Block::Block(const Block &aCopy) {
        header = aCopy.header;
        *this = aCopy;
    }

    Block &Block::operator=(const Block &aCopy) {
        std::memcpy(payload, aCopy.payload, kPayloadSize);
        header = aCopy.header;
        return *this;
    }

    StatusResult Block::write(std::ostream &aStream) {
        INFO("Block::write(std::ostream &aStream")
        header.write(aStream);
        aStream.write(payload, kPayloadSize);
        return StatusResult{Errors::noError};
    }

    //---------------------------------------------------

    // USE: write data a given block (after seek) ---------------------------------------
    StatusResult BlockIO::writeBlock(uint32_t aBlockNum, Block &aBlock) {
        INFO("BlockIO::writeBlock(uint32_t aBlockNum, Block &aBlock), where aBlockNum=", aBlockNum)
        stream.seekp(aBlockNum * kBlockSize);
        dontReadTilWrite = false;
        return aBlock.write(stream);
    }

    // USE: read data at given block (after seek) ---------------------------------------
    StatusResult BlockIO::readBlock(uint32_t aBlockNum, Block &aBlock) {
        INFO("BlockIO::readBlock(uint32_t aBlockNum, Block &aBlock), where aBlockNum=", aBlockNum)
        if (dontReadTilWrite) return {Errors::readError};
        stream.seekg(aBlockNum * kBlockSize);
        if (stream.peek() == EOF) {
            ERROR("readError")
            return {Errors::readError};
        }
        aBlock.header.read(stream);
        stream.read(aBlock.payload, kPayloadSize);
        return {Errors::noError};
    }

    // USE: count blocks in file ---------------------------------------
    uint32_t BlockIO::getBlockCount() {
        stream.seekg(0, std::ios::end);
        return stream.tellg() / kBlockSize; //What should this be?
    }

}
