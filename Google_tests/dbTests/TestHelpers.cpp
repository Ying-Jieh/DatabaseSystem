//
// Created by Daniel Ziper on 4/22/24.
//

#include "TestHelpers.hpp"

using namespace ECE141;

void helperTestStorable(Storable* in, Storable* out) {
    std::fstream file;
    file.open("testFiles/helperTest", std::fstream::out | std::ios::binary | std::fstream::trunc);
    in->encode(file);
    file.close();
    std::fstream file2;
    file2.open("testFiles/helperTest", std::fstream::in | std::fstream::binary);
    out->decode(file2);
    file2.close();
}

