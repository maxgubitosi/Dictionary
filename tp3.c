#include "tp3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defino parametros globales
#define INITIAL_TABLE_SIZE 7 // comienzo con 7 para los casos de testeo con 5 iteraciones
#define RES_FACT 2 // nros mas grandes sacrifican complejidad espacial por mejor complejidad temporal
#define LOAD_FACT 0.75 // proporcion comunmente usada y eficiente
// nota: la versión actual no busca minimizar ninguna de las complejidades mencionadas 

// struct para entradas: key-values individuales
  typedef struct dictEntry{
  const char *key;
  void *value;
  struct dictEntry *next;
} dictEntry_t;

// struct para array de punteros dictEntry
struct dictionary {
  dictEntry_t **entries;
  uint32_t size;
  uint32_t capacity;
  destroy_f destroy;
};

// funcion auxiliar para imprimir el diccionario
void dictionary_print(dictionary_t *dictionary) {
  if (dictionary == NULL) {
    printf("NULL\n");
    return;
  }
  for (uint32_t i = 0; i < dictionary->capacity; i++) {
    dictEntry_t *entry = dictionary->entries[i];
    printf("Index %u: ", i);
    while (entry) {
      printf("{Key: %s, Value: %d} ", entry->key, *((int*)entry->value));
      entry = entry->next;
    }
    printf("\n");
  }
}

// funcion de hashing FNV-1a (copiada de internet)
uint32_t FNV_hash(const char *key) {
  uint32_t FNV_OFFSET_BASIS = 2166136261U;
  uint32_t FNV_PRIME = 16777619U;
  uint8_t *data = (uint8_t *) key;
  uint32_t hash = FNV_OFFSET_BASIS;
  size_t len = strlen(key);
  for (size_t i = 0; i < len; i++) {
      hash ^= data[i];
      hash *= FNV_PRIME;
  }
  return hash;
}
// deberia probar distintas func de hashing y evaluar cual me da mejores resultados
// cambiar la hash function y las varaibles globales deberían tener gran impacto 
// sobre tanto la eficiencia temporal como de memoria

// funcion auxiliar que devuelve el índice
static uint32_t dictIndex(dictionary_t* dict, const char* key) {
  uint32_t hash = FNV_hash(key) % dict->capacity;
  return hash;
}


/* funcion de rehashing, tiene en cuenta los parametros globales definidos
 * recorre todos los entries del diccionario original y las reasigna en newEntries 
 * calculado los hashes, con la nueva capacidad del diccionario
 * recorre newEntries y para cada entry busca el value original usando dictionary_get y lo asigna
 * libera la memoria del array antiguo y actualiza el puntero entries del diccionario para apuntar a newEntries
 * reutiliza codigo: función get
*/
bool rehash(dictionary_t *dictionary) {
  if (!dictionary) return false;
  uint32_t newCapacity = dictionary->capacity * RES_FACT;
  dictEntry_t **newEntries = (dictEntry_t **) calloc(sizeof(dictEntry_t *), newCapacity);
  if (!newEntries) return false;

  for (uint32_t i = 0; i < dictionary->capacity; i++) {
    dictEntry_t *entry = dictionary->entries[i];
    while (entry) {
      dictEntry_t *nextEntry = entry->next; 
      uint32_t hash = FNV_hash(entry->key) % newCapacity;
      entry->next = newEntries[hash];
      newEntries[hash] = entry;
      entry = nextEntry; 
    }
  }

  for (uint32_t i = 0; i < newCapacity; i++) {
    dictEntry_t *entry = newEntries[i];
    while (entry) {
      bool err = false;
      void *value = dictionary_get(dictionary, entry->key, &err);
      if (!err) entry->value = value;
      entry = entry->next;
    }
  }

  free(dictionary->entries);
  dictionary->entries = newEntries;
  dictionary->capacity = newCapacity;
  // printf("===========rehashing: capacity = %u ===========\n", dictionary->capacity);
  return true;
}
// nota: no usé la función put en esta implementación para evitar que las funciones se llamen mutuamente


// creo un diccionario vacio, usando los structs ya definidos y mallocs, verificando que no haya errores
dictionary_t *dictionary_create(destroy_f destroy) { 
  dictionary_t* dict = (dictionary_t*) malloc(sizeof(dictionary_t));
  if (!dict) return NULL;
  dict->size = 0;
  dict->capacity = INITIAL_TABLE_SIZE;
  dict->destroy = destroy;
  dict->entries = (dictEntry_t**) calloc(sizeof(dictEntry_t*), dict->capacity);
  if (!dict->entries) {
    free(dict); 
    return NULL;
  }
  return dict;
}

// uso chaining en el caso de colisiones (agrego la nueva entrada al principio de la lista enlazada)
bool dictionary_put(dictionary_t *dictionary, const char *key, void *value) {
  if (strlen(key) == 0 || dictionary == NULL)
    return false;

  // chequea si necesita rehashear o no
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

  // agrego nuevas entradas al principio de la lista en casos de colision 
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


// reutiliza codigo: función pop
bool dictionary_delete(dictionary_t *dictionary, const char *key) {
  bool err = false;
  void *value = dictionary_pop(dictionary, key, &err);
  if (!value) return false;
  if (dictionary->destroy) free(value);
  return true;
}


void *dictionary_pop(dictionary_t *dictionary, const char *key, bool *err) {
  if (err == NULL) return NULL;
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


// reutiliza código: función get
bool dictionary_contains(dictionary_t *dictionary, const char *key) {
  if (strlen(key) == 0 || dictionary == NULL) return false;
  bool err = false;
  void *value = dictionary_get(dictionary, key, &err);
  return (!err && value != NULL);
}


size_t dictionary_size(dictionary_t *dictionary) {
  return dictionary->size;
}


/* 
 * reutiliza código: función delete
 * bucle for recorre todas las entries
 * ciclo while recorre las listas enlazadas 
 * nota: solo en los casos de colisiones hay mas de un elemento
 * en cada una de estas iteraciones llama a la función delete 
*/
void dictionary_destroy(dictionary_t *dictionary) {
  if (!dictionary) return;
  // dictionary_print(dictionary);
  
  for (uint32_t i = 0; i < dictionary->capacity; i++) {
    dictEntry_t* entry = dictionary->entries[i];
    
    while (entry) {
      dictEntry_t *nextEntry = entry->next;
      dictionary_delete(dictionary, entry->key);
      entry = nextEntry;
    }
  }

  free(dictionary->entries);
  free(dictionary);
}
