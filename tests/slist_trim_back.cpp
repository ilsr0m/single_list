#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(TrimBackNull, TrimBackTest) {
    auto[lst, size] = GetParam();
    EXPECT_EQ(slist_trim_back(lst, size), tuti::onFailure);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(TrimBackNullSuite, TrimBackNull, ::testing::Values (
    std::make_tuple(nullptr, 0),
    std::make_tuple(slist_create(sizeof(int)), 2)
));

TEST_P(TrimBackFull, TrimBackTest) {
    TrimFrontFullParam trim = GetParam();
    slist_t *lst = tuti::toSlist(trim.base);
    EXPECT_EQ(slist_trim_back(lst, trim.count), trim.count);
    tuti::compareWith(lst, trim.target);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(TrimBackFullSuite, TrimBackFull, ::testing::Values (
    TrimFrontFullParam({1, 2, 3, 4, 5}, 0, {1, 2, 3, 4, 5}),
    TrimFrontFullParam({1, 2, 3, 4, 5}, 1, {1, 2, 3, 4}),
    TrimFrontFullParam({1, 2, 3, 4, 5}, 2, {1, 2, 3}),
    TrimFrontFullParam({1, 2, 3, 4, 5}, 3, {1, 2}),
    TrimFrontFullParam({1, 2, 3, 4, 5}, 4, {1}),
    TrimFrontFullParam({1, 2, 3, 4, 5}, 5, {})
));