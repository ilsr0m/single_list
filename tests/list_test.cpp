#include "slist_append.hpp"
#include "slist_prepend.hpp"
#include "slist_insert.hpp"
#include "slist_set.hpp"
#include "slist_at.hpp"
#include "slist_remove.hpp"
#include "slist_remove_at.hpp"
#include "slist_pop_front.hpp"
#include "slist_sort.hpp"
#include "slist_pop_back.hpp"
#include "slist_remove_all.hpp"

#include "slist_trim_front.hpp"
#include "slist_trim_back.hpp"

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}