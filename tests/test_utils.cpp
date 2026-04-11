#include "test_utils.hpp"

namespace test_utils {
    
    void tearDown(slist_t * lst) {
        slist_delete(&lst);
        EXPECT_EQ(lst, nullptr);
    }

    void tearDown(std::vector<slist_t *> lists){
        for(auto l : lists) {
            slist_delete(&l);
            EXPECT_EQ(l, nullptr);
        }
    }

    void testEmpty(slist_t *li) {
        EXPECT_NE(li, nullptr);
        EXPECT_EQ(slist_size(li), 0); 
        EXPECT_EQ(slist_head(li), nullptr); 
        EXPECT_EQ(slist_tail(li), nullptr);
        EXPECT_EQ(slist_item_size(li), sizeof(int));
        EXPECT_EQ(slist_empty(li), 1);
    }

    void testNotEmpty(slist_t *li) {
        EXPECT_NE(li, nullptr); 
        EXPECT_NE(slist_head(li), nullptr); 
        EXPECT_NE(slist_tail(li), nullptr); 
        EXPECT_GT(slist_size(li), 0); 
        EXPECT_EQ(slist_item_size(li), sizeof(int));
        EXPECT_EQ(slist_empty(li), 0);
    }

    // usual comparator
    int ascending(const void* a, const void* b) {
        int _a = *static_cast<const int*>(a);
        int _b = *static_cast<const int*>(b);
        if(_a > _b) return 1;
        if(_a < _b) return -1;
        return 0;
    }

    // reversed comparator
    int descending(const void* a, const void* b) {
        int _a = *static_cast<const int*>(a);
        int _b = *static_cast<const int*>(b);
        if(_a > _b) return -1;
        if(_a < _b) return 1;
        return 0;
    }
}