#include "list_c.h"

list_t* list_create(const size_t item_size)
{
	assert(item_size > 0);
	
	if(item_size == 0) return NULL;
	
	list_t* list = (list_t*)malloc(sizeof(list_t));
	if(!list) return NULL;

	list->size = 0;
	list->item_size = item_size;
	list->head = NULL;
	list->tail = NULL;

	return list;
}

void list_clear(list_t *list)
{
	assert(list != NULL);
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

void list_delete(list_t **list)
{
	assert((*list) != NULL);

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

int list_append(list_t *list, const void *item)
{
	assert(list != NULL);
	assert(item != NULL);
	if(!list || !item) return -1;
	// new node pointer
	node_t* new_ptr = (node_t*)malloc(sizeof(node_t));
	new_ptr->item = malloc(list->item_size);
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

int list_prepend(list_t *list, const void *item)
{
	assert(list != NULL);
	assert(item != NULL);

	if(!list || !item) return -1;

	node_t* new_ptr = (node_t*)malloc(sizeof(node_t));
	new_ptr->item = malloc( list->item_size);
	memcpy(new_ptr->item, item, list->item_size);
	new_ptr->next = NULL;

	// создаем список с нуля
	if(list->head == NULL)
	{
		list->head = new_ptr;
	    list->tail = new_ptr;
	}
	else
	{
		node_t *tmp = list->head;
		list->head = new_ptr;
		list->head->next = tmp;
	}

	list->size += 1;
	return 0;
}

int list_insert(list_t *list, const void *item, const size_t pos)
{
	// some stuff to prevent monkey coders from monkey code
	assert(list != NULL);
	assert(item != NULL);
	assert(list->size >= pos);

	if(!list || !item) return -1;
	if(pos > list->size) return -1;

	size_t count = 0;
	node_t *cur = list->head;
	node_t *prev = NULL;

	while(cur != NULL)
	{
		if(count == pos)
		{
			if(cur == list->head)
			{
				list_prepend(list, item);
				return 0;
			}
			else
			{
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

	if(pos == list->size)
	{
		list_append(list, item);
		return 0;
	}
	return -1;
}

void* list_front(list_t *list)
{
	// some stuff to prevent monkey coders from monkey code
	assert(list != NULL);
	assert(list->size > 0);

	if(list == NULL || list->size == 0) 
		return NULL;
	return list->head->item;
}

void* list_back(list_t *list)
{
	// some stuff to prevent monkey coders from monkey code
	assert(list != NULL);
	assert(list->size > 0);

	if(list == NULL || list->size == 0) 
		return NULL;	
	return list->tail->item;
}

void* list_remove(list_t *list, void* key, cmp_func_t cmp_func)
{
	// some stuff to prevent monkey coders from monkey code
	assert(list != NULL);
	assert(key != NULL);
	assert(cmp_func != NULL);

	if(!list || !key || !cmp_func) return NULL;

	node_t* ptr = list->head;
	node_t* prev; node_t *next;

	while(ptr != NULL){
		next = ptr->next;
		// находим соответствие по компаратору
		if(cmp_func(ptr->item, key) == 0)
		{
			if(ptr == list->head) // remove first
				list->head = next;
			else if(ptr == list->tail) // remove last
			{
				list->tail = prev;
				prev->next = NULL;
			}
			else // remove in the middle
				prev->next = next;

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

int list_remove_all(list_t *list, void* key, cmp_func_t cmp_func)
{
	// some stuff to prevent monkey coders from monkey code
	assert(list != NULL);
	assert(key != NULL);
	assert(cmp_func != NULL);

	if(!list || !key || !cmp_func) return -1;

	int rem_count = 0;
	node_t* ptr = list->head;
	node_t* prev; node_t *next;

	while(ptr != NULL)
	{
		next = ptr->next;
		// находим соответствие по предикату
		if(cmp_func(ptr->item, key) == 0)
		{
			if(ptr == list->head){ // remove first
				list->head = next;
			}
			else if(ptr == list->tail) // remove last
			{
				list->tail = prev;
				prev->next = NULL;
			}
			else{ // remove in the middle
				prev->next = next;
			}

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

void* list_pop_front(list_t *list)
{
	// some stuff to prevent monkey coders from monkey code
	assert(list != NULL);
	assert(list->size > 0);

	if(!list) return NULL;
	if(list->size == 0) return NULL;

	node_t *ptr = list->head;
	node_t *next = ptr->next;
	list->head = next;

	void* item = malloc(list->item_size);
	memcpy(item, ptr->item, list->item_size);

	free(ptr->item);
	free(ptr);
	list->size -= 1;
	return item;
}

void* list_pop_back(list_t *list)
{
	// some stuff against monkey coders
	assert(list != NULL);
	assert(list->size > 0);

	if(!list) return NULL;
	if(list->size == 0) return NULL;

	node_t *cur = list->head;
	node_t *prev = NULL;

	while(cur)
	{
		if(cur == list->tail)
		{
			list->tail = prev;

			// cut node connection with tail
			// prev is new tail node
			if(prev)
				prev->next = NULL;

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