#include "tp3.h"
#include "queryable_dict.h"

/*
 * Punto extra de queryable dict, suma 3 puntos como máximo.
 */

struct queryable_dictionary {
    dictionary_t* dict;
};

struct dict_item {
    char* key;
    void* value;
};

/* Obtiene las claves del diccionario en el mismo orden en el que fueron insertadas.
 * Pre-condiciones:
 * - El diccionario existe
 * Post-condiciones:
 * - Retorna un vector de punteros a claves del mismo largo que el largo del diccionario.
 * Estas claves son copias de las originales.
 */
char** get_keys(queryable_dictionary_t* dict){
    return NULL;
}

/* Obtiene los valores del diccionario en el mismo orden en el que fueron insertados.
 * Pre-condiciones:
 * - El diccionario existe
 * - La función de copia existe
 * Post-condiciones:
 * - Retorna un vector de punteros a valores del mismo largo que el largo del diccionario.
 * Estos valores son copias de los del diccionario.
 */
void** get_values(queryable_dictionary_t* dict, copy_value_f copy){
    return NULL;
}

/* Obtiene las claves y valores del diccionario en el mismo orden en el que fueron insertados.
 * Pre-condiciones:
 * - El diccionario existe
 * - La función de copia existe
 * Post-condiciones:
 * - Retorna un vector de punteros a structs que contienen una copia de la clave y del valor.
 */
dict_item_t** get_items(queryable_dictionary_t* dict, copy_value_f copy){
    return NULL;
}

// Tienen el mismo comportamiento que el diccionario común y SIGUEN SIENDO O(1)
queryable_dictionary_t *qdictionary_create(destroy_f destroy){
    return NULL;
}
bool qdictionary_put(queryable_dictionary_t *dictionary, const char *key, void *value){
    return false;
}
void *qdictionary_get(queryable_dictionary_t *dictionary, const char *key, bool *err){
    return NULL;
}
bool qdictionary_delete(queryable_dictionary_t *dictionary, const char *key){
    return false;
}
void *qdictionary_pop(queryable_dictionary_t* dictionary, const char *key, bool *err){
    return NULL;
}
bool qdictionary_contains(queryable_dictionary_t *dictionary, const char *key){
    return false;
}
size_t qdictionary_size(queryable_dictionary_t *dictionary){
    return 0;
}
void qdictionary_destroy(queryable_dictionary_t *dictionary){
    return;
}