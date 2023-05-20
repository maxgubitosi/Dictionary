#include "tp3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TABLE_SIZE 100
// #define RES_FACT 5

// struct para kev-values individuales
  struct dictEntry{
  const char *key;
  void *value;
  struct dictEntry *next;
};

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
// deberia probar distintas func de hashing y evaluar cual me da mejores resultados

// funcion auxiliar que devuelve el índice
static uint32_t dictIndex(dictionary_t* dict, const char* key) {
  uint32_t hash = FNV_hash(key) % dict->capacity;
  return hash;
}

// creo un diccionario vacio, usando los structs ya definidos y mallocs, verificando que no haya errores
dictionary_t *dictionary_create(destroy_f destroy) { 
  dictionary_t* dict = (dictionary_t*) malloc(sizeof(dictionary_t));
  if (!dict) return NULL;
  dict->size = 0;
  dict->capacity = TABLE_SIZE;
  dict->entries = (dictEntry_t**) calloc(sizeof(dictEntry_t*), dict->capacity);
  if (!dict->entries) {
    free(dict); 
    return NULL;
  }
  return dict;
}


// inserto un par key-value en el diccionario, verificando que no haya errores
bool dictionary_put(dictionary_t *dictionary, const char *key, void *value) {
  if (strlen(key) == 0 || dictionary == NULL) return false;
  uint32_t hash = dictIndex(dictionary, key);

  dictEntry_t* newEntry = (dictEntry_t*) malloc(sizeof(dictEntry_t));
  if (!newEntry) return false;

  newEntry->key = key;
  newEntry->value = value;
  newEntry->next = NULL;

  // chaining
  if (!dictionary->entries[hash]) {
    dictionary->entries[hash] = newEntry;
  } else {
    dictEntry_t* prev_entry = dictionary->entries[hash];
    while (prev_entry->next) {
      prev_entry = prev_entry->next;
    }
    prev_entry->next = newEntry;
  }
  dictionary->size++;
  return true;
}

void *dictionary_get(dictionary_t *dictionary, const char *key, bool *err) {
  if (strlen(key) == 0 || dictionary == NULL) {
    *err = true;
    return NULL;
  }
  uint32_t hash = dictIndex(dictionary, key);
  dictEntry_t *entry = dictionary->entries[hash];
  dictEntry_t *lastEntry = NULL; 
  if (!entry) {
    *err = true;
    return NULL;
  }

  while (entry) {
    if (strcmp(entry->key, key) == 0) {
      lastEntry = entry;  
    }
    entry = entry->next;
  }

  if (lastEntry) {
    *err = false;
    return lastEntry->value;
  } else {
    *err = true;
    return NULL;
  }
}

bool dictionary_delete(dictionary_t *dictionary, const char *key) {
  bool err = false;
  void *value = dictionary_pop(dictionary, key, &err);
  if (!value) return false;
  return true;
}

void *dictionary_pop(dictionary_t *dictionary, const char *key, bool *err) {
  if (strlen(key) == 0 || dictionary == NULL) {
    *err = true;
    return NULL;
  }
  uint32_t hash = dictIndex(dictionary, key);
  dictEntry_t *entry = dictionary->entries[hash];
  dictEntry_t *prevEntry = NULL;
  dictEntry_t *lastEntry = NULL;

  while (entry) {
    if (strcmp(entry->key, key) == 0) {
      if (prevEntry == NULL) {
        dictionary->entries[hash] = entry->next;
      } else {
        prevEntry->next = entry->next;
      }
      void *value = entry->value;
      free(entry);
      dictionary->size--;
      *err = false;
      if (lastEntry) {
        while (lastEntry->next) {
          lastEntry = lastEntry->next;
        }
        return lastEntry->value;
      } else {
        return value;
      }
    }
    lastEntry = prevEntry;
    prevEntry = entry;
    entry = entry->next;
  }
  *err = true;
  return NULL;
}




bool dictionary_contains(dictionary_t *dictionary, const char *key) {
  if (strlen(key) == 0 || dictionary == NULL) {
    return false;
  }
  uint32_t hash = dictIndex(dictionary, key);
  if (!dictionary->entries[hash]) return false;
  return true;
}
// alternativa: puedo llamar a dictionary_get() o vice-versa
// tiene mas sentido al revés, esta función es mas simple: la llamo en el get: si devuelve false cierra y sino hace todo el resto

size_t dictionary_size(dictionary_t *dictionary) {
  return dictionary->size;
}

void dictionary_destroy(dictionary_t *dictionary) {
 for (uint32_t i = 0; i < dictionary->capacity; i++) {
    dictEntry_t* entry = dictionary->entries[i];
    if (entry != NULL) {
      // free((char *) dictionary->entries[i]->key); // esto va con las 3 lineas del put() da un leak de 5 bytes
      free(dictionary->entries[i]); 
    }
  }
  free(dictionary->entries);
  free(dictionary);
}

/* duda/ idea: el error en test_put_get_delete_loop puede ser que venga del orden
 * en el que se insertan valores en la lista cuando hay colisiones
 * puede ser que quieran que inserte el nuevo primero y mover los ultimos o al revés
 * lo mismo pasa con el get, si voy a buscar a una key, devuelvo el primero que encuentro o el último?
 * creo que lo que tiene sentido es agregar el último al final de la lista y buscar siempre el ultimo
*/