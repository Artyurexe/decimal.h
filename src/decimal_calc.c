#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  bool sign_1 = get_bit(value_1.bits[3], 31);
  bool sign_2 = get_bit(value_2.bits[3], 31);
  value_1.bits[3] &= ~(1 << 31);
  value_2.bits[3] &= ~(1 << 31);
  int flag = (get_exp(value_1) > 28 || get_exp(value_2) > 28);
  if (flag) return 2;
  s21_big_decimal try_1 = norm_to_big(value_1);
  s21_big_decimal try_2 = norm_to_big(value_2);
  flag = scale_normalize(&try_1, &try_2);
  if (!flag) {
    if (sign_1 == sign_2) {
      flag = add(try_1, try_2, result);
      set_sign(result, sign_1);
    } else if (s21_is_less(value_1, value_2)) {
      sub(try_2, try_1, result);
      set_sign(result, sign_2);
    } else {
      sub(try_1, try_2, result);
      set_sign(result, sign_1);
    }
  }
  return (flag == 0) ? 0 : flag + get_bit(result->bits[3], 31);
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  bool sign_1 = get_bit(value_1.bits[3], 31);
  bool sign_2 = get_bit(value_2.bits[3], 31);
  value_1.bits[3] &= ~(1 << 31);
  value_2.bits[3] &= ~(1 << 31);
  int flag = (check_exp(value_1) || check_exp(value_1));
  if (flag) return 2;
  s21_big_decimal try_1 = norm_to_big(value_1);
  s21_big_decimal try_2 = norm_to_big(value_2);
  flag = scale_normalize(&try_1, &try_2);
  if (!flag) {
    if (sign_1 == sign_2) {
      if (s21_is_less(value_1, value_2)) {
        sub(try_2, try_1, result);
        set_sign(result, !sign_1);
      } else {
        sub(try_1, try_2, result);
        set_sign(result, sign_1);
      }
    } else {
      flag = add(try_1, try_2, result);
      set_sign(result, sign_1);
    }
  }
  return (flag == 0) ? 0 : flag + get_bit(result->bits[3], 31);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int ret = 0;
  init_decimal(result);

  bool check_1 = is_null(value_1);
  bool check_2 = is_null(value_2);

  if (check_1 || check_2) return 0;

  bool sign_1 = get_sign(value_1);
  bool sign_2 = get_sign(value_2);

  s21_big_decimal copy_1 = norm_to_big(value_1);
  s21_big_decimal copy_2 = norm_to_big(value_2);

  ret = mul(copy_1, copy_2, result);

  bool sign = sign_1 ^ sign_2;
  set_sign(result, sign);

  return ret;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int ret = 0;
  *result = (s21_decimal){0};

  bool check_1 = is_null(value_1);
  bool check_2 = is_null(value_2);

  if (check_1) return 0;

  if (check_2) return 3;

  return ret;
}