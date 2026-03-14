#include "list_c.h"

list_t* list_create(const size_t item_size){
	if(item_size == 0) return NULL;
	
	list_t* list = (list_t*)malloc(sizeof(list_t));
	if(!list) return NULL;

	list->size = 0;
	list->item_size = item_size;
	list->head = NULL;
	list->tail = NULL;

	return list;
}

void list_clear(list_t *list){
	if(list == NULL) return;
	// прописать очистку итемов
	node_t *cur = list->head;
	while(cur != NULL){
		node_t* next = cur->next;
		free(cur->item);
		free(cur);
		cur = next;
	}
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

void list_delete(list_t **list){
	if (list == NULL) return;
	if(*list == NULL) return;

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
	// new node pointer
	node_t* new_ptr = (node_t*)malloc(sizeof(node_t));
	if(new_ptr == NULL) return -1;
	new_ptr->item = malloc(list->item_size);
	if(new_ptr->item == NULL) return -1;
	memcpy(new_ptr->item, item, list->item_size);
	new_ptr->next = NULL;

	// if first item placed
	if(list->head == NULL){
		list->head = new_ptr;
	    list->tail = new_ptr;
	}
	else{ // if more than one
		list->tail->next = new_ptr;
		list->tail = new_ptr;
	}
	list->size += 1;
	return 0;
}

int list_prepend(list_t *list, const void *item){
	if(!list || !item) return -1;

	node_t* new_ptr = (node_t*)malloc(sizeof(node_t));
	if(new_ptr == NULL) return -1;
	new_ptr->item = malloc(list->item_size);
	if(new_ptr->item == NULL) return -1;
	memcpy(new_ptr->item, item, list->item_size);
	new_ptr->next = NULL;

	// создаем список с нуля
	if(list->head == NULL){
		list->head = new_ptr;
	    list->tail = new_ptr;
	}
	else{
		node_t *tmp = list->head;
		list->head = new_ptr;
		list->head->next = tmp;
	}

	list->size += 1;
	return 0;
}

int list_insert(list_t *list, const void *item, const size_t pos){
	if(!list || !item) return -1;
	if(pos > list->size) return -1;

	size_t count = 0;
	node_t *cur = list->head;
	node_t *prev = NULL;

	while(cur != NULL){
		if(count == pos){
			if(cur == list->head){
				list_prepend(list, item);
				return 0;
			}
			else{
				node_t* new_ptr = (node_t*)malloc(sizeof(node_t));
				new_ptr->item = malloc( list->item_size);
				memcpy(new_ptr->item, item, list->item_size);
				prev->next = new_ptr;
				new_ptr->next = cur;
				list->size += 1;
				return 0;
			}
		}
		count++;
		prev = cur;
		cur = cur->next;
	}

	if(pos == list->size){
		list_append(list, item);
		return 0;
	}
	return -1;
}

void* list_front(list_t *list){
	return (list == NULL || list->size == 0) ? NULL : list->head->item;
}

void* list_back(list_t *list){
	return (list == NULL || list->size == 0) ? NULL : list->tail->item;
}

void* list_remove(list_t *list, void* key, cmp_func_t cmp_func){
	if(!list || !key || !cmp_func) return NULL;

	node_t* ptr = list->head;
	node_t* prev; node_t *next;

	while(ptr != NULL){
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

			list->size -= 1;
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

	while(ptr != NULL){
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
			list->size -= 1;
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
	if(list->size == 0) return NULL;

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
	list->size -= 1;
	return item;
}

void* list_pop_back(list_t *list){
	if(!list) return NULL;
	if(list->size == 0) return NULL;

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

			list->size -= 1;
			return item;
		}

		prev = cur;
		cur = cur->next;
	}
	return NULL;
}