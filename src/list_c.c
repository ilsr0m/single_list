#include "list_c.h"

list_t* list_create(const size_t item_size){
	if(item_size == 0) return NULL;
	
	list_t* list = (list_t*)malloc(sizeof(list_t));
	if(!list) return NULL;

	list->list_size = 0;
	list->item_size = item_size;
	list->head = NULL;
	list->tail = NULL;

	return list;
}

void list_clear(list_t *list){
	if(!list) return;
	// прописать очистку итемов
	node_t *current_node = list->head;
	while(current_node){
		node_t* next_node = current_node->next;
		free(current_node->item);
		free(current_node);
		current_node = next_node;
	}
	list->head = NULL;
	list->tail = NULL;
	list->list_size = 0;
}

void list_delete(list_t **list) {
	if (!list || !(*list)) return;
	// clean items
	node_t *current_node = (*list)->head;
	while(current_node != NULL){
		node_t* next_node = current_node->next;
		free(current_node->item);
		free(current_node);
		current_node = next_node;
	}
	
	// delete list
	free(*list);
	(*list) = NULL;
}

int list_append(list_t *list, const void *item) {
	if(!list || !item) return -1;

	// try to allocate memory for new node
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	if(!new_node) return -1;

	// try to allocate memory for new item
	new_node->item = malloc(list->item_size);
	if(!new_node->item) {
		free(new_node);
		return -1;
	} 
	memcpy(new_node->item, item, list->item_size);
	new_node->next = NULL;

	// if list is empty
	if(!list->head) {
		list->head = new_node;
	    list->tail = new_node;
	}
	else { // list size is greater than 0
		list->tail->next = new_node;
		list->tail = new_node;
	}
	list->list_size += 1;
	return 0;
}

int list_prepend(list_t *list, const void *item) {
	if(!list || !item) return -1;

	// try to allocate memory for new node
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	if(!new_node) return -1;

	// try to allocate memory for new item
	new_node->item = malloc(list->item_size);
	if(!new_node->item) {
		free(new_node);
		return -1;
	}
	memcpy(new_node->item, item, list->item_size);
	new_node->next = NULL;

	// if list is empty
	if(!list->head) {
		list->head = new_node;
	    list->tail = new_node;
	}
	else { // list size is greater than 0
		node_t *tmp = list->head;
		list->head = new_node;
		list->head->next = tmp;
	}

	list->list_size += 1;
	return 0;
}

int list_insert(list_t *list, const void *item, const size_t pos) {
	if(!list || !item) return -1;
	if(pos > list->list_size) return -1;

	size_t count = 0;
	node_t *current_node = list->head;
	node_t *previous_node = NULL;

	while(current_node) {
		if(count == pos) {
			if(current_node == list->head) {
				list_prepend(list, item);
				return 0;
			}
			else {
				// try to allocate memory for new node
				node_t* new_node = (node_t*)malloc(sizeof(node_t));
				if(!new_node) return -1;

				// try to allocate memory for new item
				new_node->item = malloc( list->item_size);
				if(!new_node->item) {
					free(new_node);
					return -1;
				}
				memcpy(new_node->item, item, list->item_size);
				// node swapping
				previous_node->next = new_node;
				new_node->next = current_node;
				list->list_size += 1;
				return 0;
			}
		}
		count++;
		previous_node = current_node;
		current_node = current_node->next;
	}

	if(pos == list->list_size) {
		list_append(list, item);
		return 0;
	}
	return -1;
}

void* list_front(list_t* list) {
	return (!list || list->list_size == 0) ? NULL : list->head->item;
}

void* list_back(list_t* list) {
	return (!list || list->list_size == 0) ? NULL : list->tail->item;
}

void* list_remove(list_t* list, void* key, cmp_func_t comparator) {
	if(!list || !key || !comparator) return NULL;

	node_t* current_node = list->head;
	node_t* previous_node; node_t *next_node;

	while(current_node) {
		next_node = current_node->next;
		// check by comaprator
		if(comparator(current_node->item, key) == 0) {
			if(current_node == list->head) { // remove first
				if(list->tail == list->head) // if only one item in list
					list->head = list->tail = NULL;	
				else list->head = next_node;
			}
			else if(current_node == list->tail) { // remove last
				list->tail = previous_node;
				previous_node->next = NULL;
			}
			else previous_node->next = next_node; // remove in the middle
			
			void* item = malloc(list->item_size);
			memcpy(item, current_node->item, list->item_size);

			free(current_node->item);
			free(current_node);

			list->list_size -= 1;
			return item;
		}
		previous_node = current_node;
		current_node = current_node->next;
	}

	return NULL;
}

int list_remove_all(list_t* list, void* key, cmp_func_t comparator) {
	if(!list || !key || !comparator) return -1;

	int removed_items_count = 0;
	node_t* current_node = list->head;
	node_t* previous_node; node_t* next_node;

	while(current_node) {
		next_node = current_node->next;
		// check by comaprator
		if(comparator(current_node->item, key) == 0) {
			if(current_node == list->head) { // remove first
				if(list->head == list->tail) // if only one item in list
					list->head = list->tail = NULL;
				else list->head = next_node;
			}
			else if(current_node == list->tail) { // remove last
				list->tail = previous_node;
				previous_node->next = NULL;
			}
			else previous_node->next = next_node; // remove in the middle
			
			free(current_node->item); 
			free(current_node);
			list->list_size -= 1;
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

void* list_pop_front(list_t *list) {
	if(!list) return NULL;
	if(list->list_size == 0) return NULL;

	node_t *current_node = list->head;

	if(list->head == list->tail) // if only one item	
		list->head = list->tail = NULL;
	else {
		node_t *next_node = current_node->next;
		list->head = next_node;
	}

	void* item = malloc(list->item_size);
	memcpy(item, current_node->item, list->item_size);

	free(current_node->item);
	free(current_node);
	list->list_size -= 1;
	return item;
}

void* list_pop_back(list_t *list) {
	if(!list) return NULL;
	if(list->list_size == 0) return NULL;

	node_t *current_node = list->head;
	node_t *previous_node = NULL;

	while(current_node) {
		if(current_node == list->tail) {
			if(list->head == list->tail) // if only one item
				list->head = list->tail = NULL;
			else {
				list->tail = previous_node;
				// cut node connection with tail
				if(previous_node) previous_node->next = NULL; // prev is new tail node
			}
			
			void* item = malloc(list->item_size);
			memcpy(item, current_node->item, list->item_size);

			free(current_node->item);
			free(current_node);

			list->list_size -= 1;
			return item;
		}

		previous_node = current_node;
		current_node = current_node->next;
	}
	return NULL;
}

void* list_at(const list_t *list, const size_t index) {
	if(!list) return NULL;
	if(index >= list->list_size) return NULL;

	node_t* current_node = list->head;
	int count = 0;
	while(current_node != NULL) {
		if(index == count)
			return current_node->item;
		current_node = current_node->next;
		count++;
	}
	return NULL;
}

list_t *list_copy(const list_t *list) {
	if(!list) return NULL;

	list_t *copy = list_create(list->item_size);
	if(!copy) return NULL;
	if(list->list_size) {
		node_t *current_node = list->head;
		while(current_node) {
			list_append(copy, current_node->item);
			current_node = current_node->next;
		}
	}
	return copy;
}

int list_contains(const list_t *list, const void* key, cmp_func_t comparator) {
	if(!list || !key || !comparator) return -1;
	if(!list->item_size) return -1;

	size_t count = 0;
	if(list->list_size) {
		node_t *current_node = list->head;
		while(current_node != NULL) {
			if(comparator(current_node->item, key) == 0)
			count++;
			current_node = current_node->next;
		}
	}
	return count;
}

list_t* list_filter(const list_t *list, void* context, predicate_fn predicate) {
	if(list == NULL || predicate == NULL || context == NULL) return NULL;
	if(!list->item_size) return NULL;

	list_t* destination = list_create(list->item_size);
	if(!destination) return NULL;
	if(list->list_size) {
		node_t* current_node = list->head;
		while(current_node) {
			if(predicate(current_node->item, context) == 0)
				list_append(destination, current_node->item);
			current_node = current_node->next;
	 	}
	}
	return destination;
}

int list_trim_front(list_t* list, const size_t n) {
	if(!list) return -1;
	if(n > list->list_size) return -1;
	if(!n) return 0;
	int i;
	for(i = 0; i < n; i++){
		node_t *current_node = list->head;
		if(list->head == list->tail) // if only one item
			list->head = list->tail = NULL;
		else {
			node_t *next_node = list->head->next;
			list->head = next_node;
		}
		free(current_node->item);
		free(current_node);
		list->list_size -= 1;
	}
	return i;
}

int list_trim_back(list_t* list, const size_t n) {
	if(!list) return -1;
	if(n > list->list_size) return -1;
	if(!n) return 0;

	int trim_start = list->list_size - n;
	node_t* current_node = list->head;
	node_t* previous_node; node_t *next_node;
	int i = 0, count = 0;
	while(current_node) {
		next_node = current_node->next;
		if(i >= trim_start) {
			if(current_node == list->head) // remove first
				list->head = next_node;
			else if(current_node == list->tail) { // remove last
				list->tail = previous_node;
				previous_node->next = NULL;
			}
			else // remove in the middle
				previous_node->next = next_node;

			free(current_node->item);
			free(current_node);
			list->list_size -= 1;
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

int list_trim_range(list_t *list, const size_t start, const size_t end) {
	if(!list) return -1;
	if(start > end || end > list->list_size) return -1;
	if(start == end) return 0;

	if(start == 0 && end == list->list_size){
		list_clear(list);
		return end;
	}
	if(start == 0)
		return list_trim_front(list, end);

	int count = 0;

	// we must find left_node and right_node
	node_t* left_node = NULL;
	node_t* right_node = NULL;
	node_t* current_node = list->head;
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
		list->tail = left_node;
	}

	list->list_size -= (end - start);
	return end - start;
}