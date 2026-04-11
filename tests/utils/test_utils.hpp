#pragma once

#include <gtest/gtest.h>
#include <vector>

extern "C" {
    #include "single_list.h"
}

namespace test_utils {
    constexpr int onSuccess = 0;
    constexpr int onFailure = -1;

    void tearDown(slist_t * lst);
    void tearDown(std::vector<slist_t *> lists);
    void testEmpty(slist_t *li);
    void testNotEmpty(slist_t *li);

    template <typename Container>
    void compareWith(slist_t * lst, Container& container, bool reverse=false) {
        using Type = typename Container::value_type;
        if(slist_empty(lst)) { 
            EXPECT_EQ(slist_size(lst), container.size());
            testEmpty(lst);
            return; 
        }
        testNotEmpty(lst);
        EXPECT_EQ(slist_size(lst), container.size());
        snode_t *current = slist_head(lst); // Compare each list's item and vector's item

        if(reverse) {
            // Head must be equal to the last vector's item
            EXPECT_EQ(*(int*)snode_data(slist_head(lst)), container[container.size() - 1]);
            // Tail must be equal to the first vector's item
            EXPECT_EQ(*(int*)snode_data(slist_tail(lst)), container[0]);
            for (auto it = container.rbegin(); it != container.rend(); ++it) {
                EXPECT_EQ(*(int*)snode_data(current), *it);
                current = snode_next(current);
            }
        }
        else{
            EXPECT_EQ(*(int*)snode_data(slist_head(lst)), container[0]); // Head must be equal to the first vector's item
            EXPECT_EQ(*(int*)snode_data(slist_tail(lst)), container[container.size() - 1]); // Tail must be equal to the last vector's item
            for(auto v : container) {
                EXPECT_EQ(*(int*)snode_data(current), v);
                current = snode_next(current);
            }
        }
    }

    template <typename T>
    const void* toVoidPtr(const T& value) { return &value; }

    template <typename Container>
    slist_t *toSlist(const Container& container) {
        using Type = typename Container::value_type;

        slist_t* slst = slist_create(sizeof(Type));
        if(!slst) return nullptr;

        for(const auto& c : container)
            slist_append(slst, &c);
        return slst;
    }    

    // usual comparator
    int ascending(const void* a, const void* b);
    // reversed comparator
    int descending(const void* a, const void* b);
}

namespace tuti = test_utils;
