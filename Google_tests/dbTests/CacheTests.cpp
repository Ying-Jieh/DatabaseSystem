//
// Created by YING JIEH XIA on 2024/5/24.
//

#include "gtest/gtest.h"
#include "misc/LRUCache.hpp"
#include "misc/Config.hpp"
#include "storage/Storage.hpp"

using namespace ECE141;
size_t Config::cacheSize[]={0,0,0};

TEST(LRUCacheTest, basic) {
    Config::setCacheSize(CacheType::block, 3);
    LRUCache<size_t, std::string> theCache{Config::getCacheSize(CacheType::block)};
    std::string str1{"cache1"};
    std::string str2{"cache2"};
    std::string str3{"cache3"};
    theCache.put(1, str1);
    theCache.put(2, str2);
    theCache.put(3, str3); // 3 - 2 - 1
    EXPECT_EQ(theCache.size(), 3);

    theCache.put(1, str1); // 1 - 3 - 2
    EXPECT_EQ(theCache.size(), 3);

    std::string cache2 = theCache.get(2); // 2 - 1 - 3
    EXPECT_EQ(cache2, "cache2");

    std::string str4{"cache4"};
    theCache.put(4, str4); // 4 - 2 - 1
    EXPECT_EQ(theCache.size(), 3);
    EXPECT_ANY_THROW(theCache.get(3));
}


TEST(LRUCacheTest, useCache) {
    Config::setCacheSize(CacheType::block, 3);
    std::string theStorageName{"useCacheTest"};
    Storage storage{theStorageName, CreateFile()};
    LRUCache<uint32_t, Block>& theCache = storage.getCache();
    Block b0;
    b0.header.type = ECE141::BlockType::metadata;
    b0.header.payloadSize = 1;
    b0.payload[0] = 'a';
    Block b1;
    b1.header.type = BlockType::schema;
    b1.header.payloadSize = 1;
    b1.payload[0] = 'b';
    Block b2;
    b2.header.type = BlockType::data;
    b2.header.payloadSize = 1;
    b2.payload[0] = 'c';
    Block b3;
    b3.header.type = BlockType::data;
    b3.header.payloadSize = 1;
    b3.payload[0] = 'd';

    storage.writeBlock(0, b0);
    storage.writeBlock(1, b1);
    storage.writeBlock(2, b2); // 2 - 1 - 0
    EXPECT_EQ(theCache.peek().second, b2);
    storage.writeBlock(3, b3); // 3 - 2 - 1
    EXPECT_EQ(theCache.peek().second, b3);
    EXPECT_EQ(theCache.size(), 3);

    Block cacheB0;
    storage.readBlock(0, cacheB0);
    EXPECT_EQ(b0, cacheB0);

    Block cacheB1;
    storage.readBlock(1, cacheB1);
    EXPECT_EQ(b1, cacheB1);
}