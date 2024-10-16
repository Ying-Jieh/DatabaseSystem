//
//  Storage.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//


#include <sstream>
#include <cmath>
#include <cstdlib>
#include <optional>
#include <cstring>
#include <iostream>
#include "storage/Storage.hpp"
#include "misc/Config.hpp"

namespace ECE141 {


    // USE: ctor ---------------------------------------
//
//    Storage::Storage(const std::string &aName, AccessMode aMode)
//            : BlockIO(aName, aMode) {}
//
//    // USE: dtor ---------------------------------------
//    Storage::~Storage() = default;

    bool Storage::each(const BlockVisitor &aVisitor, size_t fromIndex) {
        uint32_t blockCount = getBlockCount();
        uint32_t index = fromIndex;
        Block block;
        while (index < blockCount) {
            readBlock(index, block);
            if (!aVisitor(block, index)) {
                return false; // Terminated early by visitor
            }
            index++;
        }
        return true;
    }

    bool Storage::eachInList(const BlockVisitor &aVisitor, size_t fromIndex) {
        uint32_t index = fromIndex;
        Block block;
        uint32_t next = -1;
        while (next) { // != 0
            readBlock(index, block);
            next = block.header.nextBlock;
            if (!aVisitor(block, index)) {
                return false; // Terminated early by visitor
            }
            index = next;
        }
        return true;
    }


    uint32_t Storage::writeStorable(const ECE141::Storable &aStorable, int64_t startBlock, BlockType type) {
        INFO("Storage::writeStorable(const ECE141::Storable &aStorable, int64_t startBlock, BlockType type), where (startBlock, type) = (", startBlock, ", ", char(type), ")")
        std::stringstream ss;
        aStorable.encode(ss);
        BlockHeader header;
        aStorable.initHeader(header);
        if (type != BlockType::unknown) header.type = type;

        bool writeInChain = startBlock != -1;
        if (!writeInChain) startBlock = popFreeBlock();
        bool done = false;

        Block block;
        block.header = header;

        uint32_t current = startBlock;

        uint32_t prevBlock = 0;
        if (writeInChain) {
            eachInList([&](const Block& readBlock, uint32_t index) {
                if (done) {
                    markBlockAsFree(index); // Free the rest of the chain if our new thing is shorter
                    return true;
                }

                // Prepare block we're writing
                ss.read(block.payload, kPayloadSize);
                block.header.payloadSize = ss.gcount();
                block.header.prevBlock = prevBlock;
                if (ss.peek() == EOF) {
                    done = true;
                    block.header.nextBlock = 0; // stop the chain
                } else {
                    if (readBlock.header.nextBlock == 0) {
                        // End of chain we're currently overwriting, but still not done writing this Storable
                        // eachInList will exit after this iter, and we will move to do {} while
                        current = popFreeBlock();
                        block.header.nextBlock = current;
                    } else {
                        block.header.nextBlock = readBlock.header.nextBlock;
                    }
                }

                // Overwrite the block at index
                writeBlock(index, block);
                prevBlock = index;

                return true;
            }, startBlock);

            if (done) return startBlock;
        }

        uint32_t nextFree = current;
        do {
            ss.read(block.payload, kPayloadSize);
            block.header.payloadSize = ss.gcount();
            if (ss.peek() == EOF) {
                done = true;
                block.header.nextBlock = 0; // stop the chain
            } else {
                nextFree = popFreeBlock(current);
                block.header.nextBlock = nextFree;
            }
            block.header.prevBlock = prevBlock;
            writeBlock(current, block);
            prevBlock = current;
            current = nextFree;
        } while(!done);

        return startBlock;
    }

    StatusResult Storage::readStorable(Storable& aStorable, uint32_t startBlock) {
        INFO("Storage::readStorable(Storable& aStorable, uint32_t startBlock), where startBlock=", startBlock)
        std::stringstream ss;
        eachInList([&](const Block& aBlock, uint32_t index) {
            // Stream all blocks into SS
            ss.write(aBlock.payload, aBlock.header.payloadSize);
            return true;
        }, startBlock);
        return aStorable.decode(ss);
    }


    uint32_t Storage::writeWhereFree(std::istream &anInput, BlockHeader aHeader, int64_t startBlock) {
        Block block;
        block.header = aHeader;
        bool done = false;
        uint32_t free;
        if (startBlock == -1) {
            free = popFreeBlock();
            startBlock = free;
        } else {
            free = startBlock;
        }
        uint32_t nextFree = free;
        do {
            anInput.read(block.payload, kPayloadSize);
            if (anInput.peek() == EOF) {
                done = true;
                block.header.nextBlock = 0;
            } else {
                nextFree = popFreeBlock(free);
                block.header.nextBlock = nextFree;
            }
            free = nextFree;
            writeBlock(free, block);
        } while (!done);
        return startBlock;
    }

    uint32_t Storage::popFreeBlock(int64_t skipBlock) {
        if (!freeBlocks.empty()) {
            uint32_t top = freeBlocks.top();
            freeBlocks.pop();
            return top;
        }
        uint32_t nextFree = highestUsedBlock + 1;
        if (skipBlock != -1) {
            if (nextFree == skipBlock) return ++nextFree;
        }
        return nextFree;
    }

    StatusResult Storage::writeBlock(uint32_t aBlockNumber, Block &aBlock) {
        INFO("Storage::writeBlock(uint32_t aBlockNumber, Block &aBlock), where aBlockNumber=", aBlockNumber)
        BlockIO::writeBlock(aBlockNumber, aBlock);
        blockCache.put(aBlockNumber, aBlock);
        highestUsedBlock = std::max(aBlockNumber, highestUsedBlock);
        DEBUG("blockCache.put(aBlockNumber, aBlock), where aBlockNumber=", aBlockNumber)
        DEBUG("highestUsedBlock = ", std::max(aBlockNumber, highestUsedBlock))
        return {Errors::noError};
    }

    StatusResult Storage::readBlock(uint32_t aBlockNumber, Block &aBlock) {
        INFO("Storage::readBlock(uint32_t aBlockNumber, Block &aBlock), where aBlockNum=", aBlockNumber)
        if (blockCache.contains(aBlockNumber)) {
            DEBUG("blockCache.contains(aBlockNumber)")
            aBlock = blockCache.get(aBlockNumber);
            DEBUG("aBlock = blockCache.get(aBlockNumber)")
            return {Errors::noError};
        }
        DEBUG("!blockCache.contains(aBlockNumber)")
        StatusResult theStatue = BlockIO::readBlock(aBlockNumber, aBlock);
        blockCache.put(aBlockNumber, aBlock);
        return theStatue;
    }

    StatusResult Storage::markBlockAsFree(uint32_t aPos) {
        DEBUG("Storage::markBlockAsFree(uint32_t aPos), where aPos=", aPos)
        Block block;
        block.header.type = BlockType::free;
        block.header.nextBlock = 0;
        block.header.prevBlock = 0;
        returnIfError(writeBlock(aPos, block));
        freeBlocks.push(aPos);
        DEBUG("freeBlocks.push(aPos), where aPos=", aPos)
        return {Errors::noError};
    }

    StatusResult Storage::markChainAsFree(uint32_t aPos) {
        if (eachInList([this](const Block& aBlock, uint32_t index) {
                                    markBlockAsFree(index);
                                    return true;
                                }, aPos)) {
            return {Errors::noError};
        }
        ERROR("readError")
        return {Errors::readError};
    }
}

