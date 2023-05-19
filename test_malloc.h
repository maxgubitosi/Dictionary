#ifndef TEST_MALLOC_H
#define TEST_MALLOC_H
#include <stdlib.h>
#include <stdbool.h>

/*
 * Sets the malloc status, if true malloc works normally, else it fails.
 */
void set_malloc_status(bool status);

#endif //TEST_MALLOC_H
