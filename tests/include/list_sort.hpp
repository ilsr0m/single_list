#ifndef LIST_SORT_HPP
#define LIST_SORT_HPP

#include "list_base.hpp"

struct TestSortValues {
    bool isListNull;
    comparator_fn comparator;
    std::vector<int> unsortedValues;
    std::vector<int> sortedValues;
    int result;

    TestSortValues(bool isListNull, comparator_fn comparator, 
        std::vector<int> unsortedValues, std::vector<int> sortedValues, int result) : 
        isListNull{isListNull}, comparator{comparator}, 
        unsortedValues{unsortedValues}, sortedValues{sortedValues}, result{result} {}
};

class ListSortTest : public ListBaseTest<TestSortValues> {
public:
    static int SortComparator(const void* a, const void* b) {
        int _a = *(int*)a;
        int _b = *(int*)b;
        if(_a > _b) return 1;
        if(_a == _b) return 0;
        return -1;
    }

    static int ReverseSortComparator(const void* a, const void* b) {
        int _a = *(int*)a;
        int _b = *(int*)b;
        if(_a > _b) return -1;
        if(_a == _b) return 0;
        return 1;
    }
};  

TEST_P(ListSortTest, ListSort) {
    TestSortValues testValues = GetParam();
    
    if(testValues.isListNull){
        EXPECT_EQ(slist_sort(nullptr, testValues.comparator), testValues.result);
        return;
    }
    
    FillList(testValues.sortedValues);
    EXPECT_EQ(slist_sort(lst, testValues.comparator), testValues.result);

    if(testValues.result == 0) {
        EXPECT_EQ(slist_size(lst), testValues.unsortedValues.size());
        EXPECT_EQ(slist_size(lst), testValues.sortedValues.size());
        if(!slist_size(lst)) IsEmpty();
        else CompareWith(testValues.sortedValues);
    }
    ClearList();
};

INSTANTIATE_TEST_SUITE_P(TestSuiteListSort, ListSortTest, ::testing::Values(
    TestSortValues(false, nullptr                     , {2}, {2}, -1), // invalid
    TestSortValues(true , nullptr                     , {2}, {2}, -1), // invalid
    TestSortValues(true , ListSortTest::SortComparator, {2}, {2}, -1), // invalid

    TestSortValues(false, ListSortTest::SortComparator, {}                    , {}                    , 0),
    TestSortValues(false, ListSortTest::SortComparator, {111}                 , {111}                 , 0),
    TestSortValues(false, ListSortTest::SortComparator, {2, 1}                , {1, 2}                , 0),
    TestSortValues(false, ListSortTest::SortComparator, {9, 0, 2, 1, 6, 5}    , {0, 1, 2, 5, 6, 9}    , 0),
    TestSortValues(false, ListSortTest::SortComparator, {9, 0, 2, 1, 6, 5, 44}, {0, 1, 2, 5, 6, 9, 44}, 0),

    TestSortValues(false, ListSortTest::ReverseSortComparator, {}                    , {}                    , 0),
    TestSortValues(false, ListSortTest::ReverseSortComparator, {111}                 , {111}                 , 0),
    TestSortValues(false, ListSortTest::ReverseSortComparator, {1, 2}                , {2, 1}                , 0),
    TestSortValues(false, ListSortTest::ReverseSortComparator, {9, 0, 2, 1, 6, 5}    , {9, 6, 5, 2, 1, 0}    , 0),
    TestSortValues(false, ListSortTest::ReverseSortComparator, {9, 0, 2, 1, 6, 5, 44}, {44, 9, 6, 5, 2, 1, 0}, 0)
));

#endif
