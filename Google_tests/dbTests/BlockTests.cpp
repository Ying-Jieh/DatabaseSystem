//
// Created by Daniel Ziper on 4/14/24.
//

#include <cstring>
#include "gtest/gtest.h"

#define protected public
#include "../include/storage/BlockIO.hpp"
#include "../include/storage/Storage.hpp"
#include "../include/misc/Config.hpp"
#undef protected


using namespace ECE141;


TEST(BasicBlockTests, Constructor) {
    BlockHeader b;
    EXPECT_EQ(b.type, BlockType::free);
    EXPECT_EQ(b.nextBlock, 0);
    EXPECT_EQ(b.prevBlock, 0);
    EXPECT_EQ(b.hash, 0);

    BlockHeader b1{ECE141::BlockType::data, 456, 0, 123};
    EXPECT_EQ(b1.type, BlockType::data);
    EXPECT_EQ(b1.hash, 123);
    EXPECT_EQ(b1.nextBlock, 456);
    EXPECT_EQ(b1.prevBlock, 0);
    BlockHeader b2 = b1;
    EXPECT_EQ(b2.type, BlockType::data);
    EXPECT_EQ(b2.hash, 123);
    EXPECT_EQ(b2.nextBlock, 456);

    EXPECT_EQ(sizeof(b1), 24);
    EXPECT_EQ(sizeof(BlockHeader), 24);
}

TEST(BasicBlockTests, Write) {
    std::stringstream aStream;

    BlockHeader header{BlockType::data, 456, 0, 123};
    header.write(aStream);

    BlockHeader newHeader;
    aStream.seekg(0);
    newHeader.read(aStream);

    EXPECT_EQ(newHeader.type, header.type);
    EXPECT_EQ(newHeader.nextBlock, header.nextBlock);
    EXPECT_EQ(newHeader.hash, header.hash);
}

TEST(BasicBlockIOTests, Create) {
    {
        BlockIO io{"iotest", CreateFile()};
        Block b;
        b.header.hash = 1234;
        io.writeBlock(0, b);
    }
    EXPECT_TRUE(std::filesystem::exists(Config::getDBPath("iotest")));
}

class StorageTests : public testing::Test {
public:
    StorageTests() : storage("storageTest", CreateFile()) {};
protected:
    void SetUp() override {
        for (int i = 0; i < blockNum; i++) {
            Block b = randomBlock();
            blocksRef.push_back(b);
            storage.writeBlock(i, b);
        }
    }

    Block randomBlock(int seed=-1) {
        if (seed > -1) {srand(seed);}
        Block b;
        b.header.type = ECE141::BlockType::data;
        b.header.hash = rand() % 10;
        b.header.nextBlock = 0;
        b.header.prevBlock = 0;
        for (int i = 0; i < kPayloadSize; ++i) {
            b.payload[i] = rand() % 26 + 'A';
        }
        return b;
    }

    std::vector<Block> blocksRef;
    size_t blockNum = 2;
    Storage storage;
};

TEST_F(StorageTests, ReadWrite) {
    BlockIO bio{"blockTests", CreateFile()};
    bio.writeBlock(0, blocksRef[0]);
    Block out;
    bio.readBlock(0, out);
    EXPECT_EQ(out.header, blocksRef[0].header);
    EXPECT_EQ(memcmp(out.payload, blocksRef[0].payload, kPayloadSize), 0);
}

TEST_F(StorageTests, Each) {
    size_t count = 0;
    storage.each([&](const Block& aBlock, uint32_t index) {
        EXPECT_EQ(aBlock.header, blocksRef[index].header);
        EXPECT_EQ(memcmp(aBlock.payload, blocksRef[index].payload, kPayloadSize), 0);
        count++;
        return true;
    });
    EXPECT_EQ(count, blockNum);
}

struct StorableTestStruct : public Storable {
    std::string bigData;
    StorableTestStruct(const std::string& aData) : bigData(aData) {
    }
    StatusResult encode(std::ostream &anOutput) const override {
        anOutput << bigData;
        return {Errors::noError};
    };

    StatusResult decode(std::istream &anInput) override {
        anInput >> bigData;
        return {Errors::noError};
    };

    bool initHeader(BlockHeader &header) const override {
        header.type = ECE141::BlockType::data;
        return true;
    };
};

TEST_F(StorageTests, blockhelp) {
//    std::stringstream ss
}

TEST_F(StorageTests, Storable) {
    StorableTestStruct sts("abc123aslkdajslkjfalaskdj");
    uint32_t start = storage.writeStorable(sts);
    StorableTestStruct sts2("");
    storage.readStorable(sts2, start);
    EXPECT_EQ(sts.bigData, sts2.bigData);
}

TEST_F(StorageTests, BigStorable) {
    StorableTestStruct sts(std::string(3728, 'a'));
    uint32_t start = storage.writeStorable(sts);
    StorableTestStruct sts2("");
    storage.readStorable(sts2, start);
    EXPECT_EQ(sts.bigData, sts2.bigData);

    StorableTestStruct sts3(std::string(2010, 'b'));
    storage.writeStorable(sts3, start, ECE141::BlockType::data); // overwrites previous block
    sts2.bigData.clear();
    storage.readStorable(sts2, start);
    EXPECT_EQ(sts3.bigData, sts2.bigData);
}


TEST_F(StorageTests, FreeBlocks) {
    StorableTestStruct sts(std::string(3728, 'a'));
    uint32_t start = storage.writeStorable(sts);
    uint32_t curr = start;
    storage.eachInList([&](const Block& aBlock, uint32_t index) {
        EXPECT_EQ(curr, index);
        curr++;
        return true;
    }, start);
    EXPECT_EQ(curr, start + 4);

    StorableTestStruct sts3(std::string(1800, 'b'));
    uint32_t newStart = storage.writeStorable(sts3, start, ECE141::BlockType::data); // overwrites previous block
    EXPECT_EQ(newStart, start);
    curr = newStart;
    storage.eachInList([&](const Block& aBlock, uint32_t index) {
        EXPECT_EQ(curr, index);
        curr++;
        return true;
    }, newStart);
    EXPECT_EQ(curr, newStart + 2);

    EXPECT_EQ(storage.popFreeBlock(), 4);
    EXPECT_EQ(storage.popFreeBlock(), 5);
    EXPECT_EQ(storage.popFreeBlock(), 6);
}

TEST_F(StorageTests, Free2) {
    StorableTestStruct sts(std::string(3728, 'a'));
    storage.writeStorable(sts);
    storage.markChainAsFree(1);
    EXPECT_EQ(storage.popFreeBlock(), 1);
    EXPECT_EQ(storage.popFreeBlock(), 6);
}

TEST_F(StorageTests, WriteAfterFree) {
    StorableTestStruct sts(std::string(3728, 'a'));
    storage.writeStorable(sts);
    storage.markChainAsFree(1);

    StorableTestStruct sts3(std::string(1800, 'b'));
    uint32_t newStart = storage.writeStorable(sts3); // overwrites previous block
    EXPECT_EQ(newStart, 1);
    int count = 0;
    storage.eachInList([&](const Block& aBlock, uint32_t index) {
        if (count == 0) EXPECT_EQ(index, newStart);
        else EXPECT_EQ(index, 6);

        count++;
        return true;
    }, newStart);
    EXPECT_EQ(count, 2);
}

TEST_F(StorageTests, WriteInChainThenFree) {
    StorableTestStruct sts2(std::string(1800, 'b'));
    uint32_t start = storage.writeStorable(sts2);
    // Writes two blocks starting at 2
    storage.markBlockAsFree(1);
    StorableTestStruct sts3(std::string(3700, 'a'));
    uint32_t newStart = storage.writeStorable(sts3, start);
    EXPECT_EQ(newStart, start);
    // Writes 4 blocks. 2, 3, 1, 4
    std::vector<uint32_t> exp = {2,3,1,4};
    size_t count = 0;
    storage.eachInList([&](const Block& b, uint32_t idx) {
        EXPECT_EQ(idx, exp[count]);
        count++;
        return true;
    }, start);
    EXPECT_EQ(count, 4);
}

TEST(BlockTests, Create) {
    StorableTestStruct sts(std::string("abcd"));
    {
        Storage s{"myDB", CreateFile()};
        s.writeStorable(sts, 0, ECE141::BlockType::metadata);
        s.close();
    }
    BlockIO io{"myDB", OpenFile()};
    Block b;
    io.readBlock(0, b);
    BlockHeader h(ECE141::BlockType::metadata, 0, 0);
    EXPECT_EQ(b.header, h);

    Storage s{"myDB", OpenFile()};
    StorableTestStruct sts2("");
    s.readStorable(sts2, 0);
    EXPECT_EQ(sts2.bigData, sts.bigData);
}

TEST(BlockTests, Simple) {
    Block b;
    b.header.type = ECE141::BlockType::metadata;
    b.header.payloadSize = 3;
    b.payload[0] = 'a';
    b.payload[1] = 'b';
    b.payload[2] = 'c';

    // TODO: For some reason these simple tests arent' working...
    StorableTestStruct sts(std::string("abc"));

    {
        Storage storage{"plsDB", CreateFile()};
//        storage.writeBlock(0, b);
        storage.writeStorable(sts, 0, ECE141::BlockType::metadata);
    }

    {
        BlockIO bio ("plsDB", OpenFile());
        Block bl;
        bio.readBlock(0, bl);
        EXPECT_EQ(bl.header, b.header);
    }
}

TEST(BlockTests, Simple2) {
    Block b;
    b.header.type = ECE141::BlockType::metadata;
    b.header.payloadSize = 3;
    b.payload[0] = 'a';
    b.payload[1] = 'b';
    b.payload[2] = 'c';

    // TODO: For some reason these simple tests arent' working...
    StorableTestStruct sts(std::string("a139012312890123890bc"));

    Storage stor{"testagain", CreateFile()};
//    stor.writeStorable(sts, ECE141::BlockType::metadata, 0);
    Block b2;
    stor.readBlock(1, b2);
    stor.writeBlock(1, b);
}

TEST(FileTests, file) {
    std::fstream fs;
    std::string p = "tmp/testpath";

    auto thing = std::ios::binary | std::ios::in | std::ios::out |
           std::ios::trunc;
    fs.clear();
    fs.open(p, thing);
    fs.close();
    fs.open(p, thing);

    uint16_t a = 5;
    fs.seekg(2 * kBlockSize);

    fs.write(reinterpret_cast<char*>(a), sizeof(a));
    fs.close();
}