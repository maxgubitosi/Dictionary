#include "tp3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TABLE_SIZE 2000
#define LOAD_FACTOR 0.75
// #define RES_FACT 5

// struct para kev-values individuales
  typedef struct dictEntry{
  const char *key;
  void *value;
} dictEntry_t;

// struct para array de punteros dictEntry
 struct dictionary {
  dictEntry_t **entries;
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

// funcion de Bernstein hashing
uint32_t Bernstein_hash(const char *key) {
  uint32_t hash = 5381;
  const uint8_t *data = (const uint8_t *)key;
  size_t len = strlen(key);
  for (size_t i = 0; i < len; i++) {
    hash = (hash * 33) ^ data[i];
  }
  return hash;
}

// funcion auxiliar que devuelve el índice, recibe una funcion de hashing
static uint32_t dictIndex(dictionary_t* dict, const char* key, uint32_t (*f_hash)(const char*) ) {
  uint32_t hash = f_hash(key) % dict->capacity;
  return hash;
}

// creo un diccionario vacio, usando los structs ya definidos y mallocs, verificando que no haya errores
dictionary_t *dictionary_create(destroy_f destroy) { 
  dictionary_t *dict = (dictionary_t*) malloc(sizeof(dictionary_t));
  if (!dict) return NULL;
  dict->size = 0;
  dict->capacity = TABLE_SIZE;

  dict->entries = (dictEntry_t**) calloc(sizeof(dictEntry_t*), dict->capacity);
  if (!dict->entries) {
    free(dict);  // hace falta esto?
    return NULL;
  }
  return dict;
}

// todavía pueden haber mejoras aca. falta rehashing
bool dictionary_put(dictionary_t *dictionary, const char *key, void *value) {
  if (strlen(key) == 0 || dictionary == NULL || key == NULL) return false;
  
  // necesito rehashing?
  if (dictionary->size >= dictionary->capacity * LOAD_FACTOR) {
    return false;
  }

  uint32_t hash = dictIndex(dictionary, key, FNV_hash);
  uint32_t d_hash = dictIndex(dictionary, key, Bernstein_hash);

  while (dictionary->entries[hash]) {
    if (strcmp(dictionary->entries[hash]->key, key) == 0) {
      free(dictionary->entries[hash]->value); // 
      dictionary->entries[hash]->value = value;
      return true;
    }
    hash = (hash + d_hash) % dictionary->capacity; 
  }

  dictEntry_t* newEntry = (dictEntry_t*) malloc(sizeof(dictEntry_t));
  if (!newEntry) return false;
  // newEntry->key = key;
  newEntry->key = (char *)malloc(strlen(key) + 1); //
  if (!newEntry->key) {
    free(newEntry);
    return false;
  }
  strcpy((char *)newEntry->key, key); //
  newEntry->value = value;
  dictionary->entries[hash] = newEntry;
  dictionary->size++;
  return true;
}

void *dictionary_get(dictionary_t *dictionary, const char *key, bool *err) {
  if (strlen(key) == 0 || dictionary == NULL) {
    *err = true;
    return NULL;
  }
  uint32_t hash = dictIndex(dictionary, key, FNV_hash);
  dictEntry_t *entry = dictionary->entries[hash];
  if (entry && strcmp(entry->key, key) == 0) {
    *err = false;
    return entry->value;
  }

  uint32_t d_hash = dictIndex(dictionary, key, Bernstein_hash);
  uint32_t rehashed_hash = (hash + d_hash) % dictionary->capacity;

  while (rehashed_hash != hash) {
    entry = dictionary->entries[rehashed_hash];
    if (entry && strcmp(entry->key, key) == 0) {
      *err = false;
      return entry->value;
    }
    rehashed_hash = (rehashed_hash + d_hash) % dictionary->capacity;
  }

  *err = true;
  return NULL;
}

bool dictionary_delete(dictionary_t *dictionary, const char *key) {
  if (strlen(key) == 0 || !dictionary) return false;
  bool err;
  void *value = dictionary_pop(dictionary, key, &err);
  if (err) return false; 
  free(value);
  return true;
}

void *dictionary_pop(dictionary_t *dictionary, const char *key, bool *err) {
  if (strlen(key) == 0 || dictionary == NULL) {
    *err = true;
    return NULL;
  }
  uint32_t hash = dictIndex(dictionary, key, FNV_hash);
  uint32_t d_hash = dictIndex(dictionary, key, Bernstein_hash);
  uint32_t rehashed_hash = hash;

  while (true) {
    dictEntry_t *entry = dictionary->entries[rehashed_hash];
    if (entry == NULL) {
      *err = true;
      return NULL;  
    }
    if (strcmp(entry->key, key) == 0) {
      void *value = entry->value;
      // free((char *)entry->value); // // mal
      free((char *)entry->key); //
      free(entry);
      dictionary->entries[rehashed_hash] = NULL;
      dictionary->size--;
      *err = false;
      return value;
    }
    rehashed_hash = (rehashed_hash + d_hash) % dictionary->capacity;
    if (rehashed_hash == hash) {
      return NULL;
    }
  }

  *err = true;
  return NULL;
}


bool dictionary_contains(dictionary_t *dictionary, const char *key) {
  if (strlen(key) == 0 || dictionary == NULL) {
    return false;
  }
  uint32_t hash = dictIndex(dictionary, key, FNV_hash);
  dictEntry_t *entry = dictionary->entries[hash];
  if (entry && strcmp(entry->key, key) == 0) {
    return true;
  }
  return false;
}


size_t dictionary_size(dictionary_t *dictionary) {
  return dictionary->size;
}

void dictionary_destroy(dictionary_t *dictionary) {
 for (uint32_t i = 0; i < dictionary->capacity; i++) {
    dictEntry_t* entry = dictionary->entries[i];
    if (entry != NULL) {
      free((char *)entry->key); //
      free(entry);
    }
  }
  free(dictionary->entries);
  free(dictionary);
}

