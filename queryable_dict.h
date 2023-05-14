#include <stdbool.h>
#include <stddef.h>
#include "tp3.h"

/*
 * Punto extra de queryable dict, suma 3 puntos como máximo.
 */

#ifndef UDESA_QUERYABLE_DICT_H
#define UDESA_QUERYABLE_DICT_H

struct queryable_dictionary;
typedef struct queryable_dictionary queryable_dictionary_t;

struct dict_item;
typedef struct dict_item dict_item_t;

typedef void* (*copy_value_f)(void *);

/* Obtiene las claves del diccionario en el mismo orden en el que fueron insertadas.
 * Pre-condiciones:
 * - El diccionario existe
 * Post-condiciones:
 * - Retorna un vector de punteros a claves del mismo largo que el largo del diccionario.
 * Estas claves son copias de las originales.
 */
char** dictionary_get_keys(queryable_dictionary_t* dict);

/* Obtiene los valores del diccionario en el mismo orden en el que fueron insertados.
 * Pre-condiciones:
 * - El diccionario existe
 * - La función de copia existe
 * Post-condiciones:
 * - Retorna un vector de punteros a valores del mismo largo que el largo del diccionario.
 * Estos valores son copias de los del diccionario.
 */
void** dictionary_get_values(queryable_dictionary_t* dict, copy_value_f copy);

/* Obtiene las claves y valores del diccionario en el mismo orden en el que fueron insertados.
 * Pre-condiciones:
 * - El diccionario existe
 * - La función de copia existe
 * Post-condiciones:
 * - Retorna un vector de punteros a structs que contienen una copia de la clave y del valor.
 */
dict_item_t** dictionary_get_items(queryable_dictionary_t* dict, copy_value_f copy);

// Tienen el mismo comportamiento que el diccionario común y SIGUEN SIENDO O(1)
queryable_dictionary_t *qdictionary_create(destroy_f destroy);
bool qdictionary_put(queryable_dictionary_t *dictionary, const char *key, void *value);
void *qdictionary_get(queryable_dictionary_t *dictionary, const char *key, bool *err);
bool qdictionary_delete(queryable_dictionary_t *dictionary, const char *key);
void *qdictionary_pop(queryable_dictionary_t* dictionary, const char *key, bool *err);
bool qdictionary_contains(queryable_dictionary_t *dictionary, const char *key);
size_t qdictionary_size(queryable_dictionary_t *dictionary);
void qdictionary_destroy(queryable_dictionary_t *dictionary);


#endif //UDESA_QUERYABLE_DICT_H
