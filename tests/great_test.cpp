#include <gtest/gtest.h>

extern "C"{
    #include "list_c.h"
}



TEST(ListTest, CreateTest){
    list_t *list = list_create(sizeof(int));
    
    EXPECT_NE(list, nullptr); // list is not null, coz it's already initialized
    EXPECT_EQ(list->size, 0); // no any items
    EXPECT_EQ(list->head, nullptr); // obviously null
    EXPECT_EQ(list->tail, nullptr); // obviously null
    EXPECT_EQ(list->item_size, sizeof(int)); // regards to the argument in list_create

    // simple check of function list_delete
    list_delete(&list);
    EXPECT_EQ(list, nullptr);
}

TEST(ListTest, AppendOnceTest){
    list_t *list = list_create(sizeof(int));
    int value = 5;
    list_append(list, &value);

    EXPECT_NE(list, nullptr);
    EXPECT_EQ(list->size, 1); // plus one item
    EXPECT_NE(list->head, nullptr); 
    EXPECT_NE(list->tail, nullptr);  
    EXPECT_EQ(list->tail, list->head); // head and tail must be equal
    
    int item = *(int*)list->tail->item;
    EXPECT_EQ(item, value); // item is equal to value
    EXPECT_EQ(list->item_size, sizeof(int));

    // simple check of function list_delete
    list_delete(&list);
    EXPECT_EQ(list, nullptr);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}