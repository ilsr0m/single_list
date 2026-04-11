#include "test_utils.hpp"
#include "test_fixtures.hpp"

TEST_P(ConcatNull, ConcatTest) {
    auto [dest, src] = GetParam();
    EXPECT_EQ(slist_concat(dest, src), tuti::onFailure); 
    tuti::tearDown({dest, src});
}

INSTANTIATE_TEST_SUITE_P(ConcatNullSuite, ConcatNull, ::testing::Values (
    std::make_tuple(nullptr, nullptr),
    std::make_tuple(nullptr, tuti::toSlist(std::vector<int>{1, 2, 3})),
    std::make_tuple(tuti::toSlist(std::vector<int>{1, 2, 3}), nullptr),
    std::make_tuple(tuti::toSlist(std::vector<char>{1, 2, 3}), // item_sizes not same
        tuti::toSlist(std::vector<double>{1.1, 2.4, 0.1}))
));

// dst and src are the same pointers
TEST(ConcatEqPtr, ConcatTest) {
    slist_t* lst = tuti::toSlist(std::vector<int>{1, 2, 3});
    EXPECT_EQ(slist_concat(lst, lst), tuti::onFailure);
    tuti::tearDown(lst);
}

TEST_P(ConcatFull, ConcatTest) {
    ConcatFullParam concat = GetParam();
    
    slist_t *dst = tuti::toSlist(concat.dest);
    slist_t *src = tuti::toSlist(concat.src);

    EXPECT_EQ(slist_concat(dst, src), tuti::onSuccess);

    tuti::compareWith(dst, concat.res);
    tuti::compareWith(src, concat.src);

    tuti::tearDown({dst, src});
}

INSTANTIATE_TEST_SUITE_P(ConcatFullSuite, ConcatFull, ::testing::Values(
    ConcatFullParam({}, {}, {}),
    ConcatFullParam({1, 2, 3}, {}, {1, 2, 3}),
    ConcatFullParam({1, 2, 3}, {0}, {1, 2, 3, 0}),
    ConcatFullParam({}, {0, 8, 7, 4}, {0, 8, 7, 4}),
    ConcatFullParam({0, 8, 7, 4}, {0, 8, 7, 4}, {0, 8, 7, 4, 0, 8, 7, 4})
));
