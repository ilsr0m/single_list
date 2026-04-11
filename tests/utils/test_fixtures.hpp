#pragma once

#include <gtest/gtest.h>

#include <vector>
#include <tuple>
#include <optional>

extern "C" {
    #include "single_list.h"
}

using vec_t = std::vector<int>;

// -------------------------------------------------
// slist_append
// -------------------------------------------------
using AppendNull = ::testing::TestWithParam<std::tuple<slist_t *, std::optional<int>>>;
using AppendFull = ::testing::TestWithParam<std::vector<int>>;

// -------------------------------------------------
// slist_prepend
// -------------------------------------------------
using PrependNull = ::testing::TestWithParam<std::tuple<slist_t *, std::optional<int>>>;
using PrependFull = ::testing::TestWithParam<std::vector<int>>;

// -------------------------------------------------
// slist_insert
// -------------------------------------------------
using InsertNull = ::testing::TestWithParam<std::tuple<slist_t *, std::optional<int>, size_t>>;
struct InsertFullParam {
    vec_t base, target;
    int item, position, result;
    InsertFullParam( vec_t base, vec_t target, int item, int position, int result) 
        : base{std::move(base)}, target{std::move(target)},
         item{item}, position{position}, result{result} {}
};
using InsertFull = ::testing::TestWithParam<InsertFullParam>;

// -------------------------------------------------
// slist_at
// -------------------------------------------------
using AtNull = ::testing::TestWithParam<std::tuple<slist_t *, size_t>>;
using AtFull = ::testing::TestWithParam<std::tuple<vec_t, size_t, int>>;

// -------------------------------------------------
// slist_set
// -------------------------------------------------
using SetNull = ::testing::TestWithParam<std::tuple<slist_t *, size_t, std::optional<int>>>;
struct SetFullParam {
    struct DataParam {
        size_t index;
        int item;
        DataParam (size_t index, int item) : 
        index{index}, item{item} {}
    };
    vec_t base, target;
    std::vector<DataParam> datas;
    int result;
    SetFullParam(vec_t base, std::vector<DataParam> datas, vec_t target, int result) 
        : base{std::move(base)}, datas{std::move(datas)}, target{std::move(target)}, result{result} {}
};
using SetFull = ::testing::TestWithParam<SetFullParam>;

// -------------------------------------------------
// slist_remove
// -------------------------------------------------
using RemoveNull = ::testing::TestWithParam<std::tuple<slist_t*, std::optional<int>, comparator_fn>>;
struct RemoveFullParam {
    comparator_fn cmp;
    vec_t base, keys, target;
    int result;
    RemoveFullParam(vec_t base, vec_t keys, vec_t target, comparator_fn cmp, int result) : 
        cmp{cmp}, keys{std::move(keys)}, base{std::move(base)}, target{std::move(target)}, result{result} {}
};
using RemoveFull = ::testing::TestWithParam<RemoveFullParam>;

// -------------------------------------------------
// slist_remove_all
// -------------------------------------------------
using RemoveAllNull = ::testing::TestWithParam<std::tuple<slist_t*, std::optional<int>, comparator_fn>>;
using RemoveAllFull = ::testing::TestWithParam<RemoveFullParam>;

// -------------------------------------------------
// slist_remove_at
// -------------------------------------------------
using RemoveAtNull = ::testing::TestWithParam<std::tuple<slist_t*, const size_t>>;
struct RemoveAtFullParam {
    vec_t base, positions, target;
    int result;
    RemoveAtFullParam(vec_t base, vec_t positions, vec_t target, int result) 
        : base{std::move(base)}, positions{std::move(positions)}, target{std::move(target)}, result{result} {}
};
using RemoveAtFull = ::testing::TestWithParam<RemoveAtFullParam>;

// -------------------------------------------------
// slist_trim_front
// -------------------------------------------------
using TrimFrontNull = ::testing::TestWithParam<std::tuple<slist_t*, size_t>>;
struct TrimFrontFullParam {
    vec_t base, target;
    unsigned count;
    TrimFrontFullParam(vec_t base, unsigned count, vec_t target) 
    : base{std::move(base)}, count{count}, target{std::move(target)} {}
};
using TrimFrontFull = ::testing::TestWithParam<TrimFrontFullParam>;

// -------------------------------------------------
// slist_trim_back
// -------------------------------------------------
using TrimBackNull = ::testing::TestWithParam<std::tuple<slist_t*, size_t>>;
using TrimBackFull = ::testing::TestWithParam<TrimFrontFullParam>;

// -------------------------------------------------
// slist_trim_range
// -------------------------------------------------
using TrimRangeNull = ::testing::TestWithParam<std::tuple<slist_t*, size_t, size_t>>;
struct TrimRangeFullParam {
    vec_t base, target;
    size_t start, end;
    TrimRangeFullParam(vec_t base, size_t start, size_t end, vec_t target)
        : base{std::move(base)}, start{start}, end{end}, target{std::move(target)} {}
};
using TrimRangeFull = ::testing::TestWithParam<TrimRangeFullParam>;

// -------------------------------------------------
// slist_pop_front
// -------------------------------------------------
using PopFrontNull = ::testing::TestWithParam<slist_t*>;
struct PopFrontFullParam {
    vec_t base, target, popped;
    unsigned count;
    PopFrontFullParam(vec_t base, unsigned popCount, vec_t target, vec_t popped) 
        : base{std::move(base)}, count{popCount}, target{std::move(target)}, popped{std::move(popped)} {}
};
using PopFrontFull = ::testing::TestWithParam<PopFrontFullParam>;

// -------------------------------------------------
// slist_pop_back
// -------------------------------------------------
using PopBackNull = ::testing::TestWithParam<slist_t*>;
using PopBackFull = ::testing::TestWithParam<PopFrontFullParam>;

// -------------------------------------------------
// slist_copy
// -------------------------------------------------
using CopyNull = ::testing::TestWithParam<slist_t*>;
using CopyFull = ::testing::TestWithParam<vec_t>;

// -------------------------------------------------
// slist_splice
// -------------------------------------------------
using SpliceNull = ::testing::TestWithParam<std::tuple<slist_t*, slist_t*>>;
struct SpliceFullParam {
    vec_t dest, src, res;
    SpliceFullParam(vec_t dest, vec_t src, vec_t res) 
        : dest{std::move(dest)},src{std::move(src)}, res{std::move(res)} {}
};
using SpliceFull = ::testing::TestWithParam<SpliceFullParam>;

// -------------------------------------------------
// slist_concat
// -------------------------------------------------
using ConcatNull = ::testing::TestWithParam<std::tuple<slist_t*,slist_t*>>;
struct ConcatFullParam {
    vec_t dest, src, res;
    ConcatFullParam(vec_t dest, vec_t src, vec_t res) 
        : dest{std::move(dest)},src{std::move(src)}, res{std::move(res)} {}
};
using ConcatFull = ::testing::TestWithParam<ConcatFullParam>;

// -------------------------------------------------
// slist_contains
// -------------------------------------------------
using ContainsNull = ::testing::TestWithParam<std::tuple<slist_t*, std::optional<int>, comparator_fn>>;
using ContainsFull = ::testing::TestWithParam<std::tuple<std::vector<int>, int, int>>;

// -------------------------------------------------
// slist_count
// -------------------------------------------------
using CountNull = ::testing::TestWithParam<std::tuple<slist_t*, std::optional<int>, comparator_fn>>;
using CountFull = ::testing::TestWithParam<std::tuple<std::vector<int>, int, int>>;

// -------------------------------------------------
// slist_find
// -------------------------------------------------
using FindNull = ::testing::TestWithParam<std::tuple<slist_t*, std::optional<int>, comparator_fn>>;
using FindFull = ::testing::TestWithParam<std::tuple<std::vector<int>, int>>;

// -------------------------------------------------
// slist_sort
// -------------------------------------------------
using SortNull = ::testing::TestWithParam<std::tuple<slist_t *, comparator_fn>>;
struct SortFullParam {
    comparator_fn comparator;
    vec_t disordered, sorted;
    SortFullParam(comparator_fn comparator, vec_t disordered, vec_t sorted) : 
        comparator{comparator}, disordered{std::move(disordered)}, sorted{std::move(sorted)} {}
};
using SortFull = ::testing::TestWithParam<SortFullParam>;

// -------------------------------------------------
// slist_bogosort
// -------------------------------------------------