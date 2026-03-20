#include <gtest/gtest.h>
#include <vector>
#include <tuple>

extern "C"{
    #include "list_c.h"
}

class ListTest : public testing::Test {
protected:
    list_t *list;

    int testValues[10] = {9, 2, -1, 5, 7, 0, 4, 5, 3, 1};
    int testValuesSorted[10] = {-1, 0, 1, 2, 3, 4, 5, 5, 7, 9};

    std::vector<int> test_data = {9, 2, -1, 5, 7, 0, 4, 5, 3, 1};

    void SetUp() { 
        list = list_create(sizeof(int)); 
        IsEmpty();
    }

    void TearDown() { 
        list_delete(&list);
        EXPECT_EQ(list, nullptr);
    }

    void IsEmpty() {
        EXPECT_NE(list, nullptr); // list is not null, coz it's already initialized
        EXPECT_EQ(list->list_size, 0); // no any items
        EXPECT_EQ(list->head, nullptr); // obviously null
        EXPECT_EQ(list->tail, nullptr); // obviously null
        EXPECT_EQ(list->item_size, sizeof(int));
    }

    static void IsEmpty(list_t *li) {
        EXPECT_NE(li, nullptr); // list is not null, coz it's already initialized
        EXPECT_EQ(li->list_size, 0); // no any items
        EXPECT_EQ(li->head, nullptr); // obviously null
        EXPECT_EQ(li->tail, nullptr); // obviously null
        EXPECT_EQ(li->item_size, sizeof(int));
    }

    void IsNotEmpty() {
        EXPECT_NE(list, nullptr); // list is not null, coz it's already initialized
        EXPECT_NE(list->head, nullptr); 
        EXPECT_NE(list->tail, nullptr); 
        EXPECT_GT(list->list_size, 0); 
        EXPECT_EQ(list->item_size, sizeof(int));
    }

    static void IsNotEmpty(list_t *li) {
        EXPECT_NE(li, nullptr); // list is not null, coz it's already initialized
        EXPECT_NE(li->head, nullptr); 
        EXPECT_NE(li->tail, nullptr); 
        EXPECT_GT(li->list_size, 0); 
        EXPECT_EQ(li->item_size, sizeof(int));
    }

    void Clear() {
        list_clear(list);
        IsEmpty();
    }

    void FillList(int (*add)(list_t* list, const void* data)){
        Clear();
        int result;
        for(int i = 0; i < 10; i++){
            result = add(list, &test_data[i]);
            EXPECT_EQ(result, 0);
        }
        IsNotEmpty();
        EXPECT_EQ(list->list_size, 10);
    }

    void TestData(std::vector<int> test_values){
        node_t* current_node = list->head;
        int count = 0;
        while(current_node){
            EXPECT_EQ(*(int*)current_node->item, test_values[count]);
            current_node = current_node->next;
            count++;
        }
    }

    // this comparator is needed as arg in list_remove and list_remove_all
    static int Comparator(const void* _item, const void* _key) {
        return (*(int*)_item == *(int*)_key) ? 0 : -1;
    }

    static int Predicate(const void* _item, void* _context){
        return (*(int*)_item > *(int*)_context) ? 0 : 1;
    }
};

// function: list_create
TEST_F(ListTest, CreateTest) {
    // check it with null item size
    list_t* create = list_create(0);
    EXPECT_EQ(create, nullptr);
}

// function: list_append -> once
TEST_F(ListTest, AppendOnceTest) {
    int result; // return value of funtion 
    int value = 5;

    // First of all, check if item is null
    result = list_append(list, nullptr);
    EXPECT_EQ(result, -1);
    IsEmpty();

    // Check with real value
    result = list_append(list, &value);
    EXPECT_EQ(result, 0);
    IsNotEmpty();

    EXPECT_EQ(list->list_size, 1);     // plus one item
    EXPECT_EQ(list->tail, list->head); // head and tail must be equal
    EXPECT_EQ(*(int*)list->head->item, value); // item is equal to value

    Clear();
}

// function: list_prepend -> once
TEST_F(ListTest, PrependOnceTest) {
    int result; // return value of funtion 
    int value = 5;

    // First of all, check if item is null
    result = list_prepend(list, nullptr);
    EXPECT_EQ(result, -1);
    IsEmpty();

    // Check with real value
    result = list_prepend(list, &value);
    EXPECT_EQ(result, 0);
    IsNotEmpty();

    EXPECT_EQ(list->list_size, 1); // plus one item
    EXPECT_EQ(list->tail, list->head); // head and tail must be equal
    EXPECT_EQ(*(int*)list->head->item, value); // item is equal to value

    Clear();
}

// function: list_append -> multiple times
TEST_F(ListTest, AppendMultipleTest) {
    int result; // return value of funtion 
    // fill list with some values
    FillList(list_append);
    
    // check each value in list
    node_t *list_iter = list->head;
    int value_index = 0;
    while(list_iter != nullptr){
        EXPECT_EQ(*(int*)list_iter->item, testValues[value_index]);
        value_index++;
        list_iter = list_iter->next;
    }

    // check if item is null
    result = list_append(list, nullptr);
    EXPECT_EQ(result, -1);
    IsNotEmpty();
    EXPECT_EQ(list->list_size, 10); 

    Clear();
}

// function: list_prepend -> multiple times
TEST_F(ListTest, PrependMultipleTest){
    int result; // return value of funtion 
    
    // fill list with some values
    FillList(list_prepend);
    
    // check each value in list
    node_t *list_iter = list->head;
    int value_index = 10;
    while(list_iter != nullptr){
        value_index--;
        EXPECT_EQ(*(int*)list_iter->item, testValues[value_index]);
        list_iter = list_iter->next;
    }

    // check if item is null
    result = list_prepend(list, nullptr);
    EXPECT_EQ(result, -1);
    IsNotEmpty();
    EXPECT_EQ(list->list_size, 10); 

    Clear();
}

// function: list_insert
TEST_F(ListTest, InsertTest){
    int result = 0; // return value of funtion 
    // fill list with values to make it not empty
    FillList(list_append);

    // insert as head item
    int v0 = 1;
    result = list_insert(list, &v0, 0);
    EXPECT_EQ(result, 0); 
    IsNotEmpty();
    EXPECT_EQ(*(int*)list->head->item, v0); 
    EXPECT_EQ(list->list_size, 11);
    
    // insert as last item
    int v1 = 1;
    result = list_insert(list, &v1, list->list_size);
    EXPECT_EQ(result, 0); 
    IsNotEmpty();
    EXPECT_EQ(*(int*)list->tail->item, v1);
    EXPECT_EQ(list->list_size, 12);

    // put it in the middle
    int v3 = 8;
    int position = 7;
    result = list_insert(list, &v3, position);
    EXPECT_EQ(result, 0); 
    IsNotEmpty();
    EXPECT_EQ(list->list_size, 13);

    node_t *list_iter = list->head;
    int count = 0;
    while (list_iter != nullptr) {
        if(position == count) break;
        count++;
        list_iter = list_iter->next;
    }
    EXPECT_EQ(*(int*)list_iter->item, v3);
    
    Clear();
}

// function: list_front
TEST_F(ListTest, FrontTest) {
    void* result; // return value of funtion 
    int v1 = 3, v2 = 0;

    result = list_front(list);
    EXPECT_EQ(result, nullptr);

    list_prepend(list, &v1);
    result = list_front(list);
    EXPECT_EQ(*(int*)result, v1);

    list_prepend(list, &v2);
    result = list_front(list);
    EXPECT_EQ(*(int*)result, v2);
    
    Clear();
}

// function: list_back
TEST_F(ListTest, BackTest){
    void* result; // return value of funtion 
    int v1 = 3, v2 = 0;

    result = list_back(list);
    EXPECT_EQ(result, nullptr);

    list_append(list, &v1);
    result = list_back(list);
    EXPECT_EQ(*(int*)result, v1);

    list_append(list, &v2);
    result = list_back(list);
    EXPECT_EQ(*(int*)result, v2);

    Clear();
}

// function: list_remove
TEST_F(ListTest, RemoveOnceTest){
    // Test bundle
    void* removed_item;
    int test_ints_count;
    node_t* test_iter;
    int test_ints_1[9] = {2, -1, 5, 7, 0, 4, 5, 3, 1};  // for Test 1
    int test_ints_2[8] = {2, -1, 5, 7, 0, 4, 5, 3};     // for Test 2
    int test_ints_3[7] = {2, -1, 5, 7, 4, 5, 3};        // for Test 3
    int test_ints_4[6] = {2, -1, 7, 4, 5, 3};           // for Test 4 and Test 5

    // Test 0 - only one item in list
    int once = 4;
    list_append(list, &once);
    removed_item = list_remove(list, &once, ListTest::Comparator);
    EXPECT_EQ(*(int*)removed_item, once);
    IsEmpty();

    FillList(list_append);
    
    // Test 1 - Remove head item
    int head_key = 9; // remove 9 value
    removed_item = list_remove(list, &head_key, ListTest::Comparator);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(*(int*)removed_item, head_key);
    EXPECT_EQ(list->list_size, 9);
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
    removed_item = list_remove(list, &tail_key, ListTest::Comparator);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(*(int*)removed_item, tail_key);
    EXPECT_EQ(list->list_size, 8);
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
    removed_item = list_remove(list, &middle_key, ListTest::Comparator);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(*(int*)removed_item, middle_key);
    EXPECT_EQ(list->list_size, 7);
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
    removed_item = list_remove(list, &duplicate_key, ListTest::Comparator);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(*(int*)removed_item, duplicate_key);
    EXPECT_EQ(list->list_size, 6);
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
    removed_item = list_remove(list, &fake_key, ListTest::Comparator);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_item, nullptr);
    EXPECT_EQ(list->list_size, 6);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_4[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    Clear();
}

// function: list_remove_all
TEST_F(ListTest, RemoveAllTest){
    // Test bundle
    int removed_items_count; // for return type
    int test_ints_count;
    node_t* test_iter;
    int test_ints_1[9] = {2, -1, 5, 7, 0, 4, 5, 3, 1};  // for Test 1
    int test_ints_2[8] = {2, -1, 5, 7, 0, 4, 5, 3};     // for Test 2
    int test_ints_3[7] = {2, -1, 5, 7, 4, 5, 3};        // for Test 3
    int test_ints_4[5] = {2, -1, 7, 4, 3};              // for Test 4 and Test 5

    // Test 0 - only one item in list
    int once = 4;
    list_append(list, &once);
    removed_items_count = list_remove_all(list, &once, ListTest::Comparator);
    EXPECT_EQ(removed_items_count, 1);
    IsEmpty();

    // fill list with some values
    FillList(list_append);
    
    // Test 1 - Remove head item
    int head_key = 9; // remove 9 value
    removed_items_count = list_remove_all(list, &head_key, ListTest::Comparator);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_items_count, 1);
    EXPECT_EQ(list->list_size, 9);
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
    removed_items_count = list_remove_all(list, &tail_key, ListTest::Comparator);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_items_count, 1);
    EXPECT_EQ(list->list_size, 8);
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
    removed_items_count = list_remove_all(list, &middle_key, ListTest::Comparator);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_items_count, 1);
    EXPECT_EQ(list->list_size, 7);
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
    removed_items_count = list_remove_all(list, &duplicate_key, ListTest::Comparator);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_items_count, 8);
    EXPECT_EQ(list->list_size, 5);
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
    removed_items_count = list_remove_all(list, &fake_key, ListTest::Comparator);
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(removed_items_count, 0);
    EXPECT_EQ(list->list_size, 5);
    // check each item value
    test_iter = list->head;
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)test_iter->item, test_ints_4[test_ints_count]);
        test_iter = test_iter->next;
        test_ints_count++;
    }

    Clear();
}

// function: list_pop_front
TEST_F(ListTest, PopFrontTest){
    int val = 22;
    void* item;

    //empty
    item = list_pop_front(list);
    EXPECT_EQ(item, nullptr);

    // Remove when it has only one item
    list_prepend(list, &val);
    item = list_pop_front(list);
    IsEmpty();
    EXPECT_EQ(*(int*)item, val);

    // Remove when several items
    FillList(list_append);
    item = list_pop_front(list);
    EXPECT_EQ(*(int*)item, 9);
    IsNotEmpty();
    EXPECT_EQ(list->list_size, 9);

    Clear();
}

// function: list_pop_back
TEST_F(ListTest, PopBackTest) {
    int val = 22;
    void* item;

    // empty
    item = list_pop_front(list);
    EXPECT_EQ(item, nullptr);

    // Pop when it has only one item    
    list_append(list, &val);
    item = list_pop_back(list);
    IsEmpty();
    EXPECT_EQ(*(int*)item, val);

    // Pop when several items
    FillList(list_append);

    item = list_pop_back(list);
    EXPECT_EQ(*(int*)item, 1);
    IsNotEmpty();
    EXPECT_EQ(list->list_size, 9);

    Clear();
}

// function: list_at
TEST_F(ListTest, AtTest) {
    void* item;

    // empty list
    item = list_at(list, 0);
    EXPECT_EQ(item, nullptr);
    IsEmpty();

    // prepare values
    FillList(list_append);

    // try to get value by valid index
    // scan all values
    for(int i = 0; i < 10; i++) {
        item = list_at(list, i);
        EXPECT_NE(item, nullptr);
        EXPECT_EQ(*(int*)item, testValues[i]);
        IsNotEmpty();
    }

    // try to get value by invalid index
    // scan 10 invalid indexes
    for(int i = 10; i < 20; i++){
        item = list_at(list, i);
        EXPECT_EQ(item, nullptr);
        IsNotEmpty();
    }

    Clear();
}

// function: list_copy
TEST_F(ListTest, CopyTest){
    list_t* copy;
    
    // test with NULL argument
    copy = list_copy(nullptr);
    EXPECT_EQ(copy, nullptr);

    // copy of empty list
    copy = list_copy(list);
    ListTest::IsEmpty(copy);
    EXPECT_NE(copy, list);

    list_delete(&copy);
    EXPECT_EQ(copy, nullptr);

    // copy of one item list
    int val = 3; 
    list_append(list, &val);
    copy = list_copy(list);

    ListTest::IsNotEmpty(copy);
    EXPECT_NE(copy, list);
    EXPECT_EQ(copy->list_size, 1);
    EXPECT_NE(copy->head, nullptr);
    EXPECT_NE(copy->tail, nullptr);
    EXPECT_EQ(copy->tail, copy->head);

    list_delete(&copy);
    EXPECT_EQ(copy, nullptr);

    // copy of filled list - 10 items
    Clear();
    FillList(list_append);
    copy = list_copy(list);
    ListTest::IsNotEmpty();
    EXPECT_EQ(copy->list_size, 10);
    node_t* copy_iter = copy->head;
    node_t* list_iter = list->head;
    while (copy_iter && list_iter){
        EXPECT_NE(copy_iter->item, nullptr);
        EXPECT_EQ(*(int*)copy_iter->item, *(int*)list_iter->item);
        copy_iter = copy_iter->next;
        list_iter = list_iter->next;
    }

    list_delete(&copy);
    EXPECT_EQ(copy, nullptr);

    Clear();
}

// function: list_contains
TEST_F(ListTest, ContainsTest){
    int result;
    int v1 = 0, v2 = 5, v3 = 170;

    // if any argument is null: -1
    result = list_contains(nullptr, &v1, Comparator);
    EXPECT_EQ(result, -1);
    result = list_contains(list, nullptr, Comparator);
    EXPECT_EQ(result, -1);
    result = list_contains(list, &v1, nullptr);
    EXPECT_EQ(result, -1);
    result = list_contains(nullptr, nullptr, Comparator);
    EXPECT_EQ(result, -1);
    result = list_contains(nullptr, &v1, nullptr);
    EXPECT_EQ(result, -1);
    result = list_contains(list, nullptr, nullptr);
    EXPECT_EQ(result, -1);
    result = list_contains(nullptr, nullptr, nullptr);
    EXPECT_EQ(result, -1);

    // if list is empty: 0
    IsEmpty();
    result = list_contains(list, &v1, Comparator);
    EXPECT_EQ(result, 0);

    // if list has one item
    list_append(list, &v1);
    result = list_contains(list, &v2, Comparator); // invalid item: 0 
    EXPECT_EQ(result, 0);
    IsNotEmpty();
    result = list_contains(list, &v1, Comparator); // valid item: 1
    EXPECT_EQ(result, 1);
    IsNotEmpty();
    Clear();
    
    // if has multiple items
    FillList(list_append);
    result = list_contains(list, &v3, Comparator); // invalid item: 0 
    EXPECT_EQ(result, 0);
    IsNotEmpty();
    result = list_contains(list, &v1, Comparator); // if valid single item: 1
    EXPECT_EQ(result, 1);
    IsNotEmpty();

    // if valid multiple item: > 1
    result = list_contains(list, &v2, Comparator); // result: 2
    EXPECT_EQ(result, 2);
    IsNotEmpty();

    list_append(list, &v2);
    result = list_contains(list, &v2, Comparator); // result: 3
    EXPECT_EQ(result, 3);
    IsNotEmpty();

    list_append(list, &v2);
    list_append(list, &v2);
    result = list_contains(list, &v2, Comparator); // result: 5
    EXPECT_EQ(result, 5);
    IsNotEmpty();

    Clear();
}   

// function: list_filter
TEST_F(ListTest, FilterTest){
    list_t *filtered;
    int v1 = 5, v2 = 170, v3 = 0, v4 = 3;
    int filtered_values[5] = {9, 5, 7, 4, 5};

    // if any argument is null: -1
    filtered = list_filter(nullptr, &v1, Predicate);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(list, nullptr, Predicate);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(list, &v1, nullptr);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(nullptr, nullptr, Predicate);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(nullptr, &v1, nullptr);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(list, nullptr, nullptr);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(nullptr, nullptr, nullptr);
    EXPECT_EQ(filtered, nullptr);
    
    // if list is empty
    IsEmpty();
    filtered = list_filter(list, &v1, Predicate);
    ListTest::IsEmpty(filtered);
    list_delete(&filtered);
    EXPECT_EQ(filtered, nullptr);

    // if list has one item
    // invalid context
    list_append(list, &v1);
    filtered = list_filter(list, &v2, Predicate); 
    ListTest::IsEmpty(filtered);
    list_delete(&filtered);
    EXPECT_EQ(filtered, nullptr);

    // valid context
    filtered = list_filter(list, &v3, Predicate); 
    ListTest::IsNotEmpty(filtered);
    EXPECT_EQ(filtered->list_size, 1);
    EXPECT_NE(filtered->head, nullptr);
    EXPECT_EQ(filtered->head, filtered->tail);
    list_delete(&filtered);
    EXPECT_EQ(filtered, nullptr);    
    Clear();

    // it has multiple items
    FillList(list_append);
    // invalid
    filtered = list_filter(list, &v2, Predicate); 
    ListTest::IsEmpty(filtered);
    list_delete(&filtered);
    EXPECT_EQ(filtered, nullptr);
    // valid: gt 3
    filtered = list_filter(list, &v4, Predicate); 
    ListTest::IsNotEmpty(filtered);
    EXPECT_EQ(filtered->list_size, 5);
    for(int i = 0; i < 5; i++)
        EXPECT_EQ(*(int*)list_at(filtered, i), filtered_values[i]); // using list_at function checked before
    list_delete(&filtered);
    EXPECT_EQ(filtered, nullptr);

    Clear();
}

TEST_F(ListTest, TrimFrontTest){
    int result;
    int valid_values[7] = {5, 7, 0, 4, 5, 3, 1};

    // if list is null
    result = list_trim_front(nullptr, 2); // -1
    EXPECT_EQ(result, -1);
    // empty list
    IsEmpty();
    result = list_trim_front(list, 0); // 0
    EXPECT_EQ(result, 0);
    IsEmpty();
    result = list_trim_front(list, 1); // -1
    EXPECT_EQ(result, -1);

    FillList(list_append);
    // if n = 0
    result = list_trim_front(list, 0); // 0
    EXPECT_EQ(result, 0);
    // if n gt list_size
    result = list_trim_front(list, 15); // -1
    EXPECT_EQ(result, -1);

    // valid trim twice 
    result = list_trim_front(list, 3); // 3
    IsNotEmpty();
    EXPECT_EQ(result, 3);
    EXPECT_EQ(list->list_size, 7);
    for(int i = 0; i < 7; i++)
        EXPECT_EQ(*(int*)list_at(list, i), valid_values[i]);
    
    result = list_trim_front(list, 2); // 3
    IsNotEmpty();
    EXPECT_EQ(result, 2);
    EXPECT_EQ(list->list_size, 5);
    for(int i = 2; i < 7; i++)
        EXPECT_EQ(*(int*)list_at(list, i - 2), valid_values[i]);
    
    Clear();
}

TEST_F(ListTest, TrimBackTest){
    int result;
    int valid_values[7] = {9, 2, -1, 5, 7, 0, 4};
    int count;
    node_t* current_node;
    // if list is null
    result = list_trim_back(nullptr, 2); // -1
    EXPECT_EQ(result, -1);
    // empty list
    IsEmpty();
    result = list_trim_back(list, 0); // 0
    EXPECT_EQ(result, 0);
    IsEmpty();
    result = list_trim_back(list, 1); // -1
    EXPECT_EQ(result, -1);

    FillList(list_append);
    // if n = 0
    result = list_trim_back(list, 0); // 0
    EXPECT_EQ(result, 0);
    // if n gt list_size
    result = list_trim_back(list, 15); // -1
    EXPECT_EQ(result, -1);

    // valid trim twice 
    result = list_trim_back(list, 3); // 3
    IsNotEmpty();
    EXPECT_EQ(result, 3);
    EXPECT_EQ(list->list_size, 7);

    current_node = list->head;
    count = 0;
    while(current_node){
        EXPECT_EQ(*(int*)current_node->item, valid_values[count]);
        current_node = current_node->next;
        count++;
    }
    EXPECT_EQ(count, 7);

    result = list_trim_back(list, 2); // 3
    IsNotEmpty();
    EXPECT_EQ(result, 2);
    EXPECT_EQ(list->list_size, 5);
    
    current_node = list->head;
    count = 0;
    while(current_node){
        EXPECT_EQ(*(int*)current_node->item, valid_values[count]);
        current_node = current_node->next;
        count++;
    }
    EXPECT_EQ(count, 5);

    Clear();
}

TEST_F(ListTest, TrimRangeTest){
    int result;

    // if list is null
    result = list_trim_range(nullptr, 2, 5); // -1
    EXPECT_EQ(result, -1);

    // empty list
    IsEmpty();
    result = list_trim_range(list, 0, 0); // 0
    EXPECT_EQ(result, 0);

    IsEmpty();
    result = list_trim_range(list, 5, 1); // -1
    EXPECT_EQ(result, -1);

    FillList(list_append);
    result = list_trim_range(list, 4, 4); // 0
    EXPECT_EQ(result, 0);
    result = list_trim_range(list, 1, 0); // 0
    EXPECT_EQ(result, -1);
    result = list_trim_range(list, 5, 3); // 0
    EXPECT_EQ(result, -1);
    // if n gt list_size
    result = list_trim_range(list, 2, 15); // -1
    EXPECT_EQ(result, -1);

    // remove full list
    FillList(list_append);
    result = list_trim_range(list, 0, list->list_size);
    EXPECT_EQ(result, 10);
    IsEmpty();

    // data = {9, 2, -1, 5, 7, 0, 4, 5, 3, 1}
    std::vector<std::tuple<int, int, int, int, std::vector<int>>> test_input = {  
        {0, 1, 1, 9, {2, -1, 5, 7, 0, 4, 5, 3, 1}},     // trime range (0, 1) - only one item
        {0, 5, 5, 5, {0, 4, 5, 3, 1}},                  // trim range (0, end) - multiple items
        {0, 9, 9, 1, {1}},                              // trim all items
        {6, 7, 1, 9, {9, 2, -1, 5, 7, 0, 5, 3, 1}},     // trim only one item in the middle
        {3, 4, 1, 9, {9, 2, -1, 7, 0, 4, 5, 3, 1}},     // trim only one item in the middle
        {2, 6, 4, 6, {9, 2, 4, 5, 3, 1}},               // trim multiple in the middle
        {5, 8, 3, 7, {9, 2, -1, 5, 7, 3, 1}},           // trim multiple in the middle
        {9, 10, 1, 9, {9, 2, -1, 5, 7, 0, 4, 5, 3}},    // trim only last item
        {5, 10, 5, 5, {9, 2, -1, 5, 7}},                // trim multiple last items
        {3, 10, 7, 3, {9, 2, -1}}                       // trim multiple last items
    };
    
    for(int i = 0; i < test_input.size(); i++) {
        FillList(list_append);
        result = list_trim_range(list, std::get<0>(test_input[i]), std::get<1>(test_input[i]));
        IsNotEmpty();
        EXPECT_EQ(result, std::get<2>(test_input[i]));
        EXPECT_EQ(list->list_size, std::get<3>(test_input[i]));
        TestData(std::get<4>(test_input[i]));
    }

    Clear();
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}