#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(RemoveAllNull, RemoveAllTest) {
    auto [lst, keyOpt, cmp] = GetParam();
    const void *key = keyOpt ? static_cast<const void*>(&*keyOpt) : nullptr;
    EXPECT_EQ(slist_remove_all(lst, key, cmp), tuti::onFailure);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(RemoveAllNullSuite, RemoveAllNull, ::testing::Values(
    std::make_tuple(static_cast<slist_t*>(nullptr), std::nullopt, static_cast<comparator_fn>(nullptr)), 
    std::make_tuple(slist_create(sizeof(int)), std::nullopt, static_cast<comparator_fn>(nullptr)), 
    std::make_tuple(static_cast<slist_t*>(nullptr), std::optional<int>(2), static_cast<comparator_fn>(nullptr)), 
    std::make_tuple(static_cast<slist_t*>(nullptr), std::nullopt, &test_utils::ascending), 
    std::make_tuple(slist_create(sizeof(int)), std::optional<int>(2), static_cast<comparator_fn>(nullptr)), 
    std::make_tuple(static_cast<slist_t*>(nullptr), std::optional<int>(2), &test_utils::ascending),
    std::make_tuple(slist_create(sizeof(int)), std::nullopt, &test_utils::ascending), 
    std::make_tuple(slist_create(sizeof(int)), std::optional<int>(2), &test_utils::ascending) 
));

TEST_P(RemoveAllFull, RemoveAllTest) {
    RemoveFullParam rm = GetParam();
    slist_t* lst = tuti::toSlist(rm.base);
    EXPECT_EQ(slist_remove_all(lst, &rm.keys[0], rm.cmp), rm.result);
    tuti::compareWith(lst, rm.target);
    tuti::tearDown(lst);
}

// base, keys, target 
INSTANTIATE_TEST_SUITE_P(RemoveAllFullSuite, RemoveAllFull, ::testing::Values(
    RemoveFullParam({1, 2, 3, 4, 5, 1, 1, 7}, {6} , {1, 2, 3, 4, 5, 1, 1, 7}, &test_utils::ascending, tuti::onFailure), // invalid key
    RemoveFullParam({1, 2, 3, 4, 5, 1, 1, 7}, {1} , {2, 3, 4, 5, 7}         , &test_utils::ascending, 3), // single remove in head
    RemoveFullParam({1, 2, 3, 4, 5, 1, 1, 7}, {7} , {1, 2, 3, 4, 5, 1, 1}   , &test_utils::ascending, 1), // single remove in tail
    RemoveFullParam({1, 2, 3, 4, 5, 1, 1, 7}, {4} , {1, 2, 3, 5, 1, 1, 7}   , &test_utils::ascending, 1), // single remove in middle
    RemoveFullParam({1, 2, 3, 4, 5, 1, 1, 1}, {1} , {2, 3, 4, 5}            , &test_utils::ascending, 4), // single remove in head
    RemoveFullParam({1, 1, 3, 4, 5, 1, 1, 7}, {1} , {3, 4, 5, 7}            , &test_utils::ascending, 4), // single remove in tail
    RemoveFullParam({0, 2, 3, 4, 5, 1, 1, 7}, {1} , {0, 2, 3, 4, 5, 7}      , &test_utils::ascending, 2), // multiple remove
    RemoveFullParam({0, 0, 3, 0, 5, 0, 1, 0}, {0} , {3, 5, 1}      , &test_utils::ascending, 5) // single remove in middle
));
