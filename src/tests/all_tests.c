#include "tests.h"

int main(void) {
  Suite *s_comparison, *s_converters, *s_calc, *s_rounding;
  SRunner *runner;

  s_comparison = comparison_suite();
  s_converters = convertors_suite();
  s_calc = calc_suite();
  s_rounding = rounding_suite();
  runner = srunner_create(s_comparison);
  srunner_add_suite(runner, s_converters);
  srunner_add_suite(runner, s_calc);
  srunner_add_suite(runner, s_rounding);
  srunner_run_all(runner, CK_ENV);
  srunner_free(runner);
  return 0;
}