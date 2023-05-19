#include "test_malloc.h"
#include "testing.h"
#include "queryable_dict.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_create_and_destroy_qdict() {
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    queryable_dictionary_t *dict = qdictionary_create(free);
    tests_result &= test_assert("El diccionario fue creado", dict != NULL);
    qdictionary_destroy(dict);
    return tests_result;
}

// Acá implementa tus tests

int main_queryable_dict(void) {
  srand(117);
  int return_code = 0;
  return_code += !test_create_and_destroy_qdict();
  return_code += 1; // Implementa tus propios tests acá
  exit(return_code);
}
