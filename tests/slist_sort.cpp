#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(SortNull, SortTest) {
    auto [lst, cmp] = GetParam();
    EXPECT_EQ(slist_sort(lst, cmp), tuti::onFailure);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(SortNullSuite, SortNull, ::testing::Values(
    std::make_tuple(nullptr, nullptr),
    std::make_tuple(nullptr, &test_utils::ascending),
    std::make_tuple(slist_create(sizeof(int)), nullptr)
));

TEST_P(SortFull, SortTest) {
    SortFullParam testValues = GetParam();
    slist_t *lst = tuti::toSlist(testValues.sorted);
    EXPECT_EQ(slist_sort(lst, testValues.comparator), tuti::onSuccess);
    EXPECT_EQ(slist_size(lst), testValues.disordered.size());
    EXPECT_EQ(slist_size(lst), testValues.sorted.size());

    if(!slist_size(lst)) tuti::testEmpty(lst);
    else tuti::compareWith(lst, testValues.sorted);
    tuti::tearDown(lst);
};

INSTANTIATE_TEST_SUITE_P(SortFullSuite, SortFull, ::testing::Values(
    SortFullParam(&test_utils::ascending , {}                    , {}                    ),
    SortFullParam(&test_utils::ascending , {111}                 , {111}                 ),
    SortFullParam(&test_utils::ascending , {2, 1}                , {1, 2}                ),
    SortFullParam(&test_utils::ascending , {9, 0, 2, 1, 6, 5}    , {0, 1, 2, 5, 6, 9}    ),
    SortFullParam(&test_utils::ascending , {9, 0, 2, 1, 6, 5, 44}, {0, 1, 2, 5, 6, 9, 44}),
    SortFullParam(&test_utils::descending, {}                    , {}                    ),
    SortFullParam(&test_utils::descending, {111}                 , {111}                 ),
    SortFullParam(&test_utils::descending, {1, 2}                , {2, 1}                ),
    SortFullParam(&test_utils::descending, {9, 0, 2, 1, 6, 5}    , {9, 6, 5, 2, 1, 0}    ),
    SortFullParam(&test_utils::descending, {9, 0, 2, 1, 6, 5, 44}, {44, 9, 6, 5, 2, 1, 0})
));
