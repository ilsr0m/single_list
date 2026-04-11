#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(PrependNull, PrependTest) {
    auto [lst, itemOpt] = GetParam();
    const void *item = itemOpt ? static_cast<const void*>(&*itemOpt) : nullptr;
    EXPECT_EQ(slist_prepend(lst, item),  tuti::onFailure);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(PrependNullSuite, PrependNull, ::testing::Values (
    std::make_tuple(static_cast<SingleList*>(nullptr), std::nullopt),
    std::make_tuple(slist_create(sizeof(int)), std::nullopt),
    std::make_tuple(static_cast<SingleList*>(nullptr), std::optional<int>(2))
));

TEST_P(PrependFull, PrependTest) {
    auto values = GetParam();  
    slist_t *lst = tuti::toSlist(std::vector<int>{});
    if(values.size()){
        // Append all vector's values one by one
        for(auto v : values)
            EXPECT_EQ(slist_prepend(lst, &v), tuti::onSuccess);
        tuti::compareWith(lst, values, true);
    }
    else tuti::testEmpty(lst);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(PrependFullSuite, PrependFull, ::testing::Values(
    std::vector<int>({}                  ),
    std::vector<int>({111}               ),
    std::vector<int>({1, 2}              ),
    std::vector<int>({2, 5, 4}           ),
    std::vector<int>({-11, 1, 2, 5, 6, 9})
));
