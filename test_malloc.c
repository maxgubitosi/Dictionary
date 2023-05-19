#include "test_malloc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

bool malloc_working = true;

void *__real_malloc(size_t size);

void* __wrap_malloc(size_t size, const void *caller){
    if(malloc_working) {
        return __real_malloc(size);
    } else {
        return NULL;
    }
}

void set_malloc_status(bool status){
    malloc_working = status;
}