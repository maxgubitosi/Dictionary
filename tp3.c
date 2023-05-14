#include "tp3.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// struct para kev-values individuales
typedef struct dictEntry{
  char* key;
  void* value;
} dictEntry_t;

// struct para array de punteros dictEntry
struct dictionary {
  dictEntry_t** entries;
  int size;
};

// funcion de hashing FNV-1a
int *FNV_hash(const char *key) {
  const uint32_t FNV_OFFSET_BASIS = 2166136261U;
  const uint32_t FNV_PRIME = 16777619U;
  const uint8_t *data = (const uint8_t *) key;
  uint32_t hash = FNV_OFFSET_BASIS;
  size_t len = strlen(key);
  for (size_t i = 0; i < len; i++) {
      hash ^= data[i];
      hash *= FNV_PRIME;
  }
  return hash;
}
// deberia probar distintas func de hashing y evaluar cual me da mejores resultados

// creo un diccionario vacio, usando los structs ya definidos y mallocs, verificando que no haya errores
dictionary_t *dictionary_create(destroy_f destroy) { 
  dictionary_t* dict = (dictionary_t*) malloc(sizeof(dictionary_t));
  if (dict == NULL) {
    return NULL;
  }
  dict->entries = (dictEntry_t*) malloc(sizeof(dictEntry_t*));
  if (dict->entries == NULL) {
    return NULL;
  }
  dict->size = 0;
  return dict;
}

// inserto un par key-value en el diccionario, verificando que no haya errores
bool dictionary_put(dictionary_t *dictionary, const char *key, void *value) {
  int *hash = FNV_hash(key); // % dictionary->size;
  dictEntry_t* entry = (dictEntry_t*) malloc(sizeof(dictEntry_t));
  if (entry == NULL) {
    return false;
  }
  dictionary->entries[*hash] = entry;
  dictionary->entries[*hash]->key = key;
  dictionary->entries[*hash]->value = value;
  dictionary->size++;
  return true;
}
// primera versión, no maneja colisiones


void *dictionary_get(dictionary_t *dictionary, const char *key, bool *err) {
  return NULL;
};

bool dictionary_delete(dictionary_t *dictionary, const char *key) {
  return true;
};

void *dictionary_pop(dictionary_t *dictionary, const char *key, bool *err) {
  return NULL;
};

bool dictionary_contains(dictionary_t *dictionary, const char *key) {
  return true;
};

size_t dictionary_size(dictionary_t *dictionary) { return 0; };

void dictionary_destroy(dictionary_t *dictionary){};
