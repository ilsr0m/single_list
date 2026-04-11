#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(TrimRangeNull, TrimRangeTest) {
    auto[lst, start, end] = GetParam();
    EXPECT_EQ(slist_trim_range(lst, start, end), tuti::onFailure);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(TrimeRangeNullSuite, TrimRangeNull, ::testing::Values(
    std::make_tuple(nullptr, 0, 1),
    std::make_tuple(tuti::toSlist(std::vector<int>{1, 2, 3}), 5, 6),
    std::make_tuple(tuti::toSlist(std::vector<int>{1, 2, 3}), 0, 4),
    std::make_tuple(tuti::toSlist(std::vector<int>{1, 2, 3}), 3, 1)
));

TEST_P(TrimRangeFull, TrimRangeTest) {
    auto tr = GetParam();
    slist_t *lst = tuti::toSlist(tr.base);
    EXPECT_EQ(slist_trim_range(lst, tr.start, tr.end), 
        tr.base.size() - tr.target.size());
    tuti::compareWith(lst, tr.target);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(TrimRangeFullSuite, TrimRangeFull, ::testing::Values(
    TrimRangeFullParam({1, 2, 3, 4, 5}, 0, 0, {1, 2, 3, 4, 5}),
    TrimRangeFullParam({1, 2, 3, 4, 5}, 0, 1, {2, 3, 4, 5}),
    TrimRangeFullParam({1, 2, 3, 4, 5}, 0, 2, {3, 4, 5}),
    TrimRangeFullParam({1, 2, 3, 4, 5}, 0, 3, {4, 5}),
    TrimRangeFullParam({1, 2, 3, 4, 5}, 0, 4, {5}),
    TrimRangeFullParam({1, 2, 3, 4, 5}, 0, 5, {}),
    TrimRangeFullParam({1, 2, 3, 4, 5}, 2, 4, {1, 2, 5}),
    TrimRangeFullParam({1, 2, 3, 4, 5}, 2, 5, {1, 2})
));