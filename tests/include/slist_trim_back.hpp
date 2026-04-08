#ifndef SLIST_TRIM_BACK_HPP
#define SLIST_TRIM_BACK_HPP

#include "test_base.hpp"
#include <tuple>

/*
    Test null cases
*/
using TrimBackNull = ::testing::TestWithParam<std::tuple<slist_t*, size_t>>;

TEST_P(TrimBackNull, TrimBackTest) {
    std::tuple<slist_t*, size_t> tu = GetParam();
    slist_t* slst = std::get<0>(tu);
    EXPECT_EQ(slist_trim_back(slst, std::get<1>(tu)), API_RESULT_ON_FAILURE);
    if(slst) slist_delete(&slst);
}

INSTANTIATE_TEST_SUITE_P(TrimBackNullSuite, TrimBackNull, ::testing::Values (
    std::make_tuple(nullptr, 0),
    std::make_tuple(slist_create(sizeof(int)), 2)
));

/*
    Test full cases
*/

struct TrimBackFullParam {
    std::vector<int> base;
    unsigned count;
    std::vector<int> target;
    TrimBackFullParam(std::vector<int> base, unsigned count, std::vector<int> target) 
    : base{std::move(base)}, count{count}, target{std::move(target)} {}
};

class TrimBackFull : public TestBase<TrimBackFullParam> {};

TEST_P(TrimBackFull, TrimBackTest) {
    TrimBackFullParam trim = GetParam();
    FillList(trim.base);
    EXPECT_EQ(slist_trim_back(lst, trim.count), trim.count);
    CompareWith(trim.target);
    ClearList();
}

INSTANTIATE_TEST_SUITE_P(TrimBackFullSuite, TrimBackFull, ::testing::Values (
    TrimBackFullParam({1, 2, 3, 4, 5}, 0, {1, 2, 3, 4, 5}),
    TrimBackFullParam({1, 2, 3, 4, 5}, 1, {1, 2, 3, 4}),
    TrimBackFullParam({1, 2, 3, 4, 5}, 2, {1, 2, 3}),
    TrimBackFullParam({1, 2, 3, 4, 5}, 3, {1, 2}),
    TrimBackFullParam({1, 2, 3, 4, 5}, 4, {1}),
    TrimBackFullParam({1, 2, 3, 4, 5}, 5, {})
));


#endif