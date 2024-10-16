//
// Created by YING JIEH XIA on 2024/4/15.
//

#include "gtest/gtest.h"
#include <sstream>
#include <ostream>
#include "../include/view/QueryView.hpp"

using namespace ECE141;

TEST(QueryViewTests, show) {
    int theNumRow{1};
    std::string theDBName{"testDBName"};
    StatusResult theResult;
    theResult.value = 1;
    double theDuration = 0.000004;
    QueryView theView(theResult, theDuration);

    std::stringstream expected;
    expected << "Query OK, 1 rows affected (0.000004 sec) - \n";

    std::stringstream actual;
    theView.show(actual);
    EXPECT_EQ(actual.str(), expected.str());
}

