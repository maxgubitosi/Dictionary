#include "test_malloc.h"
#include "testing.h"
#include "tp3.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_create_and_destroy_dict() {
  printf("========== %s ==========\n", __PRETTY_FUNCTION__);
  bool tests_result = true;
  dictionary_t *dict = dictionary_create(free);
  tests_result &= test_assert("El diccionario fue creado", dict != NULL);
  dictionary_destroy(dict);
  return tests_result;
}

bool test_create_failed() {
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    set_malloc_status(false);
    dictionary_t *dict = dictionary_create(free);
    set_malloc_status(true);
    tests_result &= test_assert("El diccionario no fue creado", dict == NULL);
    return tests_result;
}

bool test_create_dict_nodestroy() {
  printf("========== %s ==========\n", __PRETTY_FUNCTION__);
  bool tests_result = true;
  dictionary_t *dict = dictionary_create(NULL);
  tests_result &= test_assert("El diccionario fue creado", dict != NULL);
  dictionary_destroy(dict);
  return tests_result;
}

bool test_put_malloc_fail() {
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    dictionary_t *dict = dictionary_create(NULL);
    tests_result &= test_assert("El tamaño es cero", dictionary_size(dict) == 0);
    int one = 1, two = 2;
    tests_result &=
            test_assert("Se puede insertar key1", dictionary_put(dict, "key1", &one));
    tests_result &= test_assert("El diccionario contiene key1",
                                dictionary_contains(dict, "key1"));
    tests_result &= test_assert("El diccionario no contiene key2",
                                !dictionary_contains(dict, "key2"));
    tests_result &= test_assert("El tamaño es uno", dictionary_size(dict) == 1);
    set_malloc_status(false);
    tests_result &=
            test_assert("No se puede insertar key2", !dictionary_put(dict, "key2", &two));
    set_malloc_status(true);
    tests_result &= test_assert("El diccionario contiene key1",
                                dictionary_contains(dict, "key1"));
    tests_result &= test_assert("El diccionario no contiene key2",
                                !dictionary_contains(dict, "key2"));
    tests_result &= test_assert("El tamaño es dos", dictionary_size(dict) == 1);
    dictionary_destroy(dict);
    return tests_result;
}

bool test_put_size() {
  printf("========== %s ==========\n", __PRETTY_FUNCTION__);
  bool tests_result = true;
  dictionary_t *dict = dictionary_create(NULL);
  tests_result &= test_assert("El tamaño es cero", dictionary_size(dict) == 0);
  int one = 1, two = 2;
  tests_result &=
      test_assert("Se puede insertar key1", dictionary_put(dict, "key1", &one));
  tests_result &= test_assert("El diccionario contiene key1",
                              dictionary_contains(dict, "key1"));
  tests_result &= test_assert("El diccionario no contiene key2",
                              !dictionary_contains(dict, "key2"));
  tests_result &= test_assert("El tamaño es uno", dictionary_size(dict) == 1);
  tests_result &=
      test_assert("Se puede insertar key2", dictionary_put(dict, "key2", &two));

  tests_result &= test_assert("El diccionario contiene key1",
                              dictionary_contains(dict, "key1"));
  tests_result &= test_assert("El diccionario contiene key2",
                              dictionary_contains(dict, "key2"));
  tests_result &= test_assert("El tamaño es dos", dictionary_size(dict) == 2);
  dictionary_destroy(dict);
  return tests_result;
}

bool test_pop_get() {
  printf("========== %s ==========\n", __PRETTY_FUNCTION__);
  bool tests_result = true;
  dictionary_t *dict = dictionary_create(NULL);
  int one = 1;
  bool err;
  dictionary_put(dict, "key0", &one);
  tests_result &= test_assert("Obtener key0 retorna el puntero correcto",
                              dictionary_get(dict, "key0", &err) == &one);
  tests_result &= test_assert("No hay error", !err);
  tests_result &= test_assert("Obtener key0 y comparar valor",
                              *(int *)dictionary_get(dict, "key0", &err) == 1);
  tests_result &= test_assert("No hay error", err == false);
  tests_result &= test_assert("Hacer pop de key0",
                              dictionary_pop(dict, "key0", &err) == &one);
  tests_result &= test_assert("No hay error", err == false);
  tests_result &=
      test_assert("key0 es NULL", dictionary_get(dict, "key0", &err) == NULL);
  tests_result &= test_assert("Hay error", err == true);
  dictionary_destroy(dict);
  return tests_result;
}

bool test_get_errcode() {
  printf("========== %s ==========\n", __PRETTY_FUNCTION__);
  bool tests_result = true;
  dictionary_t *dict = dictionary_create(NULL);
  int one = 1;
  bool err;
  void *retval = dictionary_get(dict, "key1", &err);
  tests_result &= test_assert("Se retorna NULL", retval == NULL);
  tests_result &= test_assert("Se marca error al obtener", err == true);
  tests_result &=
      test_assert("Se inserta el uno", dictionary_put(dict, "key1", &one));
  retval = dictionary_get(dict, "key1", &err);
  tests_result &= test_assert("Se retorna one", retval == &one);
  tests_result &= test_assert("El valor es correcto", *(int *)retval == 1);
  tests_result &= test_assert("No hay error al obtener", err == false);
  dictionary_destroy(dict);
  return tests_result;
}

bool test_put_get_delete_loop() {
  printf("========== %s ==========\n", __PRETTY_FUNCTION__);
  bool tests_result = true;
  dictionary_t *dict = dictionary_create(free);
  for (size_t i = 0; i < 5; i++) {
    char *key = malloc(sizeof("key"));
    strcpy(key, "key");
    int *value0 = malloc(sizeof(int));
    *value0 = 0;
    int *value = malloc(sizeof(int));
    *value = 1;
    bool err = true;
    tests_result &= test_assert("No existe la clave key",
                                NULL == dictionary_get(dict, key, &err));
    tests_result &= test_assert("Err es true", err == true);
    err = false;
    tests_result &= test_assert("No existe la clave key",
                                NULL == dictionary_get(dict, key, &err));
    tests_result &= test_assert("Err es true", err == true);

    tests_result &= test_assert("Se puede insertar key-value",
                                dictionary_put(dict, key, value0));
    tests_result &= test_assert("Se puede re-insertar key-value",
                                dictionary_put(dict, key, value));

    err = true;
    void *result = dictionary_get(dict, key, &err);
    tests_result &= test_assert("No hay error al hacer get", !err);
    tests_result &=
        test_assert("El puntero es correcto", (int *)result == value);
    tests_result &=
        test_assert("El valor del puntero es correcto", *(int *)result == 1);

    err = false;
    result = dictionary_get(dict, key, &err);
    tests_result &= test_assert("No hay error al hacer get", !err);
    tests_result &= test_assert("El valor es correcto", (int *)result == value);

    err = true;
    result = dictionary_pop(dict, key, &err);
    tests_result &= test_assert("No hay error al hacer pop", !err);
    tests_result &=
        test_assert("El valor de pop es correcto", (int *)result == value);
    tests_result &= test_assert("El valor contenido en el puntero es correcto",
                                *(int *)result == 1);

    result = dictionary_pop(dict, key, &err);
    tests_result &= test_assert("Hay error al hacer pop", err);
    tests_result &= test_assert("Result is NULL", result == NULL);

    tests_result &= test_assert("Se puede insertar de nuevo",
                                dictionary_put(dict, key, value));
    tests_result &= test_assert("Se puede eliminar el par clave-valor",
                                dictionary_delete(dict, key));
    free(key);
  }
  dictionary_destroy(dict);
  return tests_result;
}

bool test_put_NULL() {
  printf("========== %s ==========\n", __PRETTY_FUNCTION__);
  bool tests_result = true;
  dictionary_t *dict = dictionary_create(NULL);
  tests_result &= test_assert("Se puede insertar key con valor NULL",
                              dictionary_put(dict, "key0", NULL));
  bool err;
  void *result = dictionary_get(dict, "key0", &err);
  tests_result &= test_assert("No hay error", !err);
  tests_result &= test_assert("El valor es NULL", result == NULL);
  dictionary_destroy(dict);
  return tests_result;
}

bool test_insert_random_sequence(size_t n, unsigned int seed, bool delete) {
  printf("========== %s (n=%lu, seed=%u, delete=%s) ==========\n",
         __PRETTY_FUNCTION__, n, seed, delete ? "true" : "false");
  srand(seed);
  bool tests_result = true;
  dictionary_t *dict = dictionary_create(free);
  bool insert = true;
  bool size_correct = true;
  for (size_t i = 0; i < n; i++) {
    int random_number = rand();
    int length = snprintf(NULL, 0, "%d", random_number);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", random_number);
    int *random_number_copy = malloc(sizeof(int));
    *random_number_copy = random_number;
    insert &= dictionary_put(dict, str, random_number_copy);
    size_correct &= (dictionary_size(dict) == i + 1);
    free(str);
  }
  tests_result &= test_assert("Todas las inserciones fueron exitosas", insert);
  tests_result &= test_assert("El tamaño fue siempre correcto", size_correct);

  srand(seed);
  bool contains = true;
  bool correct = true;
  for (size_t i = 0; i < n; i++) {
    int random_number = rand();
    int length = snprintf(NULL, 0, "%d", random_number);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", random_number);
    contains &= dictionary_contains(dict, str);
    bool err;
    void *retval = dictionary_get(dict, str, &err);
    correct &= (retval && *(int *)retval == random_number);
    free(str);
  }

  tests_result &= test_assert("Todas las claves están presentes", contains);
  tests_result &= test_assert("Todos los valores son correctos", correct);

  if (delete) {
    srand(seed);
    bool delete_ok = true;
    bool delete_size_ok = true;
    for (size_t i = 0; i < n; i++) {
      int random_number = rand();
      int length = snprintf(NULL, 0, "%d", random_number);
      char *str = malloc(length + 1);
      snprintf(str, length + 1, "%d", random_number);
      bool this_delete_ok = dictionary_delete(dict, str);
      if (!this_delete_ok) {
        printf("No se pudo eliminar la clave %s\n", str);
      }
      delete_ok &= this_delete_ok;
      bool this_delete_size_ok = dictionary_size(dict) == (n - i - 1);
      if (!this_delete_size_ok) {
        printf(
            "El tamaño luego de borrar es erróneo: se esperaban %lu pero hay "
            "%lu\n",
            n - i - 1, dictionary_size(dict));
      }
      delete_size_ok &= this_delete_size_ok;
      free(str);
    }
    tests_result &=
        test_assert("Todos los deletes fueron correctos", delete_ok);
    tests_result &= test_assert("Todos los tamaños al borrar fueron correctos",
                                delete_size_ok);
  }

  dictionary_destroy(dict);
  return tests_result;
}

bool test_malloc_fail_create() {
  printf("========== %s ==========\n", __PRETTY_FUNCTION__);
  bool tests_result = true;
  set_malloc_status(false);
  dictionary_t *dict = dictionary_create(NULL);
  tests_result &= test_assert(
      "La creación del diccionario falló y se retorna NULL", dict == NULL);
  set_malloc_status(true);
  return tests_result;
}

bool test_fail_insert() {
  printf("========== %s ==========\n", __PRETTY_FUNCTION__);
  bool tests_result = true;
  set_malloc_status(true);
  dictionary_t *dict = dictionary_create(free);
  set_malloc_status(false);
  bool insert = dictionary_put(dict, "key", NULL);
  test_assert("No se pudo insertar", !insert);
  tests_result &= !insert;
  set_malloc_status(true);
  dictionary_destroy(dict);
  return tests_result;
}

int main(void) {
  srand(117);
  int return_code = 0;
  return_code += !test_create_and_destroy_dict();
  return_code += !test_create_failed();
  return_code += !test_create_dict_nodestroy();
  return_code += !test_put_malloc_fail();
  return_code += !test_put_size();
  return_code += !test_pop_get();
  return_code += !test_get_errcode();
  return_code += !test_put_get_delete_loop();
  return_code += !test_put_NULL();
  return_code += !test_insert_random_sequence(512, 117, false);
  return_code += !test_insert_random_sequence(512, 117, true);
  return_code += !test_insert_random_sequence(2048, 117, false);
  return_code += !test_insert_random_sequence(2048, 117, true);
  return_code += !test_insert_random_sequence(65536, 117, false);
  return_code += !test_insert_random_sequence(65536, 117, true);
  return_code += !test_malloc_fail_create();
  return_code += !test_fail_insert();
  return return_code;
}
