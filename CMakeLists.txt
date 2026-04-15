cmake_minimum_required(VERSION 3.10.0)

project(single_list VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 17)

add_library(single_list STATIC              
    src/single_list.c
)

target_include_directories(single_list
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include 
)

enable_testing() # CTest support
find_package(GTest REQUIRED) # look for gtest (and gtest_main)

add_executable(slist_test
    tests/test_utils.cpp
    tests/slist_clear.cpp
    tests/slist_append.cpp
    tests/slist_prepend.cpp
    tests/slist_insert.cpp
    tests/slist_at.cpp
    tests/slist_set.cpp
    tests/slist_remove.cpp
    tests/slist_remove_all.cpp
    tests/slist_remove_at.cpp
    tests/slist_trim_front.cpp 
    tests/slist_trim_back.cpp
    tests/slist_trim_range.cpp 
    tests/slist_copy.cpp    
    tests/slist_splice.cpp
    tests/slist_concat.cpp  
    tests/slist_contains.cpp
    tests/slist_count.cpp
    tests/slist_find.cpp
    tests/slist_filter.cpp
    tests/slist_sort.cpp
    tests/slist_bogosort.cpp
    tests/slist_front_item.cpp
    tests/slist_back_item.cpp
    tests/slist_test.cpp
)

# Директории с заголовками, которые нужны только тестам
target_include_directories(slist_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/utils
)

# Линковка
target_link_libraries(slist_test
    PRIVATE
        single_list  
        GTest::gtest            
        GTest::gtest_main      
)

include(GoogleTest)               # модуль CMake, поставляемый вместе с CMake
gtest_discover_tests(slist_test)  # добавляет тесты в CTest