#ifndef LIST_BASE_HPP
#define LIST_BASE_HPP

#include <gtest/gtest.h>
#include <vector>

extern "C" {
    #include "single_list.h"
}

/*
    The base class for each list's test case
    Contains common methods
*/
template <typename BaseType>
class ListBaseTest : public ::testing::TestWithParam<BaseType> {
protected:
    slist_t *lst;

    void SetUp()    override { Create(); }
    void TearDown() override { Delete(); }
public:
    void Create() {
        lst = slist_create(sizeof(int)); 
        IsEmpty();
    }

    void Delete() {
        slist_delete(&lst);
        EXPECT_EQ(lst, nullptr);
        EXPECT_EQ(slist_empty(lst), -1);
    }

    void ClearList() {
        slist_clear(lst);
        IsEmpty();
    }

    void IsEmpty() {
        EXPECT_NE(lst, nullptr); 
        EXPECT_EQ(slist_size(lst), 0); 
        EXPECT_EQ(slist_head(lst), nullptr); 
        EXPECT_EQ(slist_tail(lst), nullptr); 
        EXPECT_EQ(slist_item_size(lst), sizeof(int));
        EXPECT_EQ(slist_empty(lst), 1);
    }

    void IsNotEmpty() {
        EXPECT_NE(lst, nullptr); 
        EXPECT_NE(slist_head(lst), nullptr); 
        EXPECT_NE(slist_tail(lst), nullptr); 
        EXPECT_GT(slist_size(lst), 0); 
        EXPECT_EQ(slist_item_size(lst), sizeof(int));
        EXPECT_EQ(slist_empty(lst), 0);
    }

    void FillList(std::vector<int> vec){
        if(!lst) return;
        ClearList();
        for(auto v : vec) slist_append(lst, &v);
    }

    virtual void CompareWith(std::vector<int> vec) {
        if(slist_empty(lst)) { 
            IsEmpty(); 
            return; 
        }
        IsNotEmpty();
        EXPECT_EQ(slist_size(lst), vec.size());
        // Head must be equal to the first vector's item
        EXPECT_EQ(*(int*)snode_data(slist_head(lst)), vec[0]);
        // Tail must be equal to the last vector's item
        EXPECT_EQ(*(int*)snode_data(slist_tail(lst)), vec[vec.size() - 1]);   
        // Compare each list's item and vector's item
        snode_t *current = slist_head(lst);
        for(auto v : vec) {
            EXPECT_EQ(*(int*)snode_data(current), v);
            current = snode_next(current);
        }
    }

    static void FillList(slist_t *_lst, std::vector<int> vec){
        if(!_lst) return;
        slist_clear(_lst);
        for(auto v : vec) slist_append(_lst, &v);
    }

    static void IsEmpty(slist_t *li) {
        EXPECT_NE(li, nullptr);
        EXPECT_EQ(slist_size(li), 0); 
        EXPECT_EQ(slist_head(li), nullptr); 
        EXPECT_EQ(slist_tail(li), nullptr);
        EXPECT_EQ(slist_item_size(li), sizeof(int));
        EXPECT_EQ(slist_empty(li), 1);
    }

    static void IsNotEmpty(slist_t *li) {
        EXPECT_NE(li, nullptr); 
        EXPECT_NE(slist_head(li), nullptr); 
        EXPECT_NE(slist_tail(li), nullptr); 
        EXPECT_GT(slist_size(li), 0); 
        EXPECT_EQ(slist_item_size(li), sizeof(int));
        EXPECT_EQ(slist_empty(li), 0);
    }
};

#endif