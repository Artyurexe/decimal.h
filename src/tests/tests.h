#ifndef SRC_TESTS_TESTS_H
#define SRC_TESTS_TESTS_H

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../s21_decimal.h"
Suite *comparison_suite();
Suite *convertors_suite();
Suite *rounding_suite();
Suite *calc_suite();

#endif  // SRC_TESTS_TESTS_H
