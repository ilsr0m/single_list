#ifndef LIST_C_H
#define LIST_C_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

typedef struct SingleNode snode_t;
typedef struct SingleList slist_t;

/**
* @brief Comparator function.
*
* Compares element with key.
*
* @param[in] item Pointer to element.
* @param[in] key Pointer to key value.
*
* @return Positive value if item < key.
* @return Zero value if both are equal.
* @return Negative value if item < key.
*/
typedef  int (*comparator_fn)(const void *item, const void *key);

/**
* @brief Predicate function with context.
*
* Evaulates condition for element using user-defined context.
*
* @param[in] item Pointer to element.
* @param[in] context Pointer to user-defined context.
*
* @return Non-zero if condition is true.
* @return Zero otherwise.
*/
typedef int (*predicate_fn)(const void* item, void* context);

/**
* @brief Creates list.
*
* Allocates and initializes new singly-linked list container.
*
* @param[in] item_size Size of single stored element in bytes.
*
* @return Pointer to created list on success.
* @return NULL on failure.
*/
slist_t* slist_create(const size_t item_size);

/**
* @brief Removes all elements from list.
*
* Frees all nodes and stored items.
* List structure remains valid and can be reused.
*
* @param[in,out] lst Pointer to list.
*/
void slist_clear(slist_t *lst);

/**
* @brief Destroys list and resets pointer.
*
* Frees all nodes, stored items and list structure,
* then sets pointer to NULL.
*
* @param[in,out] lst Pointer to list pointer.
*/
void slist_delete(slist_t **lst);

/**
 * @brief Checks if list is empty
 * 
 * @param [in] lst Pointer to list.
 * 
 * @return 1 if list is empty.
 * @return 0 otherwise.
 * @return -1 on error.
 */
int slist_empty(const slist_t *lst);

/**
* @brief Appends element to end of list.
*
* @param[in,out] lst Pointer to list.
* @param[in] item Pointer to element to append.
*
* @return 0 on success. 
* @return -1 on failure.
*/
int slist_append(slist_t *lst, const void *item);

/**
* @brief Prepends element to beginning of list.
*
* @param[in,out] lst Pointer to list.
* @param[in] item Pointer to element to prepend.
*
* @return 0 on success. 
* @return -1 on failure.
*/
int slist_prepend(slist_t *lst, const void *item);

/**
* @brief Inserts element at specified position.
*
* Inserts element before node at position.
* If position equals current list size, element is appended.
*
* @param[in,out] lst Pointer to list.
* @param[in] item Pointer to element to insert.
* @param[in] position Zero-based insertion position.
*
* @return 0 on success. 
* @return -1 on failure.
*/
int slist_insert(slist_t *lst, const void *item, const size_t position);

/**
 * @brief Sets elements at index.
 * 
 * Replaces element at specified position with new value.
 * 
 * @param lst Pointer to list.
 * @param index Zero-based index.
 * @param item Pointer to new value.
 * 
 * @return 0 on success.
 * @return -1 on failure.
 */
int slist_set(slist_t *lst, const size_t index, const void* item);

/**
* @brief Returns element at specified index.
*
* @param[in] lst Pointer to list.
* @param[in] index Zero-based element index.
*
* @return Pointer to stored element.
* @return NULL if list is NULL or index is out of range.
*/
void* slist_at(const slist_t *lst, const size_t index);

/**
* @brief Removes first element equal to key from list.
*
* Searches list using comparator and 
* removes first matching element.
* 
* @param[in,out] lst Pointer to list.
* @param[in] key Pointer to key value.
* @param[in] comparator Comparator function.
*
* @return 0 on success. 
* @return -1 on failure.
*/
int slist_remove(slist_t *lst, const void* key, comparator_fn comparator); 

/**
* @brief Removes element at position.
*
* Removes element at specified zero-based position.
* 
* @param[in,out] lst Pointer to list.
* @param[in] position Zero-based element position.
*
* @return 0 on success. 
* @return -1 on failure.
*/
int slist_remove_at(slist_t *lst, const size_t position);

/**
* @brief Removes all elements equal to key from list.
*
* Searches list using comparator and removes every matching element.
*
* @param[in,out] lst Pointer to list.
* @param[in] key Pointer to key value.
* @param[in] comparator Comparator function.
*
* @return Number of removed elements.
* @return -1 on failure. 
*/
int slist_remove_all(slist_t *lst, const void* key, comparator_fn comparator); 

/**
* @brief Removes first element from list.
*
* Removes first node and returns copy of removed item.
*
* @param[in,out] lst Pointer to list.
*
* @return Pointer to removed element copy
* @return NULL if list is NULL or empty.
*/
void* slist_pop_front(slist_t *lst);

/**
* @brief Removes last element from list.
*
* Removes last node and returns copy of removed item.
*
* @param[in,out] lst Pointer to list.
*
* @return Pointer to removed element copy.
* @return NULL if list is NULL or empty.
*/
void* slist_pop_back(slist_t *lst); 

/**
* @brief Removes first @p n elements from list.
*
* @param[in,out] lst Pointer to list.
* @param[in] n Number of elements to remove.
*
* @return Number of removed elements.
* @return 0 if @p n is zero.
* @return -1 on failure.
*/
int slist_trim_front(slist_t *lst, const size_t n); 

/**
* @brief Removes last @p n elements from list.
*
* @param[in,out] lst Pointer to list.
* @param[in] n Number of elements to remove.
*
* @return Number of removed elements.
* @return 0 if @p n is zero.
* @return -1 on failure.
*/
int slist_trim_back(slist_t *lst, const size_t n); 

/**
* @brief Removes range of elements from list.
*
* Removes elements in half-open range [start, end).
*
* @param[in,out] lst Pointer to list.
* @param[in] start Zero-based start index.
* @param[in] end Zero-based end index, not included.
*
* @return Number of removed elements.
* @return 0 if range is empty.
* @return -1 on failure.
*/
int slist_trim_range(slist_t *lst, const size_t start, const size_t end);

/**
* @brief Creates copy of list.
*
* Allocates new list and copies all stored elements.
*
* @param[in] lst Pointer to source list.
*
* @return Pointer to copied list on success.
* @return NULL on failure.
*/
slist_t *slist_copy(const slist_t *lst);

/**
 * @brief Appends copy of source list to destination list.
 * 
 * Copies all elements from source list and 
 * appends them in destination list.
 * 
 * @param [in,out] lst_destination Pointer to destintation list.
 * @param [in] lst_source Pointer to source list.
 * 
 * @return 0 on success.
 * @return -1 on failure.
 */
int slist_concat(slist_t *lst_destination, const slist_t *lst_source); 

/**
 * @brief Moves all elements from source list to destination list.
 * 
 * Transfers all nodes from source list to end of destination list.
 * Source list becomes empty after call.
 * 
 * @param [in,out] lst_destination Pointer to destintation list.
 * @param [in,out] lst_source Pointer to source list.
 * 
 * @return 0 on success.
 * @return -1 on failure.
 */
int slist_splice(slist_t *lst_destination, slist_t *lst_source); 

/**
* @brief Checks if list contains element equal to key.
*
* Searches list using comparator.
*
* @param[in] list Pointer to list.
* @param[in] key Pointer to key value.
* @param[in] comparator Comparator function.
*
* @return 1 if matching element is found.
* @return 0 if matching element is not found.
* @return -1 on failure.
*/
int slist_contains(const slist_t *lst, const void *key, comparator_fn comparator);

/**
* @brief Counts elements equal to key.
*
* Searches list using comparator and counts matching elements.
*
* @param[in] list Pointer to list.
* @param[in] key Pointer to key value.
* @param[in] comparator Comparator function.
*
* @return Number of matching elements.
* @return -1 on failure.
*/
int slist_count(const slist_t *lst, const void *key, comparator_fn comparator);

/**
 * @brief Finds element equal to key.
 * 
 * Searches list using comparator and 
 * return first matching element.
 * 
 * @param lst Pointer to list.
 * @param key Pointer to key value.
 * @param comparator Comparator function.
 * 
 * @return Pointer to found element.
 * @return NULL if not found or on failure.
 */
void* slist_find(const slist_t *lst, const void *key, comparator_fn comparator);

/**
* @brief Creates filtered copy of list.
*
* Copies elements for which predicate returns non-zero value.
*
* @param[in] lst Pointer to source list.
* @param[in] context Pointer to user-defined predicate context.
* @param[in] predicate Predicate function with context.
*
* @return Pointer to filtered list on success.
* @return NULL on failure.
*/
slist_t* slist_filter(const slist_t *lst, predicate_fn predicate, const void *context);

/**
 * @brief Checks if list is sorted.
 * 
 * @param [in] lst Pointer to list.
 * @param [in] comparator Comparator function.
 * 
 * @return 1 if list is sorted.
 * @return 0 if list is not sorted.
 * @return -1 on failure.
 */
int slist_is_sorted(slist_t *lst, comparator_fn comparator);

/**
 * @brief Sorts list using comparator.
 * 
 * @param [in,out] lst Pointer to list.
 * @param [in] comparator Comparator function.
 * 
 * @return 0 on success.
 * @return -1 on failure.
 */
int slist_sort(slist_t *lst, comparator_fn comparator);

/**
 * @brief Sorts list using bogosort algorithm.
 * 
 * @param [in,out] lst Pointer to list.
 * @param [in] comparator Comparator function.
 * 
 * @return 0 on success.
 * @return -1 on failure.
 * 
 * @warning Extremely inefficient algotithm.
 * Average time complexity is unbounded (0(n!)).
 * Intended for testing or educational purposes only.
 */
int slist_bogosort(slist_t *lst, comparator_fn comparator);

/**
 * @brief Returns pointer to first node.
 * 
 * @param [in] lst Pointer to list.
 * 
 * @return Pointer to head node.
 * @return NULL if @p lst is NULL or empty.
 */
snode_t* slist_head(const slist_t *lst);

/**
 * @brief Returns pointer to last node.
 * 
 * @param [in] lst Pointer to list.
 * 
 * @return Pointer to tail node.
 * @return NULL if @p lst is NULL or empty.
 */
snode_t* slist_tail(const slist_t *lst);

/**
 * @brief Returns pointer to next node.
 * 
 * @param [in] nod Pointer to node.
 * 
 * @return Pointer to next node.
 * @return NULL if @p nod is NULL or last node.
 */
snode_t *snode_next(const snode_t *nod);

/**
 * @brief Returns pointer to node data.
 * 
 * @param [in] nod Pointer to node
 * 
 * @return Pointer to stored data.
 * @return NULL if @p nod is NULL
 */
void* snode_data(const snode_t *nod);

/**
 * @brief Returns number of stored elements in list.
 * 
 * @param [in] lst Pointer to list.
 * 
 * @return Number of elements in list.
 * @return 0 if @p lst is NULL.
 */
size_t slist_size(const slist_t *lst);

/**
 * @brief Returns size of stored element.
 * 
 * @param [in] lst Pointer to list.
 * 
 * @return Size of element in bytes.
 * @return 0 if @p lst is NULL.
 */
size_t slist_item_size(const slist_t *lst);

/**
 * @brief Returns first element in list.
 * 
 * @param [in] lst Pointer to list.
 * 
 * @return Pointer to first element.
 * @return NULL if list is NULL or empty.
 */
void* slist_front_item(const slist_t *lst);

/**
 * @brief Returns last element in list.
 * 
 * @param [in] lst Pointer to list.
 * 
 * @return Pointer to last element.
 * @return NULL if list is NULL or empty.
 */
void* slist_back_item(const slist_t *lst);

#endif