#include "tests.h"

// Функция s21_negate
START_TEST(s21_negate_test_1) {
  s21_decimal src = {{12, 12, 12, MINUS}};
  s21_decimal dest = {0};
  int ret = s21_negate(src, &dest);
  ck_assert_uint_eq(dest.bits[3], 0);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_negate_test_2) {
  s21_decimal src = {{12, 12, 12, 0}};
  s21_decimal dest = {0};
  int ret = s21_negate(src, &dest);
  ck_assert_uint_eq(dest.bits[3], MINUS);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(s21_floor_test_positive_number) {
  uint32_t exp = 1 << 16;
  s21_decimal value_1 = {{34, 0, 0, exp}};
  s21_decimal result = {0};
  int ret = s21_floor(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 3);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_negative_number) {
  uint32_t exp = 1 << 16;
  s21_decimal value_1 = {{34, 0, 0, MINUS | exp}};
  s21_decimal result = {0};
  int ret = s21_floor(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 4);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], MINUS);
}
END_TEST

START_TEST(s21_truncate_test) {
  uint32_t exp = 2 << 16;
  s21_decimal value_1 = {{342, 0, 0, MINUS | exp}};
  s21_decimal result = {0};
  int ret = s21_truncate(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 3);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], MINUS);
}
END_TEST

START_TEST(s21_round_test) {
  uint32_t exp = 2 << 16;
  s21_decimal value_1 = {{362, 0, 0, exp}};
  s21_decimal result = {0};
  int ret = s21_round(value_1, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 4);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

Suite *rounding_suite() {
  Suite *s;
  TCase *tc_rounding;

  s = suite_create("Comparison");

  tc_rounding = tcase_create("is equal");

  // ---------- rounding functions ----------

  suite_add_tcase(s, tc_rounding);
  tcase_add_test(tc_rounding, s21_negate_test_1);
  tcase_add_test(tc_rounding, s21_negate_test_2);
  tcase_add_test(tc_rounding, s21_floor_test_positive_number);
  tcase_add_test(tc_rounding, s21_floor_test_negative_number);
  tcase_add_test(tc_rounding, s21_truncate_test);
  tcase_add_test(tc_rounding, s21_round_test);

  return s;
}
