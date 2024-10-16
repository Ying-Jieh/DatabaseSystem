//
// Created by Daniel Ziper on 4/14/24.
//

#include "gtest/gtest.h"
#include "../include/misc/FolderReader.hpp"

TEST(FolderTest, BasicTest) {
    std::string folder = "testFiles/folderReaderTests";
    ECE141::FolderReader f(folder);
    std::set<std::string> files {"testFiles/folderReaderTests/file1.txt", "testFiles/folderReaderTests/file2.txt"};
    size_t count = 0;
    f.each(".txt", [&](const std::string &name) {
        EXPECT_TRUE(files.find(name) != files.end()) << name;
        count++;
        return true;
    });
    EXPECT_EQ(count, files.size());
}
