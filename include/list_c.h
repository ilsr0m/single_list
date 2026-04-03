#ifndef LIST_C_H
#define LIST_C_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct node node_t;
typedef struct list list_t;

/**
 * @brief Функция-компаратор, предаваемая в качестве аргумента
 * @param[in] _item Pointer to the item
 * @param[in] _key Pointer to the key
 * @return 0 - success
 * @return 1 - fail
 */
typedef  int (*cmp_func_t)(const void *_item, const void *_key);

/**
 * @brief Функция-предикат, предаваемая в качестве аргумента
 * @param[in] _item Pointer to the item
 * @param[in] _context Pointer to the context
 * @return Результат сравнения с контекстом:
 * @return 0 - success
 * @return 1 - fail
 */
typedef int (*predicate_fn)(const void* _item, void* _context);

/*---------------- C-like getters ----------------*/
/**
 * @brief Returns size of one-way linked list
 * @param[in] lst Pointer to the list
 * @return List's size
 */
size_t list_size(const list_t *lst);

/**
 * @brief Returns item size of one-way linked list
 * @param[in] lst Pointer to the list
 * @return Item's size
 */
size_t list_item_size(const list_t *lst);

node_t* list_head(const list_t *lst);
node_t* list_tail(const list_t *lst);

node_t *node_next(const node_t *nod);
void* node_data(const node_t *nod);

/**
 * @brief Функция возвращает первый элемент списка
 * @param[out] lst Pointer to the list
 * @return Указатель на первый элемент списка
 */
void* list_front_item(const list_t *lst);

/**
 * @brief Функция возвращает последний элемент списка
 * @param[out] lst Pointer to the list
 * @return Указатель на последний элемент списка
 */
void* list_back_item(const list_t *lst);

/**
 * @brief Получение указателя на элемент списка по заданому индексу
 * @param[in] lst Pointer to the list
 * @param[in] index Индекс узла
 * @return Указатель на узел, соответствующий размещению в списке по аргументу index.
 */
void* list_at(const list_t *lst, const size_t index);
/*------------------------------------------------*/

/**
 * @brief Функция создания односвязного линейного списка
 * @param[in] item_size Размер выделенной памяти для каждого элемента
 * @return Pointer to the list
 */
list_t* list_create(const size_t item_size);

/**
 * @brief Функция очистки односвязного списка
 * @param[in] lst Pointer to the list
 */
void list_clear(list_t *lst);

/**
 * @brief Функция удаления всего односвязного линейного списка
 * @param[out] lst Pointer to the list
 */
void list_delete(list_t **lst);

int list_empty(const list_t *lst);
int list_set(list_t *lst, size_t index, void* item);

/**
 * @brief Функция добавления нового элемента в конец односвязного линейного списка
 * @param[out] lst Pointer to the list
 * @param[in] item Указатель на элемент списка
 * @return Статус
 */
int list_append(list_t *lst, const void *item);

/**
 * @brief Функция добавления нового элемента в начало односвязного линейного списка
 * @param[out] lst Pointer to the list
 * @param[in] item Указатель на элемент списка
 * @return Статус
 */
int list_prepend(list_t *lst, const void *item);

/**
 * @brief Функция вставляет элемент списка в указанную позицию
 * @param[out] lst Pointer to the list
 * @param[in] item Указатель на вставляемый элемент
 * @param[in] position Номер нового элемента в списке
 * @return Статус
 */
int list_insert(list_t *lst, const void *item, const size_t position);

/**
 * @brief Функция удаления первого соответстующего элементов односвязного линейного списка
 * @param[out] lst Pointer to the list
 * @param[in] key Ключ сравнения, передаваемый в компаратор в качестве аргумента
 * @param[in] comparator Компаратор, сравнивающий ключ и текущий элемент списка. Если результат сравнения - 0, то узел будет удален
 * @return Количество удаленных элементов
 */
int list_remove(list_t *lst, void* key, cmp_func_t comparator); 

/**
 * @brief Remove item at set position in single-linked list
 * @param[out] lst Pointer to the list
 * @param[in] position Item's position
 * @return Function's result: -1 - fail, 1 - success, 
 */
int list_remove_at(list_t *lst, size_t position);

/**
 * @brief Функция удаления всех соответстующих элементов односвязного линейного списка
 * @param[out] lst Pointer to the list
 * @param[in] key Ключ сравнения, передаваемый в компаратор в качестве аргумента
 * @param[in] comparator Компаратор, сравнивающий ключ и текущий элемент списка. Если результат сравнения - 0, то узел будет удален
 * @return Количество удаленных элементов
 */
int list_remove_all(list_t *lst, void* key, cmp_func_t comparator); 

/**
 * @brief Функция удаления первого узла списка
 * @param[out] lst Pointer to the list
 */
void* list_pop_front(list_t *lst);

/**
 * @brief Функция удаления последнего узла списка
 * @param[out] lst Pointer to the list
 */
void* list_pop_back(list_t *lst); 

/**
 * @brief Формирование копии списка, указатель которого передается в аргумент
 * @param[in] lst Pointer to the list
 * @return Указатель на копию списка
 */
list_t *list_copy(const list_t *lst);

/**
 * @brief Функция удаления n-го количества элементов с начала односвязного линейного списка
 * @param[out] lst Pointer to the list
 * @param[in] n Количество удаляемых элементов
 * @return Количество удаленных элементов
 */
int list_trim_front(list_t *lst, const size_t n); // +

/**
 * @brief Функция удаления n-го количества элементов с конца односвязного линейного списка
 * @param[out] lst Pointer to the list
 * @param[in] n Количество удаляемых элементов
 * @return Количество удаленных элементов
 */
int list_trim_back(list_t *lst, const size_t n); // +

/**
 * @brief 
 * @param[out] lst Pointer to the list
 * @param[in] start 
 * @param[in] end 
 * @return 
 */
int list_trim_range(list_t *list, const size_t start, const size_t end);

/**
 * @brief 
 * @param[out] lst_destination 
 * @param[in] lst_source
 * @return 
 */
int list_concat(list_t *lst_destination, const list_t *lst_source); // копия lst_source


int list_splice(list_t *lst_destination, list_t *lst_source); // lst_source -> null

 /**
 * @brief 
 * @param[in] lst Pointer to the list
 * @param[in] key 
 * @param[in] comparator 
 * @return 
 */
int list_contains(const list_t *lst, const void *key, cmp_func_t comparator);

 /**
 * @brief 
 * @param[in] lst Pointer to the list
 * @param[in] key 
 * @param[in] comparator 
 * @return 
 */
int list_count(const list_t *lst, const void *key, cmp_func_t comparator);

 /**
 * @brief 
 * @param[in] lst Pointer to the list
 * @param[in] key 
 * @param[in] comparator 
 * @return 
 */
void* list_find(const list_t *lst, const void *key, cmp_func_t comparator);

/**
 * @brief 
 * @param[in] lst Pointer to the list
 * @param[in] predicate 
 * @param[in] context 
 * @return 
 */
list_t* list_filter(const list_t *lst, predicate_fn predicate, void *context);

#endif