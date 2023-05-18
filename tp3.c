#include "tp3.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#define TABLE_SIZE 7
// #define RES_FACT 5

// struct para kev-values individuales
typedef struct dictEntry{
  const char* key;
  void* value;
} dictEntry_t;

// struct para array de punteros dictEntry
struct dictionary {
  dictEntry_t** entries;
  uint32_t size;
  uint32_t capacity;
};

// funcion auxiliar para imprimir el diccionario
void print_dict(dictionary_t* dict) {
  for (uint32_t i = 0; i < dict->size; i++) {
    printf("key: %s, value: %s\n", dict->entries[i]->key, (char*) dict->entries[i]->value); //problema con el value por ser  void*
  }
}

// funcion de hashing FNV-1a
uint32_t FNV_hash(const char *key) {
  uint32_t FNV_OFFSET_BASIS = 2166136261U;
  uint32_t FNV_PRIME = 16777619U;
  uint8_t *data = (uint8_t *) key;
  uint32_t hash = FNV_OFFSET_BASIS;
  size_t len = strlen(key);
  for (size_t i = 0; i < len; i++) {
      hash ^= data[i];
      hash *= FNV_PRIME;
      // hash %= TABLE_SIZE;  // no hace falta, porque en la función de put() ya se hace
  }
  return hash;
}
// mejor no usar los __uint... conviene usar uint_t o size_t
// deberia probar distintas func de hashing y evaluar cual me da mejores resultados

// creo un diccionario vacio, usando los structs ya definidos y mallocs, verificando que no haya errores
dictionary_t *dictionary_create(destroy_f destroy) { 
  dictionary_t* dict = (dictionary_t*) malloc(sizeof(dictionary_t));
  if (dict == NULL) {
    return NULL;
  }
  dict->size = 0;
  dict->capacity = TABLE_SIZE;
  dict->entries = (dictEntry_t**) calloc(sizeof(dictEntry_t*), dict->capacity);
  if (dict->entries == NULL) {
    return NULL;
  }
  return dict;
}

// inserto un par key-value en el diccionario, verificando que no haya errores
bool dictionary_put(dictionary_t *dictionary, const char *key, void *value) {
  uint32_t hash = FNV_hash(key) % dictionary->size;
  dictEntry_t* entry = (dictEntry_t*) malloc(sizeof(dictEntry_t));
  if (entry == NULL) {
    return false;
  }
  if (dictionary->entries[hash] != NULL) {
    return false; // caso a desarrollar: colision
  }
  dictionary->entries[hash] = entry;
  dictionary->entries[hash]->key = key;
  dictionary->entries[hash]->value = value;
  dictionary->size++;
  print_dict(dictionary);
  return true;
}
// primera versión, no maneja colisiones


void *dictionary_get(dictionary_t *dictionary, const char *key, bool *err) {
  return NULL;
};

/* Elimina una clave del diccionario. O(1).
 * Pre-condiciones
 * - El diccionario existe
 * - La clave tiene largo mayor a cero
 * Retorna true si la clave estaba presente y se pudo eliminar, o false
 * de otro modo.
 */
bool dictionary_delete(dictionary_t *dictionary, const char *key) {
  return true;
}

void *dictionary_pop(dictionary_t *dictionary, const char *key, bool *err) {
  return NULL;
};

bool dictionary_contains(dictionary_t *dictionary, const char *key) {
  return true;
};

size_t dictionary_size(dictionary_t *dictionary) { return 0; };

void dictionary_destroy(dictionary_t *dictionary) {
  free(dictionary->entries);
  free(dictionary);
}
