#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(AtNull, AtTest) {
    auto[lst, ind] = GetParam();
    EXPECT_EQ(slist_at(lst, ind), nullptr);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(AtNullSuite, AtNull, ::testing::Values(
    std::make_tuple(nullptr, 10),
    std::make_tuple(slist_create(sizeof(int)), 10)
));

TEST_P(AtFull, AtTest) {
    auto [base, index, value] = GetParam();
    slist_t *lst = tuti::toSlist(base);
    int result = *(int*)slist_at(lst, index);
    tuti::compareWith(lst, base);
    EXPECT_EQ(result, value);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(AtFullSuite, AtFull, ::testing::Values(
    std::make_tuple(std::vector<int>{1, 2, 3}, 0, 1),
    std::make_tuple(std::vector<int>{1, 2, 3}, 1, 2),
    std::make_tuple(std::vector<int>{1, 2, 3}, 2, 3)
));
