#ifndef LIST_BASE_HPP
#define LIST_BASE_HPP

#include <gtest/gtest.h>
#include <vector>

extern "C" {
    #include "list_c.h"
}

/*
    The base class for each list's test case
    Contains common methods
*/
template <typename BaseType>
class ListBaseTest : public ::testing::TestWithParam<BaseType> {
protected:
    list_t *lst;

    void SetUp()    override { Create(); }
    void TearDown() override { Delete(); }
public:
    void Create() {
        lst = list_create(sizeof(int)); 
        IsEmpty();
    }

    void Delete() {
        list_delete(&lst);
        EXPECT_EQ(lst, nullptr);
        EXPECT_EQ(list_empty(lst), -1);
    }

    void ListClear() {
        list_clear(lst);
        IsEmpty();
    }

    void IsEmpty() {
        EXPECT_NE(lst, nullptr); 
        EXPECT_EQ(list_size(lst), 0); 
        EXPECT_EQ(list_head(lst), nullptr); 
        EXPECT_EQ(list_tail(lst), nullptr); 
        EXPECT_EQ(list_item_size(lst), sizeof(int));
        EXPECT_EQ(list_empty(lst), 1);
    }

    void IsNotEmpty() {
        EXPECT_NE(lst, nullptr); 
        EXPECT_NE(list_head(lst), nullptr); 
        EXPECT_NE(list_tail(lst), nullptr); 
        EXPECT_GT(list_size(lst), 0); 
        EXPECT_EQ(list_item_size(lst), sizeof(int));
        EXPECT_EQ(list_empty(lst), 0);
    }

    void FillList(std::vector<int> vec){
        if(!lst) return;
        ListClear();
        for(auto v : vec) list_append(lst, &v);
    }

    virtual void CompareWith(std::vector<int> vec) {
        if(list_empty(lst)) { 
            IsEmpty(); 
            return; 
        }
        IsNotEmpty();
        EXPECT_EQ(list_size(lst), vec.size());
        // Head must be equal to the first vector's item
        EXPECT_EQ(*(int*)node_data(list_head(lst)), vec[0]);
        // Tail must be equal to the last vector's item
        EXPECT_EQ(*(int*)node_data(list_tail(lst)), vec[vec.size() - 1]);   
        // Compare each list's item and vector's item
        node_t *current = list_head(lst);
        for(auto v : vec) {
            EXPECT_EQ(*(int*)node_data(current), v);
            current = node_next(current);
        }
    }

    static void FillList(list_t *_lst, std::vector<int> vec){
        if(!_lst) return;
        list_clear(_lst);
        for(auto v : vec) list_append(_lst, &v);
    }

    static void IsEmpty(list_t *li) {
        EXPECT_NE(li, nullptr);
        EXPECT_EQ(list_size(li), 0); 
        EXPECT_EQ(list_head(li), nullptr); 
        EXPECT_EQ(list_tail(li), nullptr);
        EXPECT_EQ(list_item_size(li), sizeof(int));
        EXPECT_EQ(list_empty(li), 1);
    }

    static void IsNotEmpty(list_t *li) {
        EXPECT_NE(li, nullptr); 
        EXPECT_NE(list_head(li), nullptr); 
        EXPECT_NE(list_tail(li), nullptr); 
        EXPECT_GT(list_size(li), 0); 
        EXPECT_EQ(list_item_size(li), sizeof(int));
        EXPECT_EQ(list_empty(li), 0);
    }
};

#endif