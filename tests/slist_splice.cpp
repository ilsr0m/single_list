#include "test_utils.hpp"
#include "test_fixtures.hpp"

using SpliceNull = ::testing::TestWithParam<std::tuple<slist_t*, slist_t*>>;

TEST_P(SpliceNull, SpliceTest) {
    auto [dst, src] = GetParam();
    EXPECT_EQ(slist_splice(dst, src), tuti::onFailure);
    tuti::tearDown({dst, src});
}

INSTANTIATE_TEST_SUITE_P(SpliceNullSuite, SpliceNull, ::testing::Values (
    std::make_tuple(nullptr, nullptr),
    std::make_tuple(nullptr, tuti::toSlist(vec_t{1, 2, 3})),
    std::make_tuple(tuti::toSlist(vec_t{1, 2, 3}), nullptr),
    std::make_tuple(tuti::toSlist(std::vector<char>{1, 2, 3}), // item_size not same
        tuti::toSlist(std::vector<double>{1.1, 2.4, 0.1}))
));

// dst and src are the same pointers
TEST(SpliceEqPtr, SpliceTest) {
    slist_t* lst = tuti::toSlist(std::vector<int>{1, 2, 3});
    EXPECT_EQ(slist_splice(lst, lst), tuti::onFailure);
    tuti::tearDown(lst);
}

TEST_P(SpliceFull, SpliceTest) {
    SpliceFullParam concat = GetParam();
    slist_t *dst = tuti::toSlist(concat.dest);
    slist_t *src = tuti::toSlist(concat.src);
    EXPECT_EQ(slist_splice(dst, src), tuti::onSuccess);
    tuti::compareWith(dst, concat.res);
    tuti::testEmpty(src);
    tuti::tearDown({dst, src});
}

INSTANTIATE_TEST_SUITE_P(SpliceFullSuite, SpliceFull, ::testing::Values(
    SpliceFullParam({}, {}, {}),
    SpliceFullParam({1, 2, 3}, {}, {1, 2, 3}),
    SpliceFullParam({1, 2, 3}, {0}, {1, 2, 3, 0}),
    SpliceFullParam({}, {0, 8, 7, 4}, {0, 8, 7, 4}),
    SpliceFullParam({0, 8, 7, 4}, {0, 8, 7, 4}, {0, 8, 7, 4, 0, 8, 7, 4})
));
