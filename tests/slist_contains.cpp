#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(ContainsNull, ContainsTest) {
    auto [lst, keyOpt, cmp] = GetParam();
    const void *key = keyOpt ? static_cast<const void*>(&*keyOpt) : nullptr;
    EXPECT_EQ(slist_contains(lst, key, cmp), tuti::onFailure);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(ContainsNullSuite, ContainsNull, ::testing::Values(
    // !lst, !item, !cmp
    std::make_tuple(static_cast<slist_t*>(nullptr), std::nullopt, static_cast<comparator_fn>(nullptr)),
    // !item, !cmp
    std::make_tuple(slist_create(sizeof(int)), std::nullopt, static_cast<comparator_fn>(nullptr)),
    // !lst, !cmp
    std::make_tuple(static_cast<slist_t*>(nullptr), std::optional<int>(2), static_cast<comparator_fn>(nullptr)),   
    // !lst, !item
    std::make_tuple(static_cast<slist_t*>(nullptr), std::nullopt, &tuti::ascending),
    // !lst
    std::make_tuple(static_cast<slist_t*>(nullptr), std::optional<int>(2), &tuti::ascending),
    // !item
    std::make_tuple(slist_create(sizeof(int)), std::nullopt, &tuti::ascending),    
    // !cmp
    std::make_tuple(slist_create(sizeof(int)), std::optional<int>(2), static_cast<comparator_fn>(nullptr))
));

TEST_P(ContainsFull, ContainsTest) {
    auto [base, val, expected] = GetParam();
    slist_t *lst = tuti::toSlist(base);
    EXPECT_EQ(slist_contains(lst, &val, &tuti::ascending), expected);
    tuti::compareWith(lst, base);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(ContainsFullSuite, ContainsFull, ::testing::Values(
    std::make_tuple(std::vector<int>{}, 1, 0),
    std::make_tuple(std::vector<int>{1, 2, 3, 4, 5}, 6, 0),
    std::make_tuple(std::vector<int>{1, 2, 3, 4, 5}, 1, 1),
    std::make_tuple(std::vector<int>{1, 2, 3, 4, 5}, 2, 1),
    std::make_tuple(std::vector<int>{1, 2, 3, 4, 5}, 3, 1),
    std::make_tuple(std::vector<int>{1, 2, 3, 4, 5}, 4, 1),
    std::make_tuple(std::vector<int>{1, 2, 3, 4, 5}, 5, 1)
));