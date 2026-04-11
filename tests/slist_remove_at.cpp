#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(RemoveAtNull, RemoveAtTest) {
    auto [lst, pos] = GetParam();
    EXPECT_EQ(slist_remove_at(lst, pos), tuti::onFailure);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(RemoveAtSuite, RemoveAtNull, ::testing::Values(
    std::make_tuple(nullptr                  , 10), // !lst, pos >
    std::make_tuple(nullptr                  , 0 ), // !list
    std::make_tuple(slist_create(sizeof(int)), 10),  // pos >
    std::make_tuple(tuti::toSlist(std::vector<int>{1, 2 ,3}), 10)  // pos >
));

TEST_P(RemoveAtFull, RemoveAtTest) {
    RemoveAtFullParam rat = GetParam();
    slist_t *lst = tuti::toSlist(rat.base);
    for(auto p : rat.positions) 
        EXPECT_EQ(slist_remove_at(lst, p), rat.result);
    tuti::compareWith(lst, rat.target);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(RemoveAtSuite, RemoveAtFull, ::testing::Values(
    RemoveAtFullParam({}, {1}      , {}, tuti::onFailure),
    RemoveAtFullParam({}, {1, 2, 3}, {}, tuti::onFailure),
    RemoveAtFullParam({1, 2, 3, 4, 5, 6}, {0}, {2, 3, 4, 5, 6}, tuti::onSuccess),
    RemoveAtFullParam({1, 2, 3, 4, 5, 6}, {0, 0}, {3, 4, 5, 6}, tuti::onSuccess),
    RemoveAtFullParam({1, 2, 3, 4, 5, 6}, {5}, {1, 2, 3, 4, 5}, tuti::onSuccess),
    RemoveAtFullParam({1, 2, 3, 4, 5, 6}, {5, 4}, {1, 2, 3, 4}, tuti::onSuccess),
    RemoveAtFullParam({1, 2, 3, 4, 5, 6}, {2}, {1, 2, 4, 5, 6}, tuti::onSuccess),
    RemoveAtFullParam({1, 2, 3, 4, 5, 6}, {2, 2}, {1, 2, 5, 6}, tuti::onSuccess)
));
