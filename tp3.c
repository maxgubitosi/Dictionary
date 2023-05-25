#include "tp3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TABLE_SIZE 7
#define RES_FACT 3
#define LOAD_FACT 0.75

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

bool rehash(dictionary_t *dictionary) {
  if (dictionary == NULL) return false;
  uint32_t newCapacity = dictionary->capacity * RES_FACT;
  dictEntry_t **newEntries = (dictEntry_t **)calloc(sizeof(dictEntry_t *), newCapacity);
  if (!newEntries) return false;

  for (uint32_t i = 0; i < dictionary->capacity; i++) {
    dictEntry_t *entry = dictionary->entries[i];
    while (entry) {
      dictEntry_t *nextEntry = entry->next; // Store the next entry before reassigning
      uint32_t hash = FNV_hash(entry->key) % newCapacity;
      entry->next = newEntries[hash];
      newEntries[hash] = entry;
      entry = nextEntry; // Move to the next entry
    }
  }

  // Update the values in the newEntries array
  for (uint32_t i = 0; i < newCapacity; i++) {
    dictEntry_t *entry = newEntries[i];
    while (entry) {
      // Reassign the value associated with the key
      bool err = false;
      void *value = dictionary_get(dictionary, entry->key, &err);
      if (!err) {
        entry->value = value;
      } else {
        // Handle the error case where the key is not found in the original dictionary
        // You can decide how to handle this situation based on your application's logic
        // printf("Error: Key %s not found in original dictionary\n", entry->key);
        
        // no se bien que es este error
      }
      entry = entry->next;
    }
  }

  // Free the memory of the old entries array
  free(dictionary->entries);

  // Update the dictionary's entries pointer and capacity
  dictionary->entries = newEntries;
  dictionary->capacity = newCapacity;

  printf("\n===========rehashing: capacity = %u ===========\n", dictionary->capacity);
  return true;
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

bool dictionary_put(dictionary_t *dictionary, const char *key, void *value) {
  if (strlen(key) == 0 || dictionary == NULL)
    return false;

  // si el size del diccionario está a >=0.75 del capacity hago rehash
  if (dictionary->size >= dictionary->capacity * LOAD_FACT) {
    if (!rehash(dictionary)) return false;
  }

  uint32_t hash = dictIndex(dictionary, key);
  dictEntry_t *entry = dictionary->entries[hash];

  // busco si ya existe la key
  while (entry) {
    if (strcmp(entry->key, key) == 0) {
      // si existe la key, libero el valor anterior y le asigno el nuevo
      free(entry->value);
      entry->value = value;
      return true;
    }
    entry = entry->next;
  }

  // si no existe la key, creo un nuevo entry
  dictEntry_t *newEntry = (dictEntry_t *)malloc(sizeof(dictEntry_t));
  if (!newEntry)
    return false;

  newEntry->key = malloc(strlen(key) + 1);
  if (!newEntry->key) {
    free(newEntry);
    return false;
  }
  strcpy((char *)newEntry->key, key);
  newEntry->value = value;
  newEntry->next = NULL;

  // Add the new entry at the beginning of the chain
  newEntry->next = dictionary->entries[hash];
  dictionary->entries[hash] = newEntry;

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

  if (!entry) {
    *err = true;
    return NULL;
  }

  while (entry) {
    if (strcmp(entry->key, key) == 0) {
      *err = false;
      return entry->value;
    }
    entry = entry->next;
  }

  *err = true;
  return NULL;
}


bool dictionary_delete(dictionary_t *dictionary, const char *key) {
  bool err = false;
  void *value = dictionary_pop(dictionary, key, &err);
  if (!value) return false;
  free(value);
  return true;
}



void *dictionary_pop(dictionary_t *dictionary, const char *key, bool *err) {
  if (err == NULL) {
    // Handle the case where the error pointer is NULL
    return NULL;
  }

  if (dictionary == NULL) {
    *err = true;
    return NULL;
  }

  uint32_t hash = dictIndex(dictionary, key);
  dictEntry_t *entry = dictionary->entries[hash];
  dictEntry_t *prevEntry = NULL;

  while (entry) {
    if (strcmp(entry->key, key) == 0) {
      void *value = entry->value;

      if (prevEntry) {
        prevEntry->next = entry->next;
      } else {
        dictionary->entries[hash] = entry->next;
      }

      free((char *)entry->key);
      free(entry);
      dictionary->size--;
      *err = false;
      return value;
    }

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
  dictEntry_t *entry = dictionary->entries[hash];
  while (entry) {
    if (strcmp(entry->key, key) == 0) {
      return true;
    }
    entry = entry->next;
  }
  return false;
}

// alternativa: puedo llamar a dictionary_get() o vice-versa
// tiene mas sentido al revés, esta función es mas simple: la llamo en el get: si devuelve false cierra y sino hace todo el resto

size_t dictionary_size(dictionary_t *dictionary) {
  return dictionary->size;
}

void dictionary_destroy(dictionary_t *dictionary) {
  for (uint32_t i = 0; i < dictionary->capacity; i++) {
    dictEntry_t* entry = dictionary->entries[i];
    while (entry != NULL) {
      dictEntry_t* nextEntry = entry->next;
      free((char*)entry->key);
      free(entry->value);
      free(entry);
      entry = nextEntry;
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