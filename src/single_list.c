#include "single_list.h"

/** 
 * @brief Node structure for single-linked list container.
 */
struct SingleNode {
	void *item; 	/**< Pointer to node data. */ 
	snode_t *next; 	/**< Pointer to next node. */ 
};

/** 
 * @brief Single-linked list container.
 */
struct SingleList {
	size_t list_size; 		 /**< Number of elements in list. */
	size_t item_size; 		 /**< Byte size of element. */
	struct SingleNode *head; /**< Pointer to first node. */
	struct SingleNode *tail; /**< Pointer to last node. */
};

slist_t* slist_create(const size_t item_size) {
	if(item_size == 0) return NULL;
	// allocate list memory 
	struct SingleList* lst = (slist_t*)malloc(sizeof(struct SingleList));
	if(!lst) return NULL;
	// make list empty
	lst->list_size = 0;
	lst->item_size = item_size;
	lst->head = NULL;
	lst->tail = NULL;
	return lst;
}

void slist_clear(slist_t *lst) { 
	if(!lst) return;
	struct SingleNode *current_node = lst->head;
	// clear all items
	while(current_node) { 
		snode_t* next_node = current_node->next;
		free(current_node->item);
		free(current_node);
		current_node = next_node;
	}
	lst->head = NULL;
	lst->tail = NULL;
	lst->list_size = 0;
}

void slist_delete(slist_t **lst) {
	if (!lst || !(*lst)) return;
	// clean items
	struct SingleNode *current_node = (*lst)->head;
	while(current_node != NULL){
		snode_t* next_node = current_node->next;
		free(current_node->item);
		free(current_node);
		current_node = next_node;
	}
	// delete list
	free(*lst);
	(*lst) = NULL;
}

int slist_empty(const slist_t *lst) {
	if(!lst) return -1;
	return lst->list_size == 0; // 1 - empty, 0 - not empty
}

int slist_append(slist_t *lst, const void *item) {
	if(!lst || !item) return -1;

	// try to allocate memory for new node
	struct SingleNode *new_node = (snode_t*)malloc(sizeof(struct SingleNode));
	if(!new_node) return -1;

	// try to allocate memory for new item
	new_node->item = malloc(lst->item_size);
	if(!new_node->item) {
		free(new_node);
		return -1;
	} 
	memcpy(new_node->item, item, lst->item_size);
	new_node->next = NULL;

	// if list is empty
	if(!lst->head) {
		lst->head = new_node;
	    lst->tail = new_node;
	}
	else { // list size is greater than 0
		lst->tail->next = new_node;
		lst->tail = new_node;
	}
	lst->list_size += 1;
	return 0;
}

int slist_prepend(slist_t *lst, const void *item) {
	if(!lst || !item) return -1;

	// try to allocate memory for new node
	struct SingleNode *new_node = (snode_t*)malloc(sizeof(struct SingleNode));
	if(!new_node) return -1;

	// try to allocate memory for new item
	new_node->item = malloc(lst->item_size);
	if(!new_node->item) {
		free(new_node);
		return -1;
	}
	memcpy(new_node->item, item, lst->item_size);
	new_node->next = NULL;

	// if list is empty
	if(!lst->head) {
		lst->head = new_node;
	    lst->tail = new_node;
	}
	else { // list size is greater than 0
		snode_t *tmp = lst->head;
		lst->head = new_node;
		lst->head->next = tmp;
	}

	lst->list_size += 1;
	return 0;
}

int slist_insert(slist_t *lst, const void *item, const size_t pos) {
	if(!lst || !item) return -1;
	if(pos > lst->list_size) return -1;

	size_t count = 0;
	struct SingleNode *current_node = lst->head;
	struct SingleNode *previous_node = NULL;

	while(current_node) {
		if(count == pos) {
			if(current_node == lst->head) {
				slist_prepend(lst, item);
				return 0;
			}
			else {
				// try to allocate memory for new node
				struct SingleNode *new_node = (snode_t*)malloc(sizeof(struct SingleNode));
				if(!new_node) return -1;

				// try to allocate memory for new item
				new_node->item = malloc( lst->item_size);
				if(!new_node->item) {
					free(new_node);
					return -1;
				}
				memcpy(new_node->item, item, lst->item_size);
				// node swapping
				previous_node->next = new_node;
				new_node->next = current_node;
				lst->list_size += 1;
				return 0;
			}
		}
		count++;
		previous_node = current_node;
		current_node = current_node->next;
	}

	if(pos == lst->list_size) {
		slist_append(lst, item);
		return 0;
	}
	return -1;
}

int slist_set(slist_t *lst, const size_t index, const void* item){
	if(!lst || !item) return -1;
	if(index >= lst->list_size) return -1;
	
	struct SingleNode *current_node = lst->head;
	int current_index = 0;
	while (current_node)
	{
		if(current_index == index){
			memcpy(current_node->item, item, lst->item_size);
			return 0;
		}
		current_node = current_node->next;
		current_index++;
	}
	return -1;
}

void* slist_at(const slist_t *lst, const size_t index) {
	if(!lst) return NULL;
	if(index >= lst->list_size) return NULL;

	struct SingleNode *current_node = lst->head;
	int count = 0;
	while(current_node != NULL) {
		if(index == count)
			return current_node->item;
		current_node = current_node->next;
		count++;
	}
	return NULL;
}

int slist_remove(slist_t *lst, const void *key, comparator_fn comparator) {
	if(!lst || !key || !comparator) return -1;
	if(lst->list_size == 0) return -1;

	struct SingleNode *current_node = lst->head;
	struct SingleNode *previous_node = NULL, *next_node = NULL;

	while(current_node) {
		next_node = current_node->next;
		// check by comaprator
		if(comparator(current_node->item, key) == 0) {
			if(current_node == lst->head) { // remove first
				if(lst->tail == lst->head) // if only one item in list
					lst->head = lst->tail = NULL;	
				else lst->head = next_node;
			}
			else if(current_node == lst->tail) { // remove last
				lst->tail = previous_node;
				previous_node->next = NULL;
			}
			else previous_node->next = next_node; // remove in the middle

			free(current_node->item);
			free(current_node);

			lst->list_size -= 1;
			return 0;
		}
		previous_node = current_node;
		current_node = next_node;
	}
	return -1;
}

int slist_remove_at(slist_t *lst, const size_t position){
	if(!lst || position >= lst->list_size) return -1;
	
	int count = 0;
	struct SingleNode *current_node = lst->head;
	struct SingleNode *previous_node = NULL, *next_node = NULL;
	
	while(current_node) {
		next_node = current_node->next;
		// check by comaprator
		if(position == count) {
			if(current_node == lst->head) { // remove first
				if(lst->tail == lst->head) // if only one item in list
					lst->head = lst->tail = NULL;	
				else lst->head = next_node;
			}
			else if(current_node == lst->tail) { // remove last
				lst->tail = previous_node;
				previous_node->next = NULL;
			}
			else previous_node->next = next_node; // remove in the middle

			free(current_node->item);
			free(current_node);

			lst->list_size -= 1;
			return 0;
		}
		previous_node = current_node;
		current_node = next_node;
		count++;
	}
	return -1;
}

int slist_remove_all(slist_t *lst, const void *key, comparator_fn comparator) {
	if(!lst || !key || !comparator) return -1;
	if(lst->list_size == 0) return -1;
	
	int removed_items_count = 0;
	struct SingleNode *current_node = lst->head;
	struct SingleNode *previous_node, *next_node;

	while(current_node) {
		next_node = current_node->next;
		// check by comaprator
		if(comparator(current_node->item, key) == 0) {
			if(current_node == lst->head) { // remove first
				if(lst->head == lst->tail) // if only one item in list
					lst->head = lst->tail = NULL;
				else lst->head = next_node;
			}
			else if(current_node == lst->tail) { // remove last
				lst->tail = previous_node;
				previous_node->next = NULL;
			}
			else previous_node->next = next_node; // remove in the middle
			
			free(current_node->item); 
			free(current_node);
			lst->list_size -= 1;
			removed_items_count++;
			current_node = next_node;
		}
		else {
			previous_node = current_node;
			current_node = current_node->next;
		}
	}
	if(!removed_items_count) return -1;
	return removed_items_count;
}

void* slist_pop_front(slist_t *lst) {
	if(!lst) return NULL;
	if(lst->list_size == 0) return NULL;

	struct SingleNode *current_node = lst->head;

	if(lst->head == lst->tail) // if only one item	
		lst->head = lst->tail = NULL;
	else {
		snode_t *next_node = current_node->next;
		lst->head = next_node;
	}

	void* item = malloc(lst->item_size);
	memcpy(item, current_node->item, lst->item_size);

	free(current_node->item);
	free(current_node);
	lst->list_size -= 1;
	return item;
}

void* slist_pop_back(slist_t *lst) {
	if(!lst) return NULL;
	if(lst->list_size == 0) return NULL;

	struct SingleNode *current_node = lst->head;
	struct SingleNode *previous_node = NULL;

	while(current_node) {
		if(current_node == lst->tail) {
			if(lst->head == lst->tail) // if only one item
				lst->head = lst->tail = NULL;
			else {
				lst->tail = previous_node;
				// cut node connection with tail
				if(previous_node) previous_node->next = NULL; // prev is new tail node
			}
			
			void* item = malloc(lst->item_size);
			memcpy(item, current_node->item, lst->item_size);

			free(current_node->item);
			free(current_node);

			lst->list_size -= 1;
			return item;
		}

		previous_node = current_node;
		current_node = current_node->next;
	}
	return NULL;
}

int slist_trim_front(slist_t* lst, const size_t n) {
	if(!lst) return -1;
	if(n > lst->list_size) return -1;
	if(!n) return 0;
	int i;
	for(i = 0; i < n; i++){
		struct SingleNode *current_node = lst->head;
		if(lst->head == lst->tail) // if only one item
			lst->head = lst->tail = NULL;
		else {
			snode_t *next_node = lst->head->next;
			lst->head = next_node;
		}
		free(current_node->item);
		free(current_node);
		lst->list_size -= 1;
	}
	return i;
}

int slist_trim_back(slist_t* lst, const size_t n) {
	if(!lst) return -1;
	if(n > lst->list_size) return -1;
	if(!n) return 0;

	int trim_start = lst->list_size - n;
	struct SingleNode* current_node = lst->head;
	struct SingleNode* previous_node, *next_node;
	int i = 0, count = 0;
	while(current_node) {
		next_node = current_node->next;
		if(i >= trim_start) {
			if(current_node == lst->head) // remove first
				lst->head = next_node;
			else if(current_node == lst->tail) { // remove last
				lst->tail = previous_node;
				previous_node->next = NULL;
			}
			else // remove in the middle
				previous_node->next = next_node;

			free(current_node->item);
			free(current_node);
			lst->list_size -= 1;
			count++;
			current_node = next_node;
		}
		else {
			previous_node = current_node;
			current_node = current_node->next;
		}
		i++;
	}
	return count;
}

int slist_trim_range(slist_t *lst, const size_t start, const size_t end) {
	if(!lst) return -1;
	if(start > end || end > lst->list_size) return -1;
	if(start == end) return 0;

	if(start == 0 && end == lst->list_size){
		slist_clear(lst);
		return end;
	}
	if(start == 0)
		return slist_trim_front(lst, end);

	int count = 0;

	// we must find left_node and right_node
	struct SingleNode *left_node = NULL;
	struct SingleNode *right_node = NULL;
	struct SingleNode *current_node = lst->head;
	// node_t* previous_node = NULL;

	while(current_node){
		if(count == end){
			right_node = current_node;
			break;
		}

		if(left_node){
			// remove item
			free(current_node->item);
			free(current_node);
		}
		else if(count == start - 1)
			left_node = current_node;

		count++;
		// previous_node = current_node;
		current_node = current_node->next;
	}

	if(right_node)
		left_node->next = right_node;
	else{
		// looks like end is equal to list->size
		left_node->next = NULL;
		lst->tail = left_node;
	}

	lst->list_size -= (end - start);
	return end - start;
}

slist_t *slist_copy(const slist_t *lst) {
	if(!lst) return NULL;

	slist_t *copy = slist_create(lst->item_size);
	if(!copy) return NULL;
	if(lst->list_size) {
		struct SingleNode *current_node = lst->head;
		while(current_node) {
			slist_append(copy, current_node->item);
			current_node = current_node->next;
		}
	}
	return copy;
}

int slist_concat(slist_t *lst_destination, const slist_t *lst_source){
	if(!lst_source || !lst_destination) return -1;
	if(lst_destination->item_size != lst_source->item_size) return -1;
	if(lst_destination == lst_source) return -1;
	
	if(slist_empty(lst_source)) return 0;
	
	struct SingleNode *source_current_node = lst_source->head;
	while(source_current_node) {
		slist_append(lst_destination, source_current_node->item);
		source_current_node = source_current_node->next;
	}

	return 0;
}

int slist_splice(slist_t *lst_destination, slist_t *lst_source) {
	if(!lst_destination || !lst_source) return -1;
	if(lst_destination->item_size != lst_source->item_size) return -1;
	if(lst_destination == lst_source) return -1;
	
	if(slist_empty(lst_source)) return 0;

	if(slist_empty(lst_destination)) 
		lst_destination->head = lst_source->head;
	else 
		lst_destination->tail->next = lst_source->head;

	lst_destination->tail = lst_source->tail;
	lst_destination->list_size += lst_source->list_size;

	lst_source->head = NULL;
	lst_source->tail = NULL;
	lst_source->list_size = 0;
	return 0;
}

int slist_contains(const slist_t *lst, const void* key, comparator_fn comparator) {
	if(!lst || !key || !comparator) return -1;
	if(!lst->item_size) return -1;

	if(lst->list_size) {
		struct SingleNode *current_node = lst->head;
		while(current_node != NULL) {
			if(comparator(current_node->item, key) == 0)
				return 1;
			current_node = current_node->next;
		}
	}
	return 0;
}

int slist_count(const slist_t *lst, const void *key, comparator_fn comparator) {
	if(!lst || !key || !comparator) return -1;
	if(!lst->item_size) return -1;
	int count = 0;
	if(lst->list_size) {
		struct SingleNode *current_node = lst->head;
		while(current_node != NULL) {
			if(comparator(current_node->item, key) == 0)
				count++;
			current_node = current_node->next;
		}
	}
	return count;
}

void* slist_find(const slist_t *lst, const void *key, comparator_fn comparator) {
	if(!lst || !key || !comparator) return NULL;
	if(!lst->item_size) return NULL;
	if(lst->list_size) {
		struct SingleNode *current_node = lst->head;
		while(current_node != NULL) {
			if(comparator(current_node->item, key) == 0) {
				return current_node->item;
			}
			current_node = current_node->next;
		}
	}
	return NULL;
}

slist_t* slist_filter(const slist_t *lst, predicate_fn predicate, const void* context) {
	if(lst == NULL || predicate == NULL || context == NULL) return NULL;
	if(!lst->item_size) return NULL;

	slist_t* destination = slist_create(lst->item_size);
	if(!destination) return NULL;
	if(lst->list_size) {
		struct SingleNode* current_node = lst->head;
		while(current_node) {
			if(predicate(current_node->item, context) != 0)
				slist_append(destination, current_node->item);
			current_node = current_node->next;
	 	}
	}
	return destination;
}

int slist_is_sorted(slist_t *lst, comparator_fn comparator) {
	if(!lst || !comparator) return -1;
	if(lst->list_size == 0) return -1;
	snode_t * current = lst->head;
	while (current && current->next) {
		if(comparator(current->item, current->next->item) > 0)
			return -1;
		current = current->next;
	}
	return 0;
}

struct MergeChain {
	struct SingleNode *begin; 
	struct SingleNode *end;
};

static struct MergeChain _merge(struct SingleNode *left, struct SingleNode *right, comparator_fn cmp) {
	snode_t *head = NULL, *tail = NULL;
	// first elem 
	if(cmp(left->item, right->item) > 0) {
		head = right; 
		tail = right;
		right = right->next;
	}
	else {
		head = left; 
		tail = left;
		left = left->next;
	}

	while(left && right){
		if(cmp(left->item, right->item) > 0) {
			tail->next = right;
			right = right->next;
		}
		else {
			tail->next = left;
			left = left->next;
		}
		tail = tail->next;
	}

	tail->next = (right) ? right : left;
	while (tail->next) tail = tail->next;

	struct MergeChain sorted;
	sorted.begin = head;
	sorted.end = tail;
	return sorted;
};

static struct MergeChain _merge_sort(struct SingleNode *left, struct SingleNode *right, comparator_fn comparator) {

	if(left == right) {
		struct MergeChain both;
		both.begin = left;
		both.end = right;
		return both;
	}
	// find middle
	struct SingleNode *slow = left, *fast = left, *prev = NULL;
	while(fast && fast->next) {
		prev = slow;
		slow = slow->next;
		fast = fast->next->next;
	}

	// split list
	if(prev) prev->next = NULL;
	struct MergeChain left_sorted = _merge_sort(left, prev, comparator);
	struct MergeChain right_sorted = _merge_sort(slow, right, comparator);

	return _merge(left_sorted.begin, right_sorted.begin, comparator);
}

int slist_sort(slist_t *lst, comparator_fn comparator) {
	if(!lst || !comparator) return -1; // error
	// отправная точка в сортировку слиянием
	if(lst->list_size > 1) {
		struct MergeChain sorted = _merge_sort(lst->head, lst->tail, comparator);
		lst->head = sorted.begin;
		lst->tail = sorted.end;
	}
	return 0;
}

static void _array_shuffle(snode_t** array, int n) {
	if(n < 2) return;
	// Init random generation
    srand((unsigned)time(NULL));
    for (unsigned i = n - 1; i > 0; --i) {

        unsigned j = rand() % (i + 1);

		snode_t *tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}
}

static void _array_to_list(snode_t **array, slist_t *lst) {
	if(!lst->list_size) {
		lst->head = NULL;
		lst->tail = NULL;
		return;
	}

	for(unsigned i = 0; i < lst->list_size - 1; i++)
		array[i]->next = array[i + 1];
	array[lst->list_size - 1]->next = NULL;

	lst->head = array[0];
	lst->tail = array[lst->list_size - 1];
}	

static snode_t** _list_to_array(const slist_t *lst) {
	snode_t **array = calloc(sizeof(snode_t*), lst->item_size);
	snode_t *current = lst->head;
	for(unsigned i = 0; i < lst->list_size; i++, current = current->next)
		array[i] = current;
	return array;
}

static void _list_shuffle(slist_t *lst) {
	snode_t** array = _list_to_array(lst);
	_array_shuffle(array, lst->list_size);
	_array_to_list(array, lst);
	if(array) free(array);
}

int slist_bogosort(slist_t *lst, comparator_fn comparator){
	if(!lst || !comparator) return -1;
	if(lst->list_size > 1) { 
		while(slist_is_sorted(lst, comparator) < 0) 
			_list_shuffle(lst);
	}
	return 0;
}

snode_t* slist_head(const slist_t *lst) {
	return (!lst || lst->list_size == 0) ? NULL : lst->head;
}

snode_t* slist_tail(const slist_t *lst) {
	return (!lst || lst->list_size == 0) ? NULL : lst->tail;
}

snode_t* snode_next(const snode_t *nod) {
	return (!nod || !nod->next) ? NULL : nod->next;
}

void* snode_data(const snode_t *nod) {
	return (!nod) ? NULL : nod->item;
}

size_t slist_size(const slist_t *lst) {
	return (!lst) ? 0 : lst->list_size;
}

size_t slist_item_size(const slist_t *lst) {
	return (!lst) ? 0 : lst->item_size;
}

void* slist_front_item(const slist_t *lst) {
	return (!lst || lst->list_size == 0) ? NULL : lst->head->item;
}

void* slist_back_item(const slist_t *lst) {
	return (!lst || lst->list_size == 0) ? NULL : lst->tail->item;
}