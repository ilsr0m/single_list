#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(AppendNull, AppendTest) {
    auto [lst, itemOpt] = GetParam();
    const void *item = itemOpt ? static_cast<const void*>(&*itemOpt) : nullptr;
    EXPECT_EQ(slist_append(lst, item), tuti::onFailure);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(AppendNullSuite, AppendNull, ::testing::Values (
    std::make_tuple(static_cast<SingleList*>(nullptr), std::nullopt),
    std::make_tuple(slist_create(sizeof(int)), std::nullopt),
    std::make_tuple(static_cast<SingleList*>(nullptr), std::optional<int>(2))
));

/*
    Test full cases
*/
TEST_P(AppendFull, AppendTest) {
    auto values = GetParam();
    slist_t *lst = tuti::toSlist(std::vector<int>{});
    if(values.size()) {
        // Append all vector's values one by one
        for(auto v : values)
            EXPECT_EQ(slist_append(lst, &v), tuti::onSuccess);
        tuti::compareWith(lst, values);
    }
    else tuti::testEmpty(lst);
    tuti::tearDown(lst);
}

INSTANTIATE_TEST_SUITE_P(AppendFullSuite, AppendFull, ::testing::Values (
    std::vector<int>{}                    ,
    std::vector<int>{111}                 ,
    std::vector<int>{1, 2}                ,
    std::vector<int>{2, 5, 4}             ,
    std::vector<int>{-11, 1, 2, 5, 6, 9}  
));
