#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(TrimFrontNull, TrimFrontTest) {
    auto[lst, size] = GetParam();
    EXPECT_EQ(slist_trim_front(lst, size), tuti::onFailure);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(TrimFrontNullSuite, TrimFrontNull, ::testing::Values (
    std::make_tuple(nullptr, 0),
    std::make_tuple(slist_create(sizeof(int)), 2)
));

TEST_P(TrimFrontFull, TrimFrontTest) {
    TrimFrontFullParam trim = GetParam();
    slist_t *lst = tuti::toSlist(trim.base);
    EXPECT_EQ(slist_trim_front(lst, trim.count), trim.count);
    tuti::compareWith(lst, trim.target);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(TrimFrontFullSuite, TrimFrontFull, ::testing::Values (
    TrimFrontFullParam({1, 2, 3, 4, 5}, 0, {1, 2, 3, 4, 5}),
    TrimFrontFullParam({1, 2, 3, 4, 5}, 1, {2, 3, 4, 5}),
    TrimFrontFullParam({1, 2, 3, 4, 5}, 2, {3, 4, 5}),
    TrimFrontFullParam({1, 2, 3, 4, 5}, 3, {4, 5}),
    TrimFrontFullParam({1, 2, 3, 4, 5}, 4, {5}),
    TrimFrontFullParam({1, 2, 3, 4, 5}, 5, {})
));