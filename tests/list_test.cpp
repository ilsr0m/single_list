#include "list_append.hpp"
#include "list_prepend.hpp"
#include "list_insert.hpp"
#include "list_remove.hpp"
#include "list_sort.hpp"

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}