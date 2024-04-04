#include "s21_decimal.h"

int s21_is_equal(s21_decimal src1, s21_decimal src2) {
  int i;
  for (i = 0; i < 3 && src1.bits[i] == 0 && src2.bits[i] == 0; i++)
    ;
  if (i == 3) return 1;

  if (get_sign(src1) != get_sign(src2)) return 0;

  s21_big_decimal tmp1 = norm_to_big(src1);
  s21_big_decimal tmp2 = norm_to_big(src2);

  scale_normalize(&tmp1, &tmp2);
  for (i = 0; i < 7; i++) {
    if (tmp1.bits[i] != tmp2.bits[i]) return 0;
  }
  return 1;
}

int s21_is_not_equal(s21_decimal src1, s21_decimal src2) {
  return !s21_is_equal(src1, src2);
}

int s21_is_less(s21_decimal src1, s21_decimal src2) {
  if (s21_is_equal(src1, src2)) return 0;

  if (get_sign(src1) > get_sign(src2)) return 1;

  if (get_sign(src1) < get_sign(src2)) return 0;

  int sign = get_sign(src1);
  s21_big_decimal tmp1 = norm_to_big(src1);
  s21_big_decimal tmp2 = norm_to_big(src2);
  scale_normalize(&tmp1, &tmp2);

  int result = 0;
  if (sign) result = 1;
  for (int i = 2; i >= 0; i--) {
    if (tmp1.bits[i] > tmp2.bits[i]) return result;
    if (tmp1.bits[i] < tmp2.bits[i]) return !result;
  }
  return result;
}

int s21_is_less_or_equal(s21_decimal src1, s21_decimal src2) {
  return s21_is_less(src1, src2) || s21_is_equal(src1, src2);
}

int s21_is_greater(s21_decimal src1, s21_decimal src2) {
  return !s21_is_less_or_equal(src1, src2);
}
int s21_is_greater_or_equal(s21_decimal src1, s21_decimal src2) {
  return s21_is_greater(src1, src2) || s21_is_equal(src1, src2);
}
