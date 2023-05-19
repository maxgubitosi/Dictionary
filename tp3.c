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
  if (strlen(key) == 0 || dictionary == NULL || key == NULL) {
    return false;
  }
  uint32_t hash = FNV_hash(key) % dictionary->capacity;
  dictEntry_t* entry = (dictEntry_t*) malloc(sizeof(dictEntry_t));
  if (entry == NULL) {
    return false;
  }
  if (dictionary->entries[hash] != NULL) {  // chequear: no se si se usa 0 o NULL
    return false; // caso a desarrollar: colision
  }
  entry->key = key;
  entry->value = value;
  dictionary->entries[hash] = entry;
  dictionary->size++;
  // print_dict(dictionary);
  return true;
}
// primera versión, no maneja colisiones


void *dictionary_get(dictionary_t *dictionary, const char *key, bool *err) {
  if (strlen(key) == 0 || dictionary == NULL) {
    *err = true;
    return NULL;
  }
  uint32_t hash = FNV_hash(key) % dictionary->capacity;
  if (dictionary->entries[hash] == NULL || dictionary->entries[hash]->key == NULL) {
    *err = true;
    return NULL;
  }
  *err = false;
  return dictionary->entries[hash]->value;
}


bool dictionary_delete(dictionary_t *dictionary, const char *key) {
  if (strlen(key) == 0 || dictionary == NULL) {
    return false;
  }
  uint32_t hash = FNV_hash(key) % dictionary->capacity;
  if (dictionary->entries[hash] == NULL) {
    return false;
  }
  free(dictionary->entries[hash]);
  dictionary->size--;
  return true;
}

void *dictionary_pop(dictionary_t *dictionary, const char *key, bool *err) {
  if (strlen(key) == 0 || dictionary == NULL) {
    *err = true;
    return NULL;
  }
  uint32_t hash = FNV_hash(key) % dictionary->capacity;
  if (dictionary->entries[hash] == NULL) {
    *err = true;
    return NULL;
  }
  *err = false;
  void* value = dictionary->entries[hash]->value;
  dictionary->entries[hash]->value = NULL; //
  dictionary->entries[hash]->key = NULL; // estas dos líneas o free
  // free(dictionary->entries[hash]);
  dictionary->size--;
  return value;
}



bool dictionary_contains(dictionary_t *dictionary, const char *key) {
  if (strlen(key) == 0 || dictionary == NULL) {
    return false;
  }
  uint32_t hash = FNV_hash(key) % dictionary->capacity;
  if (dictionary->entries[hash] == NULL) {
    return false;
  }
  return true;
}

size_t dictionary_size(dictionary_t *dictionary) {
  return dictionary->size;
}

void dictionary_destroy(dictionary_t *dictionary) {
 for (uint32_t i = 0; i < dictionary->capacity; i++) {    // no debería usar el size en vez del capacity? creo que esto tiene sentido porque uso calloc para inicializar el array
    dictEntry_t* entry = dictionary->entries[i];
    if (entry != NULL) {
      free(dictionary->entries[i]); 
    }
  }
  free(dictionary->entries);
  free(dictionary);
}

