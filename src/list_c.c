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
	node_t *cur = list->head;
	while(cur){
		node_t* next = cur->next;
		free(cur->item);
		free(cur);
		cur = next;
	}
	list->head = NULL;
	list->tail = NULL;
	list->list_size = 0;
}

void list_delete(list_t **list){
	if (!list || !(*list)) return;
	// if(*list == NULL) return;

	// clean items
	node_t *cur = (*list)->head;
	while(cur != NULL){
		node_t* next = cur->next;
		free(cur->item);
		free(cur);
		cur = next;
	}
	
	// delete list
	free(*list);
	(*list) = NULL;
}

int list_append(list_t *list, const void *item){
	if(!list || !item) return -1;

	// try to allocate memory for new node
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	if(!new_node) return -1;

	// try to allocate memory for new item
	new_node->item = malloc(list->item_size);
	if(!new_node->item){
		free(new_node);
		return -1;
	} 
	memcpy(new_node->item, item, list->item_size);
	new_node->next = NULL;

	// if list is empty
	if(!list->head){
		list->head = new_node;
	    list->tail = new_node;
	}
	else{ // list size is greater than 0
		list->tail->next = new_node;
		list->tail = new_node;
	}
	list->list_size += 1;
	return 0;
}

int list_prepend(list_t *list, const void *item){
	if(!list || !item) return -1;

	// try to allocate memory for new node
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	if(!new_node) return -1;

	// try to allocate memory for new item
	new_node->item = malloc(list->item_size);
	if(!new_node->item){
		free(new_node);
		return -1;
	}
	memcpy(new_node->item, item, list->item_size);
	new_node->next = NULL;

	// if list is empty
	if(!list->head){
		list->head = new_node;
	    list->tail = new_node;
	}
	else{ // list size is greater than 0
		node_t *tmp = list->head;
		list->head = new_node;
		list->head->next = tmp;
	}

	list->list_size += 1;
	return 0;
}

int list_insert(list_t *list, const void *item, const size_t pos){
	if(!list || !item) return -1;
	if(pos > list->list_size) return -1;

	size_t count = 0;
	node_t *cur = list->head;
	node_t *prev = NULL;

	while(cur){
		if(count == pos){
			if(cur == list->head){
				list_prepend(list, item);
				return 0;
			}
			else{
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
				prev->next = new_node;
				new_node->next = cur;
				list->list_size += 1;
				return 0;
			}
		}
		count++;
		prev = cur;
		cur = cur->next;
	}

	if(pos == list->list_size){
		list_append(list, item);
		return 0;
	}
	return -1;
}

void* list_front(list_t *list){
	return (!list || list->list_size == 0) ? NULL : list->head->item;
}

void* list_back(list_t *list){
	return (!list || list->list_size == 0) ? NULL : list->tail->item;
}

void* list_remove(list_t *list, void* key, cmp_func_t cmp_func){
	if(!list || !key || !cmp_func) return NULL;

	node_t* ptr = list->head;
	node_t* prev; node_t *next;

	while(ptr){
		next = ptr->next;
		// check by comaprator
		if(cmp_func(ptr->item, key) == 0){
			if(ptr == list->head){ // remove first
				if(list->tail == list->head) // if only one item in list
					list->head = list->tail = NULL;	
				else list->head = next;
			}
			else if(ptr == list->tail) { // remove last
				list->tail = prev;
				prev->next = NULL;
			}
			else prev->next = next; // remove in the middle
			
			void* item = malloc(list->item_size);
			memcpy(item, ptr->item, list->item_size);

			free(ptr->item);
			free(ptr);

			list->list_size -= 1;
			return item;
		}

		prev = ptr;
		ptr = ptr->next;
	}

	return NULL;
}

int list_remove_all(list_t *list, void* key, cmp_func_t cmp_func){
	if(!list || !key || !cmp_func) return -1;

	int rem_count = 0;
	node_t* ptr = list->head;
	node_t* prev; node_t *next;

	while(ptr){
		next = ptr->next;
		// check by comaprator
		if(cmp_func(ptr->item, key) == 0)
		{
			if(ptr == list->head){ // remove first
				if(list->head == list->tail) // if only one item in list
					list->head = list->tail = NULL;
				else list->head = next;
			}
			else if(ptr == list->tail) { // remove last
				list->tail = prev;
				prev->next = NULL;
			}
			else prev->next = next; // remove in the middle
			
			free(ptr->item); 
			free(ptr);
			list->list_size -= 1;
			rem_count++;
			ptr = next;
		}
		else{
			prev = ptr;
			ptr = ptr->next;
		}
	}
	return rem_count;
}

void* list_pop_front(list_t *list){
	if(!list) return NULL;
	if(list->list_size == 0) return NULL;

	node_t *ptr = list->head;

	if(list->head == list->tail) // if only one item	
		list->head = list->tail = NULL;
	else{
		node_t *next = ptr->next;
		list->head = next;
	}

	void* item = malloc(list->item_size);
	memcpy(item, ptr->item, list->item_size);

	free(ptr->item);
	free(ptr);
	list->list_size -= 1;
	return item;
}

void* list_pop_back(list_t *list){
	if(!list) return NULL;
	if(list->list_size == 0) return NULL;

	node_t *cur = list->head;
	node_t *prev = NULL;

	while(cur){
		if(cur == list->tail){
			if(list->head == list->tail){ // if only one item
				list->head = list->tail = NULL;

			}
			else{
				list->tail = prev;
				// cut node connection with tail
				if(prev) prev->next = NULL; // prev is new tail node
			}
			
			void* item = malloc(list->item_size);
			memcpy(item, cur->item, list->item_size);

			free(cur->item);
			free(cur);

			list->list_size -= 1;
			return item;
		}

		prev = cur;
		cur = cur->next;
	}
	return NULL;
}

void* list_at(const list_t *list, const size_t index) {
	if(!list) return NULL;
	if(index >= list->list_size) return NULL;

	node_t* cur = list->head;
	int count = 0;
	while(cur != NULL) {
		if(index == count)
			return cur->item;
		cur = cur->next;
		count++;
	}
	return NULL;
}

list_t *list_copy(const list_t *list){
	if(!list) return NULL;

	list_t *copy = list_create(list->item_size);
	if(!copy) return NULL;
	if(list->list_size){
		node_t *cur = list->head;
		while(cur){
			list_append(copy, cur->item);
			cur = cur->next;
		}
	}
	return copy;
}

int list_contains(const list_t *list, const void* item, cmp_func_t cmp)
{
	if(!list || !item || !cmp) return -1;
	if(!list->item_size) return -1;

	size_t count = 0;
	if(list->list_size) {
		node_t *cur = list->head;
		while(cur != NULL) {
			if(cmp(cur->item, item) == 0)
			count++;
			cur = cur->next;
		}
	}
	return count;
}
