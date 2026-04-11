#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(InsertNull, InsertTest) {
    auto [lst, itemOpt, pos] = GetParam();
    const void *item = itemOpt ? static_cast<const void*>(&*itemOpt) : nullptr;
    EXPECT_EQ(slist_insert(lst, item, pos), tuti::onFailure);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(InsertNullSuite, InsertNull, ::testing::Values(
    std::make_tuple(static_cast<slist_t*>(nullptr), std::nullopt, 15), // !lst, !item, pos >
    std::make_tuple(static_cast<slist_t*>(nullptr), std::nullopt, 0 ), // !lst, !item
    std::make_tuple(tuti::toSlist(std::vector<int>{1, 2, 3, 4, 5}), std::nullopt, 15), // !item, pos >
    std::make_tuple(static_cast<slist_t*>(nullptr), std::optional<int>(10), 10), // !lst, pos >
    std::make_tuple(static_cast<slist_t*>(nullptr), std::optional<int>(10), 0 ), // !lst
    std::make_tuple(tuti::toSlist(std::vector<int>{1, 2, 3, 4, 5}), std::nullopt, 0 ), // !item
    std::make_tuple(tuti::toSlist(std::vector<int>{1, 2, 3, 4, 5}), std::optional<int>(10), 15)  // pos >
));

TEST_P(InsertFull, InsertTest) {
    InsertFullParam ins = GetParam();
    slist_t* lst = tuti::toSlist(ins.base);
    int result = slist_insert(lst, &ins.item, ins.position);
    if(ins.position > ins.base.size()) EXPECT_EQ(result, tuti::onFailure);
    else EXPECT_EQ(result, tuti::onSuccess);
    tuti::compareWith(lst, ins.target);
    tuti::tearDown(lst);
};

INSTANTIATE_TEST_SUITE_P(InsertFullSuite, InsertFull, ::testing::Values(
    InsertFullParam({}          , {}             , 2, 1, tuti::onFailure), // Position > Size 
    InsertFullParam({1, 2, 3, 4}, {1, 2, 3, 4}   , 2, 5, tuti::onFailure), // Position > Size 
    InsertFullParam({}          , {0}            , 0, 0, tuti::onSuccess), // Empty
    InsertFullParam({1, 2, 3, 4}, {2, 1, 2, 3, 4}, 2, 0, tuti::onSuccess), // Insert as the first item
    InsertFullParam({1, 2, 3, 4}, {1, 2, 3, 4, 2}, 2, 4, tuti::onSuccess), // Insert as the last item
    InsertFullParam({1, 2, 3, 4}, {1, 5, 2, 3, 4}, 5, 1, tuti::onSuccess), // Insert as the middle item
    InsertFullParam({1, 2, 3, 4}, {1, 2, 5, 3, 4}, 5, 2, tuti::onSuccess), // Insert as the middle item
    InsertFullParam({1, 2, 3, 4}, {1, 2, 3, 2, 4}, 2, 3, tuti::onSuccess)  // Insert as the middle item
));
