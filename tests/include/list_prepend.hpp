#ifndef LIST_PREPEND_HPP
#define LIST_PREPEND_HPP

#include "list_append.hpp"

class ListPrependTest : public ListAppendTest {
public:
    void CompareWith(std::vector<int> vec) override {
        if(slist_empty(lst)) { 
            IsEmpty(); 
            return; 
        }
        IsNotEmpty();
        EXPECT_EQ(slist_size(lst), vec.size());
        // Tail must be equal to the first vector's item
        EXPECT_EQ(*(int*)snode_data(slist_tail(lst)), vec[0]);
        // Head must be equal to the last vector's item
        EXPECT_EQ(*(int*)snode_data(slist_head(lst)), vec[vec.size() - 1]);
        // Compare each list's item and vector's item
        snode_t *current = slist_head(lst);
        for(unsigned i = vec.size() - 1; i != 0; i--, current = snode_next(current)) 
            EXPECT_EQ(*(int*)snode_data(current), vec[i]);
    };    
};

TEST_P(ListPrependTest, ListPrepend) {
    TestAppendValues testValues = GetParam();

    // invalid case
    if(testValues.isListNull && testValues.isItemNull) {
        EXPECT_EQ(slist_prepend(nullptr, nullptr), testValues.result);
        return;
    }
    // invalid case
    if(testValues.isItemNull) {
        EXPECT_EQ(slist_prepend(lst, nullptr), testValues.result);
        return;
    }
    // invalid case
    if(testValues.isListNull) {
        for(auto v : testValues.values) 
            EXPECT_EQ(slist_prepend(nullptr, &v), testValues.result);
        return;    
    }

    if(testValues.values.size()){
        // Append all vector's values one by one
        for(auto v : testValues.values)
            EXPECT_EQ(slist_prepend(lst, &v), testValues.result);
        CompareWith(testValues.values);
        ClearList();
        return;
    }
    IsEmpty();
}

INSTANTIATE_TEST_SUITE_P(TestSuiteListPrepend, ListPrependTest, ::testing::Values(
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