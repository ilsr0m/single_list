#include <gtest/gtest.h>

extern "C"{
    #include "list_c.h"

    typedef struct TestPoint{
        double x;
        double y;
    }test_point_t;
}

// for append, prepend functions
int test_integers[10] = {9, 2, -1, 5, 7, 0, 4, 5, 3, 1};
double test_doubles[10] = {-0.6, 1.7, 15.02, 2.3, 6.33, -19.29, 0.5, 5.11, 3.93, 24.0};

test_point_t test_points[10] = {
    { 3.1, -6.22 },
    { 0.0, 9.7 },
    { 1.2, -0.2 },
    { 7.3, -6.22 },
    { -6.1, -9.09 },
    { 9.1, 15.15 },
    { 1.11, -6.22 },
    { 3.87, 88.14 },
    { 4.44, -14.87 },
    { 2.28, 27.88 }
};

// this comparator is needed as arg in list_remove and list_remove_all
int remove_cmp(const void* _item, const void* _key){
    return (*(int*)_item == *(int*)_key) ? 0 : -1;
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

TEST(ListTest, PrependOnceTest){
    list_t *list = list_create(sizeof(int));
    int value = 5;
    list_prepend(list, &value);

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

TEST(ListTest, AppendSeveralTest){
    list_t *list = list_create(sizeof(int));

    // fill list with some values
    for(int i = 0; i < 10; i++)
        list_append(list, &test_integers[i]);
    
    EXPECT_NE(list, nullptr);
    EXPECT_NE(list->head, nullptr); 
    EXPECT_NE(list->tail, nullptr);
    EXPECT_EQ(list->size, 10); 
    
    // check each value in list
    node_t *list_iter = list->head;
    int value_index = 0;
    while(list_iter != nullptr){
        EXPECT_EQ(*(int*)list_iter->item, test_integers[value_index]);
        value_index++;
        list_iter = list_iter->next;
    }

    list_delete(&list);
    EXPECT_EQ(list, nullptr);
}

TEST(ListTest, PrependSeveralTest){
    list_t *list = list_create(sizeof(int));
    
    // fill list with some values
    for(int i = 0; i < 10; i++)
        list_prepend(list, &test_integers[i]);
    
    EXPECT_NE(list, nullptr);
    EXPECT_NE(list->head, nullptr); 
    EXPECT_NE(list->tail, nullptr);
    EXPECT_EQ(list->size, 10); 
    
    // check each value in list
    node_t *list_iter = list->head;
    int value_index = 10;
    while(list_iter != nullptr){
        value_index--;
        EXPECT_EQ(*(int*)list_iter->item, test_integers[value_index]);
        list_iter = list_iter->next;
    }

    list_delete(&list);
    EXPECT_EQ(list, nullptr);
}

TEST(ListTest, ClearTest){
    list_t *list = list_create(sizeof(double));
    // fill list with values to make it not empty
    for(int i = 0; i < 10; i++)
        list_append(list, &test_doubles[i]);
    // then clear it to check if this function works properly
    list_clear(list);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(list->head, nullptr); 
    EXPECT_EQ(list->tail, nullptr);
    EXPECT_EQ(list->size, 0); 
    EXPECT_EQ(list->item_size, sizeof(double)); 

    list_delete(&list);
    EXPECT_EQ(list, nullptr);
}

TEST(ListTest, InsertTest){
    list_t *list = list_create(sizeof(test_point_t));
    int result = 0;

    // fill list with values to make it not empty
    for(int i = 0; i < 10; i++)
        list_append(list, &test_points[i]);

    printf("a1\n");
    // put it as first item
    test_point_t p0 = {2.0, 2.0};
    result = list_insert(list, &p0, 0);
    
    EXPECT_NE(list, nullptr);
    EXPECT_EQ((*(test_point_t*)list->head->item).x, p0.x); 
    EXPECT_EQ((*(test_point_t*)list->head->item).y, p0.y); 
    EXPECT_EQ(result, 0); 
    EXPECT_EQ(list->size, 11);
    
    // put it as last item
    test_point_t p1 = {4.0, 4.0};
    result = list_insert(list, &p1, list->size);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ((*(test_point_t*)list->tail->item).x, p1.x); 
    EXPECT_EQ((*(test_point_t*)list->tail->item).y, p1.y); 
    EXPECT_EQ(result, 0); 
    EXPECT_EQ(list->size, 12);

    // put it in the middle
    test_point_t p3 = {5.55, 5.55};
    int position = 7;
    result = list_insert(list, &p3, position);
    node_t *list_iter = list->head;
    int count = 0;
    while (list_iter != nullptr)
    {
        if(position == count) 
            break;
        count++;
        list_iter = list_iter->next;
    }

    EXPECT_NE(list, nullptr); 
    EXPECT_EQ((*(test_point_t*)list_iter->item).x, p3.x); 
    EXPECT_EQ((*(test_point_t*)list_iter->item).y, p3.y);  
    EXPECT_EQ(result, 0); 
    EXPECT_EQ(list->size, 13);

    list_delete(&list);
    EXPECT_EQ(list, nullptr);
}

TEST(ListTest, FrontTest){
    list_t *list;
    void* result;

    list = list_create(sizeof(int));
    int v1 = 3, v2 = 0;

    list_prepend(list, &v1);
    result = list_front(list);
    EXPECT_EQ(*(int*)result, v1);

    list_prepend(list, &v2);
    result = list_front(list);
    EXPECT_EQ(*(int*)result, v2);

    list_delete(&list);
    EXPECT_EQ(list, nullptr);
}

TEST(ListTest, BackTest){
    list_t *list;
    void* result;

    list = list_create(sizeof(int));
    int v1 = 3, v2 = 0;

    list_append(list, &v1);
    result = list_back(list);
    EXPECT_EQ(*(int*)result, v1);

    list_append(list, &v2);
    result = list_back(list);
    EXPECT_EQ(*(int*)result, v2);

    list_delete(&list);
    EXPECT_EQ(list, nullptr);
}

TEST(ListTest, RemoveOnceTest){
    list_t* list = list_create(sizeof(int));

    // fill list with some values
    for(int i = 0; i < 10; i++)
        list_append(list, &test_integers[i]);
    
    // Test bundle
    void* removed_item;
    int test_ints_count;
    node_t* test_iter;
    int test_ints_1[9] = {2, -1, 5, 7, 0, 4, 5, 3, 1};  // for Test 1
    int test_ints_2[8] = {2, -1, 5, 7, 0, 4, 5, 3};     // for Test 2
    int test_ints_3[7] = {2, -1, 5, 7, 4, 5, 3};        // for Test 3
    int test_ints_4[6] = {2, -1, 7, 4, 5, 3};           // for Test 4 and Test 5

    // Test 1 - Remove head item
    int head_key = 9; // remove 9 value
    removed_item = list_remove(list, &head_key, remove_cmp);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(*(int*)removed_item, head_key);
    EXPECT_EQ(list->size, 9);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_1[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    // Test 2 - Remove tail item
    int tail_key = 1; // remove 1 value
    removed_item = list_remove(list, &tail_key, remove_cmp);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(*(int*)removed_item, tail_key);
    EXPECT_EQ(list->size, 8);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_2[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    // Test 3 - Remove middle item
    int middle_key = 0; // remove 0 value
    removed_item = list_remove(list, &middle_key, remove_cmp);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(*(int*)removed_item, middle_key);
    EXPECT_EQ(list->size, 7);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_3[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    // Test 4 - Remove value 5 (it must remove only first item with such value)
    int duplicate_key = 5;
    removed_item = list_remove(list, &duplicate_key, remove_cmp);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(*(int*)removed_item, duplicate_key);
    EXPECT_EQ(list->size, 6);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_4[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    // Test 5 - Try to remove value which the list does not have
    int fake_key = 228;
    removed_item = list_remove(list, &fake_key, remove_cmp);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_item, nullptr);
    EXPECT_EQ(list->size, 6);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_4[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    list_delete(&list);
    EXPECT_EQ(list, nullptr);
}

TEST(ListTest, RemoveAllTest){
    list_t* list = list_create(sizeof(int));

    // fill list with some values
    for(int i = 0; i < 10; i++)
        list_append(list, &test_integers[i]);
    
    // Test bundle
    int removed_items_count; // for return type
    int test_ints_count;
    node_t* test_iter;
    int test_ints_1[9] = {2, -1, 5, 7, 0, 4, 5, 3, 1};  // for Test 1
    int test_ints_2[8] = {2, -1, 5, 7, 0, 4, 5, 3};     // for Test 2
    int test_ints_3[7] = {2, -1, 5, 7, 4, 5, 3};        // for Test 3
    int test_ints_4[5] = {2, -1, 7, 4, 3};              // for Test 4 and Test 5

    // Test 1 - Remove head item
    int head_key = 9; // remove 9 value
    removed_items_count = list_remove_all(list, &head_key, remove_cmp);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_items_count, 1);
    EXPECT_EQ(list->size, 9);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_1[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    // Test 2 - Remove tail item
    int tail_key = 1; // remove 1 value
    removed_items_count = list_remove_all(list, &tail_key, remove_cmp);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_items_count, 1);
    EXPECT_EQ(list->size, 8);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_2[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    // Test 3 - Remove middle item
    int middle_key = 0; // remove 0 value
    removed_items_count = list_remove_all(list, &middle_key, remove_cmp);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_items_count, 1);
    EXPECT_EQ(list->size, 7);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_3[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    // Test 4 - Remove value 5 (it must remove all item with such value)
    int duplicate_key = 5; // remove all fives
    // added the same value more
    list_append(list, &duplicate_key);
    list_append(list, &duplicate_key);
    list_append(list, &duplicate_key);
    list_prepend(list, &duplicate_key);
    list_prepend(list, &duplicate_key);
    list_insert(list, &duplicate_key, 4);
    // start removing all fives
    removed_items_count = list_remove_all(list, &duplicate_key, remove_cmp);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_items_count, 8);
    EXPECT_EQ(list->size, 5);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_4[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    // Test 5 - Try to remove value which the list does not have
    int fake_key = 228;
    removed_items_count = list_remove_all(list, &fake_key, remove_cmp);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_items_count, 0);
    EXPECT_EQ(list->size, 5);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_4[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    list_delete(&list);
    EXPECT_EQ(list, nullptr);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}