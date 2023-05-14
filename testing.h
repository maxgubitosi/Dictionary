#ifndef TESTING_H
#define TESTING_H

#include <stdbool.h>

#define test_assert(message, test_result) real_test_assert(message, test_result, __FILE__, __LINE__)

bool real_test_assert(const char *message, bool test_result, const char *file, int line);

#endif //TESTING_H
