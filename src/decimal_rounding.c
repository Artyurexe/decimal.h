#include "s21_decimal.h"

// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result) {
  s21_truncate(value, result);
  if (get_bit(result->bits[3], 31))
    s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
  return 0;
}

// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result) {
  s21_truncate(value, result);
  if (get_last_fractional_part(value) > 4)
    s21_add(*result, (s21_decimal){{1, 0, 0, 0}}, result);
  return 0;
}

// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result) {
  s21_big_decimal try = norm_to_big(value);
  while (get_exp(big_to_norm(try))) pointright(&try);
  *result = big_to_norm(try);
  set_sign(result, get_sign(value));
  return 0;
}

// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  negate_bit(&result->bits[3], 31);
  return 0;
}
