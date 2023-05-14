#include "testing.h"
#include <stdio.h>

bool real_test_assert(const char *message, bool test_result,
                      const char *file, int line) {
    if (test_result) {
        printf("%s:%d: OK - %s\n", file, line, message);
    } else {
        printf("%s:%d: FAILED - %s\n", file, line, message);
    }
    fflush(stdout);
    return test_result;
}