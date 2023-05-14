#include <stdbool.h>
#include <stddef.h>
#include "tp3.h"

/*
 * Punto extra de internal iterator, suma 1 punto como máximo.
 */

#ifndef UDESA_ITERABLE_DICT_H
#define UDESA_ITERABLE_DICT_H

/*
 * La función de iteración recibe para cada paso de la iteración la clave, el valor
 * y un extra que puede usar para guardar resultados auxiliares.
 * Mientras no devuelva true la iteración continua
 */
typedef bool (*iterate_f)(const char* key, void* value, void* extra);

/*
 * Itera cada par clave-valor del diccionario pasandoselo a la función de iteración.
 * Recibe un parámetro extra que puede contener cualquier cosa para permitirle a la función guardar resultados.
 */
void iterate(dictionary_t* dict, iterate_f f, void* extra);

#endif //UDESA_ITERABLE_DICT_H
