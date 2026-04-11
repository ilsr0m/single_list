#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(PopBackNull, PopBackTest) {
    slist_t* slst = GetParam();
    EXPECT_EQ(slist_pop_back(slst), nullptr);
    if(slst) slist_delete(&slst);
}

// null and empty
INSTANTIATE_TEST_SUITE_P(PopBackNullSuite, PopBackNull, ::testing::Values (
    nullptr, slist_create(sizeof(int))
));

TEST_P(PopBackFull, PopBackTest) {
    PopFrontFullParam pop = GetParam();
    slist_t *lst = tuti::toSlist(pop.base);
    for(unsigned i = 0; i < pop.count; i++)
        EXPECT_EQ(*(int*)slist_pop_back(lst), pop.popped[i]);
    tuti::compareWith(lst, pop.target);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(PopBackFullSuite, PopBackFull, ::testing::Values(
    PopFrontFullParam({1, 2, 3}, 1, {1, 2}, {3}),
    PopFrontFullParam({1, 2, 3}, 2, {1}, {3, 2}),
    PopFrontFullParam({1, 2, 3}, 3, {}, {3, 2, 1})
));