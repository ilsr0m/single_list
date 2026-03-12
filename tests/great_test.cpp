#include <gtest/gtest.h>

extern "C"{
    #include "list_c.h"
}

list_t *list;

TEST(ListTest, CreateTest){
    list = list_create(sizeof(int));
    
    EXPECT_NE(list, nullptr); // list is not null, coz it's already initialized
    EXPECT_EQ(list->size, 0); // no any items
    EXPECT_EQ(list->head, nullptr); // obviously null
    EXPECT_EQ(list->tail, nullptr); // obviously null
    EXPECT_EQ(list->item_size, sizeof(int)); // regards to the argument in list_create
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}