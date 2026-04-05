#include "list_c.h"

/** @brief Структура одного узла односвязного линейного списка */
struct Node{
	void *item; ///< Указатель на данные
	node_t *next; ///< Указатель на следующий по счету узел
};

/** @brief One-way linked list */
struct OWList{
	size_t list_size; ///< Количество элементов списка
	size_t item_size; ///< Размер выделенной памяти для каждого элемента
	struct Node *head; ///< Указатель на первый узел списка
	struct Node *tail; ///< Указатель на последний элемент списка
};

/*---------------- C-like getters ----------------*/
size_t list_size(const list_t *lst) {
	return (!lst) ? 0 : lst->list_size;
}

size_t list_item_size(const list_t *lst) {
	return (!lst) ? 0 : lst->item_size;
}

node_t* list_head(const list_t *lst) {
	return (!lst || lst->list_size == 0) ? NULL : lst->head;
}

node_t* list_tail(const list_t *lst) {
	return (!lst || lst->list_size == 0) ? NULL : lst->tail;
}

node_t* node_next(const node_t *nod) {
	return (!nod) ? NULL : nod->next;
}

void* node_data(const node_t *nod) {
	return (!nod) ? NULL : nod->item;
}

void* list_front_item(const list_t *lst) {
	return (!lst || lst->list_size == 0) ? NULL : lst->head->item;
}

void* list_back_item(const list_t *lst) {
	return (!lst || lst->list_size == 0) ? NULL : lst->tail->item;
}

void* list_at(const list_t *lst, const size_t index) {
	if(!lst) return NULL;
	if(index >= lst->list_size) return NULL;

	struct Node *current_node = lst->head;
	int count = 0;
	while(current_node != NULL) {
		if(index == count)
			return current_node->item;
		current_node = current_node->next;
		count++;
	}
	return NULL;
}
/*------------------------------------------------*/

list_t* list_create(const size_t item_size) {
	if(item_size == 0) return NULL;
	
	// allocate list memory 
	struct OWList* lst = (list_t*)malloc(sizeof(struct OWList));
	if(!lst) return NULL;

	// make list empty
	lst->list_size = 0;
	lst->item_size = item_size;
	lst->head = NULL;
	lst->tail = NULL;

	return lst;
}

void list_clear(list_t *lst){
	if(!lst) return;
	 
	struct Node *current_node = lst->head;
	// clear all items
	while(current_node) { 
		node_t* next_node = current_node->next;
		free(current_node->item);
		free(current_node);
		current_node = next_node;
	}
	lst->head = NULL;
	lst->tail = NULL;
	lst->list_size = 0;
}

void list_delete(list_t **lst) {
	if (!lst || !(*lst)) return;

	// clean items
	struct Node *current_node = (*lst)->head;
	while(current_node != NULL){
		node_t* next_node = current_node->next;
		free(current_node->item);
		free(current_node);
		current_node = next_node;
	}
	
	// delete list
	free(*lst);
	(*lst) = NULL;
}

int list_empty(const list_t *lst) {
	if(!lst) return -1;
	return lst->list_size == 0; // 1 - empty, 0 - not empty
}

int list_append(list_t *lst, const void *item) {
	if(!lst || !item) return -1;

	// try to allocate memory for new node
	struct Node *new_node = (node_t*)malloc(sizeof(struct Node));
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

int list_prepend(list_t *lst, const void *item) {
	if(!lst || !item) return -1;

	// try to allocate memory for new node
	struct Node *new_node = (node_t*)malloc(sizeof(struct Node));
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
		node_t *tmp = lst->head;
		lst->head = new_node;
		lst->head->next = tmp;
	}

	lst->list_size += 1;
	return 0;
}

int list_insert(list_t *lst, const void *item, const size_t pos) {
	if(!lst || !item) return -1;
	if(pos > lst->list_size) return -1;

	size_t count = 0;
	struct Node *current_node = lst->head;
	struct Node *previous_node = NULL;

	while(current_node) {
		if(count == pos) {
			if(current_node == lst->head) {
				list_prepend(lst, item);
				return 0;
			}
			else {
				// try to allocate memory for new node
				struct Node *new_node = (node_t*)malloc(sizeof(struct Node));
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
		list_append(lst, item);
		return 0;
	}
	return -1;
}

int list_remove(list_t *lst, void *key, comparator_fn comparator) {
	if(!lst || !key || !comparator) return -1;

	struct Node *current_node = lst->head;
	struct Node *previous_node = NULL, *next_node = NULL;

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
			return 1;
		}
		previous_node = current_node;
		current_node = next_node;
	}
	return 0;
}

int list_remove_at(list_t *lst, size_t position){
	if(!lst || position >= lst->list_size) return -1;

	int count = 0;
	struct Node *current_node = lst->head;
	struct Node *previous_node = NULL, *next_node = NULL;
	
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
			return 1;
		}
		previous_node = current_node;
		current_node = next_node;
		count++;
	}
	return 0;
}

int list_remove_all(list_t *lst, void *key, comparator_fn comparator) {
	if(!lst || !key || !comparator) return -1;

	int removed_items_count = 0;
	struct Node *current_node = lst->head;
	struct Node *previous_node, *next_node;

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
	return removed_items_count;
}

void* list_pop_front(list_t *lst) {
	if(!lst) return NULL;
	if(lst->list_size == 0) return NULL;

	struct Node *current_node = lst->head;

	if(lst->head == lst->tail) // if only one item	
		lst->head = lst->tail = NULL;
	else {
		node_t *next_node = current_node->next;
		lst->head = next_node;
	}

	void* item = malloc(lst->item_size);
	memcpy(item, current_node->item, lst->item_size);

	free(current_node->item);
	free(current_node);
	lst->list_size -= 1;
	return item;
}

void* list_pop_back(list_t *lst) {
	if(!lst) return NULL;
	if(lst->list_size == 0) return NULL;

	struct Node *current_node = lst->head;
	struct Node *previous_node = NULL;

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

list_t *list_copy(const list_t *lst) {
	if(!lst) return NULL;

	list_t *copy = list_create(lst->item_size);
	if(!copy) return NULL;
	if(lst->list_size) {
		struct Node *current_node = lst->head;
		while(current_node) {
			list_append(copy, current_node->item);
			current_node = current_node->next;
		}
	}
	return copy;
}

int list_contains(const list_t *lst, const void* key, comparator_fn comparator) {
	if(!lst || !key || !comparator) return -1;
	if(!lst->item_size) return -1;

	if(lst->list_size) {
		struct Node *current_node = lst->head;
		while(current_node != NULL) {
			if(comparator(current_node->item, key) == 0)
				return 1;
			current_node = current_node->next;
		}
	}
	return 0;
}

int list_count(const list_t *lst, const void *key, comparator_fn comparator) {
	if(!lst || !key || !comparator) return -1;
	if(!lst->item_size) return -1;
	int count = 0;
	if(lst->list_size) {
		struct Node *current_node = lst->head;
		while(current_node != NULL) {
			if(comparator(current_node->item, key) == 0)
				count++;
			current_node = current_node->next;
		}
	}
	return count;
}

void* list_find(const list_t *lst, const void *key, comparator_fn comparator) {
	if(!lst || !key || !comparator) return NULL;
	if(!lst->item_size) return NULL;
	if(lst->list_size){
		struct Node *current_node = lst->head;
		while(current_node != NULL) {
			if(comparator(current_node->item, key) == 0) {
				return current_node->item;
			}
			current_node = current_node->next;
		}
	}
	return NULL;
}

list_t* list_filter(const list_t *lst, predicate_fn predicate, void* context) {
	if(lst == NULL || predicate == NULL || context == NULL) return NULL;
	if(!lst->item_size) return NULL;

	list_t* destination = list_create(lst->item_size);
	if(!destination) return NULL;
	if(lst->list_size) {
		struct Node* current_node = lst->head;
		while(current_node) {
			if(predicate(current_node->item, context) == 0)
				list_append(destination, current_node->item);
			current_node = current_node->next;
	 	}
	}
	return destination;
}

int list_trim_front(list_t* lst, const size_t n) {
	if(!lst) return -1;
	if(n > lst->list_size) return -1;
	if(!n) return 0;
	int i;
	for(i = 0; i < n; i++){
		struct Node *current_node = lst->head;
		if(lst->head == lst->tail) // if only one item
			lst->head = lst->tail = NULL;
		else {
			node_t *next_node = lst->head->next;
			lst->head = next_node;
		}
		free(current_node->item);
		free(current_node);
		lst->list_size -= 1;
	}
	return i;
}

int list_trim_back(list_t* lst, const size_t n) {
	if(!lst) return -1;
	if(n > lst->list_size) return -1;
	if(!n) return 0;

	int trim_start = lst->list_size - n;
	struct Node* current_node = lst->head;
	struct Node* previous_node, *next_node;
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

int list_trim_range(list_t *lst, const size_t start, const size_t end) {
	if(!lst) return -1;
	if(start > end || end > lst->list_size) return -1;
	if(start == end) return 0;

	if(start == 0 && end == lst->list_size){
		list_clear(lst);
		return end;
	}
	if(start == 0)
		return list_trim_front(lst, end);

	int count = 0;

	// we must find left_node and right_node
	struct Node *left_node = NULL;
	struct Node *right_node = NULL;
	struct Node *current_node = lst->head;
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

int list_set(list_t *lst, size_t index, void* item){
	if(!lst || !item) return -1;
	if(index >= lst->list_size) return -1;
	
	struct Node *current_node = lst->head;
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

int list_concat(list_t *lst_destination, const list_t *lst_source){
	if(!lst_source || !lst_destination) return -1;
	if(lst_destination == lst_source) return -1;

	if(list_empty(lst_source)) return 0;
	
	struct Node *source_current_node = lst_source->head;
	while(source_current_node) {
		list_append(lst_destination, source_current_node->item);
		source_current_node = source_current_node->next;
	}

	return 0;
}

int list_splice(list_t *lst_destination, list_t *lst_source) {
	if(!lst_destination || !lst_source) return -1;
	if(lst_destination->item_size != lst_source->item_size) return -1;
	if(lst_destination == lst_source) return -1;
	
	if(list_empty(lst_source)) return 0;

	if(list_empty(lst_destination)) 
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

struct MergeChain {
	struct Node *begin; 
	struct Node *end;
};

static struct MergeChain _merge(struct Node *left, struct Node *right, comparator_fn cmp) {
	node_t *head = NULL, *tail = NULL;
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

static struct MergeChain _merge_sort(struct Node *left, struct Node *right, comparator_fn cmp) {

	if(left == right) {
		struct MergeChain both;
		both.begin = left;
		both.end = right;
		return both;
	}
	// find middle
	struct Node *slow = left, *fast = left, *prev = NULL;
	while(fast && fast->next) {
		prev = slow;
		slow = slow->next;
		fast = fast->next->next;
	}

	// split list
	if(prev) prev->next = NULL;
	struct MergeChain left_sorted = _merge_sort(left, prev, cmp);
	struct MergeChain right_sorted = _merge_sort(slow, right, cmp);

	return _merge(left_sorted.begin, right_sorted.begin, cmp);
}

int list_sort(list_t *lst, comparator_fn cmp) {
	if(!lst || !cmp) return -1; // error
	// отправная точка в сортировку слиянием
	if(lst->list_size > 1) {
		struct MergeChain sorted = _merge_sort(lst->head, lst->tail, cmp);
		lst->head = sorted.begin;
		lst->tail = sorted.end;
	}
	return 0;
}

static void _array_shuffle(node_t** array, int n) {
	if(n < 2) return;
	// Init random generation
    srand((unsigned)time(NULL));
    for (unsigned i = n - 1; i > 0; --i) {

        unsigned j = rand() % (i + 1);

		node_t *tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}
}

static void _array_to_list(node_t **array, list_t *lst) {
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

static node_t** _list_to_array(const list_t *lst) {
	node_t **array = calloc(sizeof(node_t*), lst->item_size);
	node_t *current = lst->head;
	for(unsigned i = 0; i < lst->list_size; i++, current = current->next)
		array[i] = current;
	return array;
}

static void _list_shuffle(list_t *lst) {
	node_t** array = _list_to_array(lst);
	_array_shuffle(array, lst->list_size);
	_array_to_list(array, lst);
	if(array) free(array);
}

int list_is_sorted(list_t *lst, comparator_fn cmp) {
	node_t * current = lst->head;
	while (current && current->next) {
		if(cmp(current->item, current->next->item) > 0)
			return -1;
		current = current->next;
	}
	return 0;
}

int list_bogosort(list_t *lst, comparator_fn cmp){
	if(!lst || !cmp) return -1;
	if(lst->list_size > 1) { 
		while(list_is_sorted(lst, cmp) < 0) 
			_list_shuffle(lst);
	}
	return 0;
}