#ifndef LIST_C_H
#define LIST_C_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/** @brief Структура одного узла односвязного линейного списка */
typedef struct node_t node_t;

/** @brief Структура одного узла односвязного линейного списка */
struct node_t{
	void *item; ///< Указатель на данные
	node_t* next; ///< Указатель на следующий по счету узел
};

/** @brief Структура односвязного линейного списка */
typedef struct{
	size_t size; ///< Количество элементов списка
	size_t item_size; ///< Размер выделенной памяти для каждого элемента
	node_t* head; ///< Указатель на первый узел списка
	node_t* tail; ///< Указатель на последний элемент списка
}list_t;

/**
 * @brief Функция-компаратор, предаваемая в качестве аргумента
 * @param[in] _item Указатель на сравниваемый элемент
 * @param[in] _key Указатель на ключ
 * @return Результат сравнения: 0 - прошел, остальное - не прошел
 */
typedef  int (*cmp_func_t)(const void *_item, const void *_key);

/**
 * @brief Функция-предикат, предаваемая в качестве аргумента
 * @param[in] _item Указатель на элемент
 * @param[in] _ctx Указатель на контекст
 * @return Результат сравнения с контекстом: 0 - прошел, остальное - не прошел
 */
typedef int (*predicate_fn)(const void * _item, void * _ctx);

/**
 * @brief Функция создания односвязного линейного списка
 * @param[in] item_size Размер выделенной памяти для каждого элемента
 * @return Указатель на список
 */
list_t* list_create(const size_t item_size);

/**
 * @brief Функция очистки односвязного списка
 * @param[in] list Указатель на список
 */
void list_clear(list_t *list);

/**
 * @brief Функция удаления всего односвязного линейного списка
 * @param[out] list Указатель на список
 */
void list_delete(list_t **list);

/**
 * @brief Функция добавления нового элемента в конец односвязного линейного списка
 * @param[out] list Указатель на список
 * @param[in] item Указатель на элемент списка
 * @return Статус
 */
int list_append(list_t *list, const void *item);

/**
 * @brief Функция добавления нового элемента в начало односвязного линейного списка
 * @param[out] list Указатель на список
 * @param[in] item Указатель на элемент списка
 * @return Статус
 */
int list_prepend(list_t *list, const void *item);

/**
 * @brief Функция вставляет элемент списка в указанную позицию
 * @param[out] list Указатель на список
 * @param[in] list Номер нового элемента в списке
 * @return Статус
 */
int list_insert(list_t *list, const void *item, const size_t pos);

/**
 * @brief Функция возвращает первый элемент списка
 * @param[out] list Указатель на список
 * @return Указатель на первый элемент списка
 */
void* list_front(list_t *list);

/**
 * @brief Функция возвращает последний элемент списка
 * @param[out] list Указатель на список
 * @return Указатель на последний элемент списка
 */
void* list_back(list_t *list);

/**
 * @brief Функция удаления первого соответстующего элементов односвязного линейного списка
 * @param[out] list Указатель на список
 * @param[in] key Ключ сравнения, передаваемый в компаратор в качестве аргумента
 * @param[in] cmp_func Компаратор, сравнивающий ключ и текущий элемент списка. Если результат сравнения - 0, то узел будет удален
 * @return Количество удаленных элементов
 */
void* list_remove(list_t *list, void* key, cmp_func_t cmp_func); 

/**
 * @brief Функция удаления всех соответстующих элементов односвязного линейного списка
 * @param[out] list Указатель на список
 * @param[in] key Ключ сравнения, передаваемый в компаратор в качестве аргумента
 * @param[in] cmp_func Компаратор, сравнивающий ключ и текущий элемент списка. Если результат сравнения - 0, то узел будет удален
 * @return Количество удаленных элементов
 */
int list_remove_all(list_t *list, void* key, cmp_func_t cmp_func); 

/**
 * @brief Функция удаления первого узла списка
 * @param[out] list Указатель на список
 */
void* list_pop_front(list_t *list);

/**
 * @brief Функция удаления последнего узла списка
 * @param[out] list Указатель на список
 */
void* list_pop_back(list_t *list); 

#endif