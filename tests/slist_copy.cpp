#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(CopyNull, CopyTest) {
    slist_t *slst = GetParam();
    EXPECT_EQ(slist_copy(slst), nullptr);
    if(slst) slist_delete(&slst);
}

INSTANTIATE_TEST_SUITE_P(TrimeRangeNullSuite, CopyNull, ::testing::Values(
    nullptr, slist_create(0)
));

TEST_P(CopyFull, CopyTest) {
    auto base = GetParam();
    slist_t *lst = tuti::toSlist(base);
    slist_t *copy = slist_copy(lst);
    EXPECT_NE(copy, nullptr); // !nullptr
    tuti::compareWith(copy, base);
    tuti::tearDown({copy, lst});
}

INSTANTIATE_TEST_SUITE_P(CopyFullSuite, CopyFull, ::testing::Values(
    std::vector<int> {1, 2, 3, 4, 5}, 
    std::vector<int> {2, 3, 4, 5}, 
    std::vector<int> {}, 
    std::vector<int> {12, 2, 444, 65}, 
    std::vector<int> (10, 10)
));