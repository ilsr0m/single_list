#ifndef LIST_APPEND_HPP
#define LIST_APPEND_HPP

#include "list_base.hpp"

struct TestAppendValues {
    bool isListNull;
    bool isItemNull;
    std::vector<int> values;
    int result;

    TestAppendValues(bool isListNull, bool isItemNull, std::vector<int> values, int result) : 
        isListNull{isListNull}, isItemNull{isItemNull}, values{values}, result{result} {}
};

class ListAppendTest : public ListBaseTest<TestAppendValues> {};

TEST_P(ListAppendTest, ListAppend) {
    TestAppendValues testValues = GetParam();

    // invalid case
    if(testValues.isListNull || testValues.isItemNull) {
        EXPECT_EQ(list_append(nullptr, nullptr), testValues.result);
        return;
    }
    // invalid case
    if(testValues.isItemNull) {
        EXPECT_EQ(list_append(lst, nullptr), testValues.result);
        return;
    }
    // invalid case
    if(testValues.isListNull) {
        for(auto v : testValues.values) 
            EXPECT_EQ(list_append(nullptr, &v), testValues.result);
        return;    
    }

    if(testValues.values.size()){
        // Append all vector's values one by one
        for(auto v : testValues.values)
            EXPECT_EQ(list_append(lst, &v), testValues.result);
        CompareWith(testValues.values);
        ListClear();
        return;
    }
    IsEmpty();
}

INSTANTIATE_TEST_SUITE_P(TestSuiteListAppend, ListAppendTest, ::testing::Values(
    TestAppendValues(true , true , {}, -1), // invalid
    TestAppendValues(true , false, {}, -1), // invalid
    TestAppendValues(false, true , {}, -1), // invalid
    TestAppendValues(false, false, {}                    , 0),
    TestAppendValues(false, false, {111}                 , 0),
    TestAppendValues(false, false, {1, 2}                , 0),
    TestAppendValues(false, false, {2, 5, 4}             , 0),
    TestAppendValues(false, false, {-11, 1, 2, 5, 6, 9}  , 0),
    TestAppendValues(false, false, {0, 1, 2, 5, 6, 9, 44}, 0),
    TestAppendValues(false, false, {9, 6, 5, 2, 133, 12} , 0),
    TestAppendValues(false, false, {44, 9, 6, 5, 2, 1, 0}, 0)
));

#endif