#ifndef LIST_REMOVE_HPP
#define LIST_REMOVE_HPP

#include "list_base.hpp"

struct TestRemoveValues {
    bool isListNull;
    comparator_fn cmp;

    std::vector<int> keys;
    std::vector<int> base;
    std::vector<int> target;

    int result;

    TestRemoveValues(bool isListNull, comparator_fn cmp, 
        std::vector<int> keys, std::vector<int> base, std::vector<int> target, int result) : 
        isListNull{isListNull}, cmp{cmp}, 
        keys{keys}, base{base}, target{target}, 
        result{result} {}
};

class ListRemoveTest : public ListBaseTest<TestRemoveValues> {
public:
    static int Comparator(const void* _item, const void* _key) {
        return (*(int*)_item == *(int*)_key) ? 0 : -1;
    }
};

TEST_P(ListRemoveTest, ListRemove){
    TestRemoveValues rm = GetParam();
    if(rm.isListNull && rm.keys.empty()) {
        EXPECT_EQ(slist_remove(nullptr, nullptr, rm.cmp), rm.result);
        return;
    }
    if(rm.isListNull) {
        EXPECT_EQ(slist_remove(nullptr, &rm.keys[0], rm.cmp), rm.result);
        return;
    }
    if(rm.keys.empty()) {
        EXPECT_EQ(slist_remove(lst, nullptr, rm.cmp), rm.result);
        return;
    }

    FillList(rm.base);

    for(auto k : rm.keys) {
        EXPECT_EQ(slist_remove(lst, &k, rm.cmp), rm.result);
    }
    CompareWith(rm.target);
    ClearList();
}

INSTANTIATE_TEST_SUITE_P(TestSuiteListRemove, ListRemoveTest, ::testing::Values(
    TestRemoveValues(true , nullptr, {} , {} , {}, -1),
    TestRemoveValues(true , nullptr, {2}, {2}, {}, -1),
    TestRemoveValues(false, nullptr, {} , {} , {}, -1),
    TestRemoveValues(true , ListRemoveTest::Comparator, {} , {} ,  {}, -1),
    TestRemoveValues(true , ListRemoveTest::Comparator, {2}, {2},  {}, -1),
    TestRemoveValues(false, ListRemoveTest::Comparator, {} , {} ,  {}, -1),

    TestRemoveValues(false, nullptr, {1}    , {1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}, -1),
    TestRemoveValues(false, nullptr, {1, 2} , {1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}, -1)

));

#endif