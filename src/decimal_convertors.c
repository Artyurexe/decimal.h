#include <math.h>

#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int flag = 0;
  // if (abs(src) > UINT_MAX) { // always false
  //   flag = -1;
  // }
  init_decimal(dst);
  if (src < 0) {
    src = src * (-1);
    set_1_bit(&dst->bits[3], 31);
  }
  dst->bits[0] = src;
  return flag;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int flag = 0;
  if (!src.bits[1] && !src.bits[2]) {
    if (get_sign(src)) {
      *dst = -1 * (int)(src.bits[0] / pow(10, get_exp(src)));
    } else {
      *dst = (int)(src.bits[0] / pow(10, get_exp(src)));
    }
  } else {
    flag = 1;
  }
  return flag;
}
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int flag = 0;
  int sign = 0;
  init_decimal(dst);
  if (src < 0) {
    src = src * (-1);
    sign = 1;
  }
  long int src_tmp = (long int)src;
  long int exp = 0;
  while (src - ((float)src_tmp / (long int)(pow(10, exp))) != 0) {
    exp++;
    src_tmp = src * (long int)(pow(10, exp));
  }
  s21_from_int_to_decimal(src_tmp, dst);
  dst->bits[3] += exp << 16;
  if (sign) set_1_bit(&dst->bits[3], 31);
  return flag;
}
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  double temp = (double)*dst;
  for (int i = 0; i < 96; i++) {
    temp += find_bit(src, i) * pow(2, i);
  }
  temp = temp * pow(10, -get_scale(src));
  if (find_bit(src, 127)) temp = temp * (-1);
  *dst = temp;
  return 0;
}
