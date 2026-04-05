#ifndef LIST_INSERT_HPP
#define LIST_INSERT_HPP

#include "list_base.hpp"

struct TestInsertValues {
    bool isListNull;
    bool isItemNull;
    std::vector<int> baseValues;
    std::vector<int> targetValues;
    int item;
    int position;
    int result;
    
    TestInsertValues(bool isListNull, bool isItemNull, std::vector<int> baseValues, std::vector<int> targetValues,
        int item, int position, int result) : isListNull{isListNull}, isItemNull{isItemNull}, 
        baseValues{baseValues}, targetValues{targetValues},
         item{item}, position{position}, result{result} {}
};

class ListInsertTest : public ListBaseTest<TestInsertValues> {};

TEST_P(ListInsertTest, ListInsert) {
    TestInsertValues testValues = GetParam();
    if(testValues.isListNull && testValues.isItemNull) {
        EXPECT_EQ(list_insert(nullptr, nullptr, testValues.position), testValues.result);
        return;
    }
    if(testValues.isListNull) {
        EXPECT_EQ(list_insert(nullptr, &testValues.item, testValues.position), testValues.result);
        return;
    }
    if(testValues.isItemNull) {
        EXPECT_EQ(list_insert(lst, nullptr, testValues.position), testValues.result);
        return;
    }

    FillList(testValues.baseValues);
    int result = list_insert(lst, &testValues.item, testValues.position);
    if(testValues.position > testValues.baseValues.size()) {
        EXPECT_EQ(result, -1);
        CompareWith(testValues.baseValues);
    }
    else {
        EXPECT_EQ(result, 0);
        CompareWith(testValues.targetValues);
    }
    ListClear();
};

INSTANTIATE_TEST_SUITE_P(TestSuiteListInsert, ListInsertTest, ::testing::Values(
    TestInsertValues(true , true , {}, {}, 0, 0, -1), // Invalid Arguments
    TestInsertValues(true , false, {}, {}, 0, 0, -1), // Invalid Arguments
    TestInsertValues(false, true , {}, {}, 0, 0, -1), // Invalid Arguments
    TestInsertValues(false, false , {}          , {}             , 2, 1, -1), // Position > Size 
    TestInsertValues(false, false , {1, 2, 3, 4}, {}             , 2, 5, -1), // Position > Size 
    TestInsertValues(false, false , {}          , {0}            , 0, 0,  0), // Empty
    TestInsertValues(false, false , {1, 2, 3, 4}, {2, 1, 2, 3, 4}, 2, 0,  0), // Insert as the first item
    TestInsertValues(false, false , {1, 2, 3, 4}, {1, 2, 3, 4, 2}, 2, 4,  0), // Insert as the last item
    TestInsertValues(false, false , {1, 2, 3, 4}, {1, 5, 2, 3, 4}, 5, 1,  0), // Insert as the middle item
    TestInsertValues(false, false , {1, 2, 3, 4}, {1, 2, 5, 3, 4}, 5, 2,  0), // Insert as the middle item
    TestInsertValues(false, false , {1, 2, 3, 4}, {1, 2, 3, 2, 4}, 2, 3,  0)  // Insert as the middle item
));

#endif