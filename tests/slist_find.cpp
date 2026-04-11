#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(FindNull, FindTest) {
    auto [lst, keyOpt, cmp] = GetParam();
    const void *key = keyOpt ? static_cast<const void*>(&*keyOpt) : nullptr;
    EXPECT_EQ(slist_find(lst, key, cmp), nullptr);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(FindNullSuite, FindNull, ::testing::Values(
    // !lst, !item, !cmp
    std::make_tuple(static_cast<slist_t*>(nullptr), std::nullopt, static_cast<comparator_fn>(nullptr)),
    // !item, !cmp
    std::make_tuple(slist_create(sizeof(int)), std::nullopt, static_cast<comparator_fn>(nullptr)),
    // !lst, !cmp
    std::make_tuple(static_cast<slist_t*>(nullptr), std::optional<int>(6), static_cast<comparator_fn>(nullptr)),   
    // !lst, !item
    std::make_tuple(static_cast<slist_t*>(nullptr), std::nullopt, &tuti::ascending),
    // !lst
    std::make_tuple(static_cast<slist_t*>(nullptr), std::optional<int>(6), &tuti::ascending),
    // !item
    std::make_tuple(slist_create(sizeof(int)), std::nullopt, &tuti::ascending), 
    // !cmp
    std::make_tuple(slist_create(sizeof(int)), std::optional<int>(6), static_cast<comparator_fn>(nullptr)),
    // empty    
    std::make_tuple(slist_create(sizeof(int)), std::optional<int>(6), &tuti::ascending),
    // not found
    std::make_tuple(tuti::toSlist(std::vector<int> {0, 1, 2, 3, 4, 5}), std::optional<int>(6), &tuti::ascending)
));


TEST_P(FindFull, FindTest) {
    auto [base, key] = GetParam();
    slist_t *lst = tuti::toSlist(base);
    EXPECT_EQ(*(int*)slist_find(lst, &key, &tuti::ascending), key);
    tuti::compareWith(lst, base);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(FindFullSuite, FindFull, ::testing::Values(
    std::make_tuple(std::vector<int>{1, 2, 3, 4, 5}, 1),
    std::make_tuple(std::vector<int>{1, 2, 3, 4, 5}, 2),
    std::make_tuple(std::vector<int>{1, 2, 3, 4, 5}, 3),
    std::make_tuple(std::vector<int>{1, 2, 3, 4, 5}, 4),
    std::make_tuple(std::vector<int>{1, 2, 3, 4, 5}, 5)
));
