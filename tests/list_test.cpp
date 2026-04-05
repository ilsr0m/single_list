#include "list_append.hpp"
#include "list_prepend.hpp"
#include "list_insert.hpp"
#include "list_sort.hpp"

class ListTest : public testing::Test {
protected:
    list_t *lst;
    std::vector<int> test_data = {9, 2, -1, 5, 7, 0, 4, 5, 3, 1};

    void SetUp() { 
        lst = list_create(sizeof(int)); 
        IsEmpty();
    }

    void TearDown() { 
        list_delete(&lst);
        EXPECT_EQ(lst, nullptr);
        EXPECT_EQ(list_empty(lst), -1);
    }

    void IsEmpty() {
        EXPECT_NE(lst, nullptr); // list is not null, coz it's already initialized
        EXPECT_EQ(list_size(lst), 0); // no any items
        EXPECT_EQ(list_head(lst), nullptr); // obviously null
        EXPECT_EQ(list_tail(lst), nullptr); // obviously null
        EXPECT_EQ(list_item_size(lst), sizeof(int));
        EXPECT_EQ(list_empty(lst), 1);
    }

    static void IsEmpty(list_t *li) {
        EXPECT_NE(li, nullptr); // list is not null, coz it's already initialized
        EXPECT_EQ(list_size(li), 0); // no any items
        EXPECT_EQ(list_head(li), nullptr); // obviously null
        EXPECT_EQ(list_tail(li), nullptr); // obviously null
        EXPECT_EQ(list_item_size(li), sizeof(int));
        EXPECT_EQ(list_empty(li), 1);
    }

    void IsNotEmpty() {
        EXPECT_NE(lst, nullptr); // list is not null, coz it's already initialized
        EXPECT_NE(list_head(lst), nullptr); 
        EXPECT_NE(list_tail(lst), nullptr); 
        EXPECT_GT(list_size(lst), 0); 
        EXPECT_EQ(list_item_size(lst), sizeof(int));
        EXPECT_EQ(list_empty(lst), 0);
    }

    static void IsNotEmpty(list_t *li) {
        EXPECT_NE(li, nullptr); // list is not null, coz it's already initialized
        EXPECT_NE(list_head(li), nullptr); 
        EXPECT_NE(list_tail(li), nullptr); 
        EXPECT_GT(list_size(li), 0); 
        EXPECT_EQ(list_item_size(li), sizeof(int));
        EXPECT_EQ(list_empty(li), 0);
    }

    void Clear() {
        list_clear(lst);
        IsEmpty();
    }

    void FillList(int (*add)(list_t* list, const void* data)) {
        Clear();
        int result;
        for(unsigned i = 0; i < 10; i++){
            result = add(lst, &test_data[i]);
            EXPECT_EQ(result, 0);
        }
        IsNotEmpty();
        EXPECT_EQ(list_size(lst), 10);
    }

    void FillRandomList(list_t *lst, std::vector<int> values){
        if(!lst) return;
        if(!list_empty(lst)) list_clear(lst);
        for(unsigned i = 0; i < values.size(); i++)
            list_append(lst, &values[i]);
    }

    int TestData(std::vector<int> test_values) {
        node_t* current_node = list_head(lst);
        int count = 0;
        while(current_node){
            EXPECT_EQ(*(int*)node_data(current_node), test_values[count]);
            current_node = node_next(current_node);
            count++;
        }
        return count;
    }

    // this comparator is needed as arg in list_remove and list_remove_all
    static int Comparator(const void* _item, const void* _key) {
        return (*(int*)_item == *(int*)_key) ? 0 : -1;
    }

    static int SortComparator(const void* _item, const void* _key) {
        return (*(int*)_item > *(int*)_key) ? 0 : -1;
    }

    static int Predicate(const void* _item, void* _context) {
        return (*(int*)_item > *(int*)_context) ? 0 : 1;
    }
};

// function: list_front
TEST_F(ListTest, FrontTest) {
    void* result; // return value of funtion 
    int v1 = 3, v2 = 0;

    // nullptr
    result = list_front_item(nullptr);
    EXPECT_EQ(result, nullptr);
    
    // empty list
    result = list_front_item(lst);
    EXPECT_EQ(result, nullptr);

    list_prepend(lst, &v1);
    result = list_front_item(lst);
    EXPECT_EQ(*(int*)result, v1);
    IsNotEmpty();

    list_prepend(lst, &v2);
    result = list_front_item(lst);
    EXPECT_EQ(*(int*)result, v2);
    IsNotEmpty();

    Clear();
}

// function: list_back
TEST_F(ListTest, BackTest){
    void* result; // return value of funtion 
    int v1 = 3, v2 = 0;

    result = list_back_item(lst);
    EXPECT_EQ(result, nullptr);

    list_append(lst, &v1);
    result = list_back_item(lst);
    EXPECT_EQ(*(int*)result, v1);

    list_append(lst, &v2);
    result = list_back_item(lst);
    EXPECT_EQ(*(int*)result, v2);

    Clear();
}

// function: list_remove
TEST_F(ListTest, RemoveOnceTest) {
    int once = 4;
    
    EXPECT_EQ(list_remove(nullptr, &once, ListTest::Comparator), -1); // null
    EXPECT_EQ(list_remove(lst, &once, ListTest::Comparator), 0);
    EXPECT_EQ(list_remove(lst, &once, nullptr), -1);
    IsEmpty();

    // Test 0 - only one item in list
    list_append(lst, &once);
    EXPECT_EQ( list_remove(lst, &once, ListTest::Comparator), 1);
    IsEmpty();
    
    // key, size, values, result
    std::vector<std::tuple<int, int, std::vector<int>, int>> test = {
        { 9  , 9, { 2, -1, 5, 7, 0, 4, 5, 3, 1 }, 1 }, // Test 1 - Remove head item
        { 1  , 8, { 2, -1, 5, 7, 0, 4, 5, 3 }, 1    }, // Test 2 - Remove tail item
        { 0  , 7, { 2, -1, 5, 7, 4, 5, 3 }, 1       }, // Test 3 - Remove middle item
        { 5  , 6, { 2, -1, 7, 4, 5, 3 }, 1          }, // Test 4 - Remove value 5 (it must remove only first item with such value)
        { 228, 6, { 2, -1, 7, 4, 5, 3 }, 0         }  // Test 5 - Try to remove value which the list does not have
    };

    FillList(list_append);
    for(unsigned i = 0; i < test.size(); i++){
        int result = list_remove(lst, &std::get<0>(test[i]), ListTest::Comparator);
        EXPECT_NE(lst, nullptr);

        EXPECT_EQ(result, std::get<3>(test[i]));

        EXPECT_EQ(list_size(lst), std::get<1>(test[i]));
        EXPECT_EQ(list_size(lst), std::get<2>(test[i]).size());

        node_t *test_iter = list_head(lst);
        int test_ints_count = 0;
        while(test_iter){
            EXPECT_EQ(*(int*)node_data(test_iter), std::get<2>(test[i])[test_ints_count]);
            test_iter = node_next(test_iter);
            test_ints_count++;
        }
        EXPECT_EQ(test_ints_count, std::get<2>(test[i]).size());
    }

    Clear();
}

TEST_F(ListTest, RemoveAtTest){
    EXPECT_EQ(list_remove_at(nullptr, 0), -1); // null
    EXPECT_EQ(list_remove_at(lst, 0), -1);
    IsEmpty();

    //std::vector<int> test_data = {9, 2, -1, 5, 7, 0, 4, 5, 3, 1};
    // index, removed, size, ethalon
    std::vector<std::tuple<int, int, int, std::vector<int>>> test = {
        { 0  , 1, 9, { 2, -1, 5, 7, 0, 4, 5, 3, 1 } }, // Test 1 - Remove head item
        { 8  , 1, 8, { 2, -1, 5, 7, 0, 4, 5, 3 }    }, // Test 2 - Remove tail item
        { 3  , 1, 7, { 2, -1, 5, 0, 4, 5, 3 }       }, // Test 3 - Remove middle item
        { 5  , 1, 6, { 2, -1, 5, 0, 4, 3 }          }, // Test 4 - Remove middle item
        { 228, -1, 6, { 2, -1, 5, 0, 4, 3 }          }  // Test 5 - Invalid index
    };

    FillList(list_append);
    for(unsigned i = 0; i < test.size(); i++){
        int result = list_remove_at(lst, std::get<0>(test[i]));
        
        IsNotEmpty(lst);

        EXPECT_EQ(list_size(lst), std::get<2>(test[i]));
        EXPECT_EQ(result, std::get<1>(test[i]));
        EXPECT_EQ(list_size(lst), std::get<3>(test[i]).size());

        node_t *test_iter = list_head(lst);
        int test_ints_count = 0;
        while(test_iter){
            EXPECT_EQ(*(int*)node_data(test_iter), std::get<3>(test[i])[test_ints_count]);
            test_iter = node_next(test_iter);
            test_ints_count++;
        }

        EXPECT_EQ(test_ints_count, std::get<3>(test[i]).size());
    }
    Clear();
}

// function: list_remove_all
TEST_F(ListTest, RemoveAllTest){
    // Test bundle
    int removed_items_count; // for return type
    int test_ints_count;
    node_t* test_iter;
    std::vector<int> test_values = {2, -1, 5, 7, 0, 4, 5, 3, 1};

    // Test 0 - only one item in list
    int once = 4;
    list_append(lst, &once);
    removed_items_count = list_remove_all(lst, &once, ListTest::Comparator);
    EXPECT_EQ(removed_items_count, 1);
    IsEmpty();

    // fill list with some values
    FillList(list_append);
    


    // Test 1 - Remove head item
    int head_key = 9; // remove 9 value
    removed_items_count = list_remove_all(lst, &head_key, ListTest::Comparator);
    EXPECT_NE(lst, nullptr);
    EXPECT_EQ(removed_items_count, 1);
    EXPECT_EQ(list_size(lst), 9); 
    // check each item value
    test_iter = list_head(lst);
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)node_data(test_iter), test_values[test_ints_count]);
        test_iter = node_next(test_iter);
        test_ints_count++;
    }

    // Test 2 - Remove tail item
    test_values = {2, -1, 5, 7, 0, 4, 5, 3};
    int tail_key = 1; // remove 1 value
    removed_items_count = list_remove_all(lst, &tail_key, ListTest::Comparator);
    EXPECT_NE(lst, nullptr);
    EXPECT_EQ(removed_items_count, 1);
    EXPECT_EQ(list_size(lst), 8); 
    // check each item value
    test_iter = list_head(lst);
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)node_data(test_iter), test_values[test_ints_count]);
        test_iter = node_next(test_iter);
        test_ints_count++;
    }

    // Test 3 - Remove middle item
    test_values = {2, -1, 5, 7, 4, 5, 3};
    int middle_key = 0; // remove 0 value
    removed_items_count = list_remove_all(lst, &middle_key, ListTest::Comparator);
    EXPECT_NE(lst, nullptr);
    EXPECT_EQ(removed_items_count, 1);
    EXPECT_EQ(list_size(lst), 7); 
    // check each item value
    test_iter = list_head(lst);
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)node_data(test_iter), test_values[test_ints_count]);
        test_iter = node_next(test_iter);
        test_ints_count++;
    }

    // Test 4 - Remove value 5 (it must remove all item with such value)
    int duplicate_key = 5; // remove all fives
    // added the same value more
    list_append(lst, &duplicate_key);
    list_append(lst, &duplicate_key);
    list_append(lst, &duplicate_key);
    list_prepend(lst, &duplicate_key);
    list_prepend(lst, &duplicate_key);
    list_insert(lst, &duplicate_key, 4);

    // start removing all fives
    removed_items_count = list_remove_all(lst, &duplicate_key, ListTest::Comparator);
    EXPECT_NE(lst, nullptr);
    EXPECT_EQ(removed_items_count, 8);
    EXPECT_EQ(list_size(lst), 5); 
    // check each item value
    test_values = {2, -1, 7, 4, 3};
    test_iter = list_head(lst);
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)node_data(test_iter), test_values[test_ints_count]);
        test_iter = node_next(test_iter);
        test_ints_count++;
    }

    // Test 5 - Try to remove value which the list does not have
    int fake_key = 228;
    removed_items_count = list_remove_all(lst, &fake_key, ListTest::Comparator);
    EXPECT_NE(lst, nullptr);
    EXPECT_EQ(removed_items_count, 0);
    EXPECT_EQ(list_size(lst), 5); 
    // check each item value
    test_iter = list_head(lst);
    test_ints_count = 0;
    while(test_iter){
        EXPECT_EQ(*(int*)node_data(test_iter), test_values[test_ints_count]);
        test_iter = node_next(test_iter);
        test_ints_count++;
    }

    Clear();
}

// function: list_pop_front
TEST_F(ListTest, PopFrontTest){
    int val = 22;
    void* item;

    //empty
    item = list_pop_front(lst);
    EXPECT_EQ(item, nullptr);

    // Remove when it has only one item
    list_prepend(lst, &val);
    item = list_pop_front(lst);
    IsEmpty();
    EXPECT_EQ(*(int*)item, val);

    // Remove when several items
    FillList(list_append);
    item = list_pop_front(lst);
    EXPECT_EQ(*(int*)item, 9);
    IsNotEmpty();
    EXPECT_EQ(list_size(lst), 9);

    Clear();
}

// function: list_pop_back
TEST_F(ListTest, PopBackTest) {
    int val = 22;
    void* item;

    // empty
    item = list_pop_front(lst);
    EXPECT_EQ(item, nullptr);

    // Pop when it has only one item    
    list_append(lst, &val);
    item = list_pop_back(lst);
    IsEmpty();
    EXPECT_EQ(*(int*)item, val);

    // Pop when several items
    FillList(list_append);

    item = list_pop_back(lst);
    EXPECT_EQ(*(int*)item, 1);
    IsNotEmpty();
    EXPECT_EQ(list_size(lst), 9);

    Clear();
}

// function: list_at
TEST_F(ListTest, AtTest) {
    void* item;

    // empty list
    item = list_at(lst, 0);
    EXPECT_EQ(item, nullptr);
    IsEmpty();

    // prepare values
    FillList(list_append);
    // try to get value by valid index
    // scan all values
    for(int i = 0; i < 10; i++) {
        item = list_at(lst, i);
        EXPECT_NE(item, nullptr);
        EXPECT_EQ(*(int*)item, test_data[i]);
        IsNotEmpty();
    }

    // try to get value by invalid index
    // scan 10 invalid indexes
    for(int i = 10; i < 20; i++){
        item = list_at(lst, i);
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
    copy = list_copy(lst);
    ListTest::IsEmpty(copy);
    EXPECT_NE(copy, lst);

    list_delete(&copy);
    EXPECT_EQ(copy, nullptr);

    // copy of one item list
    int val = 3; 
    list_append(lst, &val);
    copy = list_copy(lst);

    ListTest::IsNotEmpty(copy);
    EXPECT_NE(copy, lst);
    EXPECT_EQ(list_size(copy), 1);
    EXPECT_NE(list_head(copy), nullptr);
    EXPECT_NE(list_tail(copy), nullptr);
    EXPECT_EQ(list_tail(copy), list_head(copy));

    list_delete(&copy);
    EXPECT_EQ(copy, nullptr);

    // copy of filled list - 10 items
    Clear();
    FillList(list_append);
    copy = list_copy(lst);
    ListTest::IsNotEmpty();
    EXPECT_EQ(list_size(copy), 10);
    node_t* copy_iter = list_head(copy);
    node_t* list_iter = list_head(copy);
    while (copy_iter && list_iter){
        EXPECT_NE(node_data(copy_iter), nullptr);
        EXPECT_EQ(*(int*)node_data(copy_iter), *(int*)node_data(list_iter));
        copy_iter = node_next(copy_iter);
        list_iter = node_next(list_iter);
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
    result = list_contains(lst, nullptr, Comparator);
    EXPECT_EQ(result, -1);
    result = list_contains(lst, &v1, nullptr);
    EXPECT_EQ(result, -1);
    result = list_contains(nullptr, nullptr, Comparator);
    EXPECT_EQ(result, -1);
    result = list_contains(nullptr, &v1, nullptr);
    EXPECT_EQ(result, -1);
    result = list_contains(lst, nullptr, nullptr);
    EXPECT_EQ(result, -1);
    result = list_contains(nullptr, nullptr, nullptr);
    EXPECT_EQ(result, -1);

    // if list is empty: 0
    IsEmpty();
    result = list_contains(lst, &v1, Comparator);
    EXPECT_EQ(result, 0);

    // if list has one item
    list_append(lst, &v1);
    result = list_contains(lst, &v2, Comparator); // invalid item: 0 
    EXPECT_EQ(result, 0);
    IsNotEmpty();
    result = list_contains(lst, &v1, Comparator); // valid item: 1
    EXPECT_EQ(result, 1);
    IsNotEmpty();
    Clear();
    
    // if has multiple items
    FillList(list_append);
    result = list_contains(lst, &v3, Comparator); // invalid item: 0 
    EXPECT_EQ(result, 0);
    IsNotEmpty();
    result = list_contains(lst, &v1, Comparator); // if valid single item: 1
    EXPECT_EQ(result, 1);
    IsNotEmpty();

    // if valid multiple item: > 1
    result = list_contains(lst, &v2, Comparator); // result: 1
    EXPECT_EQ(result, 1);
    IsNotEmpty();

    list_append(lst, &v2);
    result = list_contains(lst, &v2, Comparator); // result: 1
    EXPECT_EQ(result, 1);
    IsNotEmpty();

    list_append(lst, &v2);
    list_append(lst, &v2);
    result = list_contains(lst, &v2, Comparator); // result: 1
    EXPECT_EQ(result, 1);
    IsNotEmpty();

    Clear();
}   

// function: list_filter
TEST_F(ListTest, FilterTest){
    list_t *filtered;
    int v1 = 5, v2 = 170, v3 = 0, v4 = 3;
    std::vector<int> filtered_values = {9, 5, 7, 4, 5};

    // if any argument is null: -1
    filtered = list_filter(nullptr, Predicate, &v1);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(lst, Predicate, nullptr);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(lst, nullptr, &v1);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(nullptr, Predicate, nullptr);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(nullptr, nullptr, &v1);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(lst, nullptr, nullptr);
    EXPECT_EQ(filtered, nullptr);
    filtered = list_filter(nullptr, nullptr, nullptr);
    EXPECT_EQ(filtered, nullptr);
    
    // if list is empty
    IsEmpty();
    filtered = list_filter(lst, Predicate, &v1);
    ListTest::IsEmpty(filtered);
    list_delete(&filtered);

    // // if list has one item
    // // invalid context
    list_append(lst, &v1);
    filtered = list_filter(lst, Predicate, &v2); 
    ListTest::IsEmpty(filtered);
    list_delete(&filtered);

    // // valid context
    filtered = list_filter(lst, Predicate, &v2); 
    EXPECT_EQ(list_size(filtered), 0);
    EXPECT_EQ(list_head(filtered), nullptr);
    EXPECT_EQ(list_head(filtered), list_tail(filtered));
    list_delete(&filtered);  
    Clear();

    // it has multiple items
    FillList(list_append);
    // invalid
    filtered = list_filter(lst, Predicate, &v2); 
    ListTest::IsEmpty(filtered);
    list_delete(&filtered);
    EXPECT_EQ(filtered, nullptr);
    // valid: gt 3
    filtered = list_filter(lst, Predicate, &v4); 
    ListTest::IsNotEmpty(filtered);
    EXPECT_EQ(list_size(filtered), 5);
    for(int i = 0; i < 5; i++)
        EXPECT_EQ(*(int*)list_at(filtered, i), filtered_values[i]); // using list_at function checked before
    list_delete(&filtered);
    EXPECT_EQ(filtered, nullptr);

    Clear();
}

TEST_F(ListTest, TrimFrontTest){
    int result;
    std::vector<int> test_values = {5, 7, 0, 4, 5, 3, 1};

    // if list is null
    result = list_trim_front(nullptr, 2); // -1
    EXPECT_EQ(result, -1);
    // empty list
    IsEmpty();
    result = list_trim_front(lst, 0); // 0
    EXPECT_EQ(result, 0);
    IsEmpty();
    result = list_trim_front(lst, 1); // -1
    EXPECT_EQ(result, -1);

    FillList(list_append);
    // if n = 0
    result = list_trim_front(lst, 0); // 0
    EXPECT_EQ(result, 0);
    // if n gt list_size
    result = list_trim_front(lst, 15); // -1
    EXPECT_EQ(result, -1);
    
    // valid trim twice 
    result = list_trim_front(lst, 3); // 3
    IsNotEmpty();
    EXPECT_EQ(result, 3);
    EXPECT_EQ(list_size(lst), 7);
    for(int i = 0; i < 7; i++)
        EXPECT_EQ(*(int*)list_at(lst, i), test_values[i]);
    
    result = list_trim_front(lst, 2); // 3
    IsNotEmpty();
    EXPECT_EQ(result, 2);
    EXPECT_EQ(list_size(lst), 5);
    for(int i = 2; i < 7; i++)
        EXPECT_EQ(*(int*)list_at(lst, i - 2), test_values[i]);
    
    Clear();
}

TEST_F(ListTest, TrimBackTest){
    int result;
    // if list is null
    result = list_trim_back(nullptr, 2); // -1
    EXPECT_EQ(result, -1);
    // empty list
    IsEmpty();
    result = list_trim_back(lst, 0); // 0
    EXPECT_EQ(result, 0);
    IsEmpty();
    result = list_trim_back(lst, 1); // -1
    EXPECT_EQ(result, -1);

    FillList(list_append);
    // if n = 0
    result = list_trim_back(lst, 0); // 0
    EXPECT_EQ(result, 0);
    // if n gt list_size
    result = list_trim_back(lst, 15); // -1
    EXPECT_EQ(result, -1);

    // data = {9, 2, -1, 5, 7, 0, 4, 5, 3, 1}
    std::vector<std::tuple<int, int, int, std::vector<int>, int>> test_input = {  
        {3, 3, 7, {9, 2, -1, 5, 7, 0, 4}, 7},       // valid trim twice 
        {2, 2, 5, {9, 2, -1, 5, 7}, 5}              // valid trim twice 
    };
    
    for(int i = 0; i < test_input.size(); i++) {
        result = list_trim_back(lst, std::get<0>(test_input[i])); // 3
        IsNotEmpty();
        EXPECT_EQ(result, std::get<1>(test_input[i]));
        EXPECT_EQ(list_size(lst), std::get<2>(test_input[i]));
        EXPECT_EQ(TestData(std::get<3>(test_input[i])), std::get<4>(test_input[i]));
    }

    Clear();
}

TEST_F(ListTest, TrimRangeTest){
    int result;

    // if list is null
    result = list_trim_range(nullptr, 2, 5); // -1
    EXPECT_EQ(result, -1);

    // empty list
    IsEmpty();
    result = list_trim_range(lst, 0, 0); // 0
    EXPECT_EQ(result, 0);

    IsEmpty();
    result = list_trim_range(lst, 5, 1); // -1
    EXPECT_EQ(result, -1);

    FillList(list_append);
    result = list_trim_range(lst, 4, 4); // 0
    EXPECT_EQ(result, 0);
    result = list_trim_range(lst, 1, 0); // 0
    EXPECT_EQ(result, -1);
    result = list_trim_range(lst, 5, 3); // 0
    EXPECT_EQ(result, -1);
    // if n gt list_size
    result = list_trim_range(lst, 2, 15); // -1
    EXPECT_EQ(result, -1);

    // remove full list
    FillList(list_append);
    result = list_trim_range(lst, 0, list_size(lst));
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
        result = list_trim_range(lst, std::get<0>(test_input[i]), std::get<1>(test_input[i]));
        IsNotEmpty();
        EXPECT_EQ(result, std::get<2>(test_input[i]));
        EXPECT_EQ(list_size(lst), std::get<3>(test_input[i]));
        TestData(std::get<4>(test_input[i]));
    }

    Clear();
}

TEST_F(ListTest, SetTest){
    // nullptr list
    int val = 1;

    // check nullptr and empty
    std::vector<std::tuple<list_t*, int, void*>> nullTest = {
        { nullptr, 2   , &val    }, 
        { lst    , 2   , nullptr },
        { nullptr, 2   , nullptr },
        { nullptr, 0   , &val    }, 
        { lst    , 0   , nullptr },
        { nullptr, 0   , nullptr },
        { nullptr, 1000, &val    }, 
        { lst    , 1000, &val    },
        { nullptr, 1000, &val    },
        { lst    , 0   , &val    }, 
        { lst    , 5   , &val    },
        { lst    , 1   , &val    },
    };

    for(unsigned i = 0; i < nullTest.size(); i++) {
        int setResult = list_set(std::get<0>(nullTest[i]), std::get<1>(nullTest[i]), std::get<2>(nullTest[i]));
        EXPECT_EQ(setResult, -1);
        IsEmpty();
    }

    // check valid index
    std::vector<std::tuple<int, int, std::vector<int>>> test = {
        { 0 , 1   , { 1, 2, -1, 5, 7, 0, 4   , 5  , 3  , 1 } }, 
        { 1 , 2   , { 9, 2, -1, 5, 7, 0, 4   , 5  , 3  , 1 } },
        { 2 , 3   , { 9, 2, 3 , 5, 7, 0, 4   , 5  , 3  , 1 } },
        { 3 , 4   , { 9, 2, -1, 4, 7, 0, 4   , 5  , 3  , 1 } }, 
        { 4 , 5   , { 9, 2, -1, 5, 5, 0, 4   , 5  , 3  , 1 } },
        { 5 , 6   , { 9, 2, -1, 5, 7, 6, 4   , 5  , 3  , 1 } },
        { 6 , 2222, { 9, 2, -1, 5, 7, 0, 2222, 5  , 3  , 1 } }, 
        { 7 , 111 , { 9, 2, -1, 5, 7, 0, 4   , 111, 3  , 1 } },
        { 8 , -55 , { 9, 2, -1, 5, 7, 0, 4   , 5  , -55, 1 } },
        { 9 , 0   , { 9, 2, -1, 5, 7, 0, 4   , 5  , 3  , 0 } },
        { 22, 10  , { 9, 2, -1, 5, 7, 0, 4   , 5  , 3  , 1 } }, // invalid index
        { 17, 11  , { 9, 2, -1, 5, 7, 0, 4   , 5  , 3  , 1 } }  // invalid index
    };

    for(unsigned i = 0; i < test.size(); i++){
        FillList(list_append);
        
        int setReslut = list_set(lst, std::get<0>(test[i]), &std::get<1>(test[i]));
        
        IsNotEmpty();
        EXPECT_EQ(list_size(lst), 10);
        
        if(setReslut != -1)
            EXPECT_EQ(setReslut, 0);
        else EXPECT_EQ(setReslut, -1);

        // check list with ethalon
        node_t *current_item = list_head(lst);
        int count = 0;
        while (current_item) {
            EXPECT_EQ(*(int*)node_data(current_item), std::get<2>(test[i])[count]);
            current_item = node_next(current_item);
            count++;
        }
        Clear();
    }
}

TEST_F(ListTest, ConcatTest) {
    // data = {9, 2, -1, 5, 7, 0, 4, 5, 3, 1}
    FillList(list_append);

    EXPECT_EQ(list_concat(nullptr, nullptr), -1);
    EXPECT_EQ(list_concat(lst    , nullptr), -1);
    EXPECT_EQ(list_concat(nullptr, lst    ), -1);

    // | int    | std::vector<int>  | std::vector<int>  | std::vector<int>      |
    // | return | destination       | source            | concat destination    |
    std::vector<std::tuple<int, std::vector<int>, std::vector<int>, std::vector<int>>> test = {
        { 0, {1, 2, 3}, {1, 2, 3}   , {1, 2, 3, 1, 2, 3}    },
        { 0, {1}      , {1, 2, 3}   , {1, 1, 2, 3}          },
        { 0, {1, 2, 3}, {1}         , {1, 2, 3, 1}          },
        { 0, {1111}   , {1222}      , {1111, 1222}          },
        { 0, {3, 2, 1}, {1, 2, 3, 9}, {3, 2, 1, 1, 2, 3, 9} },
        { 0, {}       , {1, 2, 3}   , {1, 2, 3}             },
        { 0, {1, 2, 3}, {}          , {1, 2, 3}             }
    };

    auto lst_source = list_create(sizeof(int));
    for(unsigned i = 0; i < test.size(); i++) {
        FillRandomList(lst       , std::get<1>(test[i])); // lst as destination
        FillRandomList(lst_source, std::get<2>(test[i])); // fill source with values

        int result = list_concat(lst, lst_source);
        EXPECT_EQ(result, std::get<0>(test[i]));
        IsNotEmpty();

        int counter = 0;
        node_t *current_node = list_head(lst);
        EXPECT_EQ(list_size(lst), std::get<3>(test[i]).size());

        while(current_node) {
            EXPECT_EQ(*(int*)node_data(current_node), std::get<3>(test[i])[counter]);
            counter++;
            current_node = node_next(current_node);
        }
        EXPECT_EQ(counter, std::get<3>(test[i]).size());
    }

    list_delete(&lst_source);
    Clear();
}

TEST_F(ListTest, SpliceTest){
    // data = {9, 2, -1, 5, 7, 0, 4, 5, 3, 1}
    FillList(list_append);

    EXPECT_EQ(list_splice(nullptr, nullptr), -1);
    EXPECT_EQ(list_splice(lst    , nullptr), -1);
    EXPECT_EQ(list_splice(nullptr, lst    ), -1);
    EXPECT_EQ(list_splice(lst    , lst    ), -1);

    list_t *src = list_create(sizeof(double));
    int n = 1;
    list_append(src, &n);
    list_append(src, &n);
    list_append(src, &n);
    EXPECT_EQ(list_splice(lst    , src    ), -1);
    list_delete(&src);

    // | int    | std::vector<int>  | std::vector<int>  | std::vector<int>      |
    // | return | destination       | source            | concat destination    |
    std::vector<std::tuple<int, std::vector<int>, std::vector<int>, std::vector<int>>> test = {
        { 0, {1, 2, 3}, {1, 2, 3}   , {1, 2, 3, 1, 2, 3}    },
        { 0, {1}      , {1, 2, 3}   , {1, 1, 2, 3}          },
        { 0, {1, 2, 3}, {1}         , {1, 2, 3, 1}          },
        { 0, {1111}   , {1222}      , {1111, 1222}          },
        { 0, {3, 2, 1}, {1, 2, 3, 9}, {3, 2, 1, 1, 2, 3, 9} },
        { 0, {}       , {1, 2, 3}   , {1, 2, 3}             },
        { 0, {1, 2, 3}, {}          , {1, 2, 3}             }
    };

    auto lst_source = list_create(sizeof(int));
    for(unsigned i = 0; i < test.size(); i++) {
        FillRandomList(lst       , std::get<1>(test[i])); // lst as destination
        FillRandomList(lst_source, std::get<2>(test[i])); // fill source with values

        int result = list_splice(lst, lst_source);
        EXPECT_EQ(list_empty(lst_source), 1);

        EXPECT_EQ(result, std::get<0>(test[i]));
        IsNotEmpty();

        int counter = 0;
        node_t *current_node = list_head(lst);
        EXPECT_EQ(list_size(lst), std::get<3>(test[i]).size());

        while(current_node) {
            EXPECT_EQ(*(int*)node_data(current_node), std::get<3>(test[i])[counter]);
            counter++;
            current_node = node_next(current_node);
        }
        EXPECT_EQ(counter, std::get<3>(test[i]).size());
    }

    list_delete(&lst_source); 

    Clear();
}


int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}