#include <stdbool.h>
#include <stddef.h>
#include "tp3.h"

/*
 * Punto extra de operable dict, suma 3 puntos como máximo.
 */

#ifndef UDESA_OPERABLE_DICT_H
#define UDESA_OPERABLE_DICT_H

/*
 * Inserta o pisa en dictionary1 todas las claves y valores que están en dictionary2.
 * Las claves se mantienen independientes entre ambos diccionarios, pero los valores no.
 * (las claves nuevas de agregar dictionary2 en dictionary1 deben ser punteros distintos
 * pero los valores son el mismo puntero).
 * La operación es atómica, si falla dictionary1 queda en el mismo estado que estaba antes del update.
 * Devuelve true si funcionó, false si fayó.
 */
bool dictionary_update(dictionary_t *dictionary1, dictionary_t *dictionary2);

/*
 * Combina ambos diccionarios en uno nuevo que contiene solo las claves que están presentes en ambos.
 * En todos los casos se conservan los valores del dictionary1.
 * Devuelve NULL si falla.
 */
dictionary_t* dictionary_and(dictionary_t *dictionary1, dictionary_t *dictionary2);

/*
 * Combina ambos diccionarios en uno nuevo que contiene todas las claves de ambos.
 * En caso de que ambos tengan una misma clave se conserva el valor de dictionary1.
 * Devuelve NULL si falla.
 */
dictionary_t* dictionary_or(dictionary_t *dictionary1, dictionary_t *dictionary2);

/*
 * Devuelve true si ambos diccionarios son iguales. Dos diccionarios son iguales sí:
 *  - Las claves son iguales aunque puedan tener distinta posición en memoria.
 *  - Los values son los mismos punteros para cada clave
 *  - Tienen la misma cantidad de claves
 */
bool dictionary_equals(dictionary_t *dictionary1, dictionary_t *dictionary2);

#endif //UDESA_OPERABLE_DICT_H
