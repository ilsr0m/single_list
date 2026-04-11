#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(SetNull, SetTest) {
    auto [lst, index, itemOpt] = GetParam();
    const void *item = itemOpt ? static_cast<const void*>(&*itemOpt) : nullptr;
    EXPECT_EQ(slist_set(lst, index, item), tuti::onFailure);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(SetNullSuite, SetNull, ::testing::Values(
    std::make_tuple(nullptr                  , 10, std::nullopt), 
    std::make_tuple(nullptr                  , 10, std::optional<int>(1)), 
    std::make_tuple(slist_create(sizeof(int)), 10, std::nullopt),
    std::make_tuple(slist_create(sizeof(int)), 0, std::optional<int>(1))   
));

TEST_P(SetFull, SetTest) {
    SetFullParam set = GetParam();
    slist_t *lst = tuti::toSlist(set.base);
    for(auto d : set.datas) {
        int result = slist_set(lst, d.index, &d.item);
        EXPECT_EQ(result, set.result);
    }
    tuti::compareWith(lst, set.target);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(SetFullSuite, SetFull, ::testing::Values(
    SetFullParam({1, 2, 3}, {{4, 0}}, {1, 2, 3}, tuti::onFailure), // index out of range
    SetFullParam({1, 2, 3}, {{0, 0}}, {0, 2, 3}, tuti::onSuccess), 
    SetFullParam({1, 2, 3}, {{1, 0}}, {1, 0, 3}, tuti::onSuccess),
    SetFullParam({1, 2, 3}, {{2, 0}}, {1, 2, 0}, tuti::onSuccess),
    SetFullParam({1, 2, 3}, {{0, 0}, {1, 0}}, {0, 0, 3}, tuti::onSuccess),
    SetFullParam({1, 2, 3}, {{2, 0}, {0, 6}, {1, 5}}, {6, 5, 0}, tuti::onSuccess),
    SetFullParam({1, 2, 3, 4}, {{1, 0}, {2, 7}}, {1, 0, 7, 4}, tuti::onSuccess)
));
