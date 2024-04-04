#include "s21_decimal.h"

int get_scale(s21_decimal dst) {
  int mask = 127 << 16;
  int scale = (mask & dst.bits[3]) >> 16;
  return scale;
}

int find_bit(s21_decimal dst, int index) {
  int mask = 1u << (index % 32);
  return (dst.bits[index / 32] & mask) != 0;
}

// Если экспонента в децимале больше 28 то возвращает 1, иначе возвращает 0
bool check_exp(s21_decimal src) { return (get_exp(src) > 28); }

// получаем бит из переданного инта по индексу (возможные индексы от 0 до 31)
bool get_bit(unsigned int src, unsigned int index) {
  return (src >> index) & 1;
}

// устанавливаем бит на 1 по указанному индексу
void set_1_bit(unsigned int *src, unsigned int index) { *src |= (1 << index); }

// устанавливаем бит на 0 по указанному индексу
void set_0_bit(unsigned int *src, unsigned int index) { *src &= ~(1 << index); }

// Функция инвертации бита
void negate_bit(unsigned int *src, unsigned int index) { *src ^= 1 << index; }

// проверяем децимал на знак
bool get_sign(s21_decimal src) { return (get_bit(src.bits[3], 31)); }

void set_sign(s21_decimal *src, bool sign) { src->bits[3] |= sign ? MINUS : 0; }

// Проверка мантиссы на ноль
bool is_null(s21_decimal src) {
  return (src.bits[0] == 0 && src.bits[1] == 0 && src.bits[2] == 0);
}

// получаем из децимала значение экспаненты (степени десятки для вычисления
// дробного числа) в десятичном виде
unsigned int get_exp(s21_decimal src) { return (src.bits[3] & EXP) >> 16; }

// Устанавливаем в децимал значение экспоненты ввзятое из десятичного вида
void set_exp(s21_decimal *value, unsigned int exp) {
  value->bits[3] |= (exp << 16);
}

// Прибавляет к экспоненте децимала значение экспаненты аргумента и помещает в
// dest. Возвращает 1 при итоговой сумме больше 28
// bool add_exp(s21_decimal src, s21_decimal *dest, unsigned int exp) {
//   unsigned int new_exp = get_exp(src) + exp;
//   if (new_exp > 28) return 1;
//   set_exp(dest, new_exp);
//   return 0;
// }

// Проверяет число четное оно или нет
bool is_even(unsigned int num) { return get_bit(num, 0) ? 0 : 1; }

// Конвертирует из биг децимала в обычный
s21_decimal big_to_norm(s21_big_decimal src) {
  s21_decimal res = {0};
  for (int i = 0; i < 3; i++) res.bits[i] = src.bits[i] & UINT_MAX;
  set_exp(&res, (uint32_t)src.sexp);
  return res;
}

// Наоборот
s21_big_decimal norm_to_big(s21_decimal src) {
  s21_big_decimal res = {0};
  unsigned int exp = get_exp(src);
  for (int i = 0; i < 3; i++) res.bits[i] = src.bits[i] & UINT_MAX;
  res.sexp = exp;
  return res;
}

// Функция переполнения
bool getoverflow(s21_big_decimal *value) {
  uint32_t overflow = 0;
  for (int i = 0; i < 7; i++) {
    value->bits[i] += overflow;
    overflow = value->bits[i] >> 32;
    value->bits[i] &= UINT_MAX;
  }
  if (is_overflow(*value)) overflow = 1;
  return overflow;
}

// Функция сдвига влево ( то есть домножения на 10 ), если переполнение,
// возвращает 1
bool pointleft(s21_big_decimal *value) {
  s21_big_decimal copy = *value;
  bool ret = 0;
  for (int i = 0; i < 7; i++) {
    copy.bits[i] *= 10;
  }
  copy.sexp++;
  ret = getoverflow(&copy);
  if (!ret && value->sexp < 28) *value = copy;
  return ret;
}

// Функция сдвига вправо ( то есть деления на 10 )
uint32_t pointright(s21_big_decimal *value) {
  uint16_t remainder = 0;
  if (get_exp(big_to_norm(*value)) > 0) {
    s21_big_decimal copy = *value;
    for (int i = 6; i >= 0; i--) {
      copy.bits[i] += (uint64_t)remainder << 32;
      remainder = copy.bits[i] % 10;
      copy.bits[i] /= 10;
    }
    copy.sexp--;
    *value = copy;
  }
  return remainder;
}

// Проверяет большой децимал на переполнение
bool is_overflow(s21_big_decimal value) {
  return (value.bits[3] || value.bits[4] || value.bits[5] || value.bits[6]);
}

// Функция банковского округления
uint32_t bank_round(uint32_t value, uint16_t remainders[], uint16_t index) {
  if (remainders[index] > 5) {
    return value + 1;
  } else if (remainders[index] < 5) {
    return value;
  } else {
    while (index > 0) {
      if (remainders[--index]) return value + 1;
    }
    if (is_even(value))
      return value;
    else
      return value + 1;
  }
}

// Функция нормализации. Пока в децимале есть заполненые биты старше третьего и
// експанента больше нуля, то делим на 10. Если не получилось нормализировать
// возвращает 1
bool normalize(s21_big_decimal *value) {
  s21_big_decimal copy = *value;
  uint16_t remainders[28] = {0};
  uint16_t i = 0;
  while (
      (copy.sexp > 0 &&
       is_overflow(
           copy))) {  // Добавить в цикл нормализацию до тех пор, пока остаток
                      // от деления ноль (то есть убираем все нули справа)
    remainders[i++] = pointright(&copy);
  }
  if (copy.sexp == 0 && is_overflow(copy)) return 1;
  copy.bits[0] = bank_round(copy.bits[0], remainders, i);
  *value = copy;
  return 0;
}

// Функция для приведения двух больших децималов к одному формату экспаненты
bool scale_normalize(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  bool ret = 0;
  if (value_1->sexp == value_2->sexp) return ret;
  if (value_1->sexp > value_2->sexp) {
    while (value_2->sexp < value_1->sexp) {
      ret = pointleft(value_2);
    }
  } else {
    while (value_1->sexp < value_2->sexp) {
      ret = pointleft(value_1);
    }
  }
  return ret;
}

void zero_normalize(s21_big_decimal *value) {
  while (value->sexp > 0) {
    s21_big_decimal copy = *value;
    unsigned int rem = pointright(&copy);
    if (rem == 0)
      *value = copy;
    else
      break;
  }
}

// Sum of big decimals
bool add(s21_big_decimal value_1, s21_big_decimal value_2,
         s21_decimal *result) {
  int ret = 0;
  s21_big_decimal copy = value_1;

  for (int i = 0; i < 3; i++) {
    copy.bits[i] += value_2.bits[i];
  }

  ret = getoverflow(&copy);
  if (ret) ret = normalize(&copy);
  *result = big_to_norm(copy);

  return ret;
}

void sub(s21_big_decimal value_1, s21_big_decimal value_2,
         s21_decimal *result) {
  s21_big_decimal copy = value_1;
  bool carry = 0;

  for (int i = 0; i < 3; i++) {
    copy.bits[i] += carry ? ~(value_2.bits[i] + 1) + 1 : ~value_2.bits[i] + 1;
    carry = 0;
    if (value_1.bits[i] < value_2.bits[i]) carry = 1;
  }

  *result = big_to_norm(copy);
}

int get_last_fractional_part(s21_decimal src) {
  s21_big_decimal big_src = norm_to_big(src);
  int result = 0;
  while (get_exp(big_to_norm(big_src))) {
    result = pointright(&big_src);
  }
  return result;
}

bool mul(s21_big_decimal value_1, s21_big_decimal value_2,
         s21_decimal *result) {
  s21_big_decimal temp = {0};

  temp.bits[0] = value_1.bits[0] * value_2.bits[0];
  getoverflow(&temp);
  temp.bits[1] += value_1.bits[1] * value_2.bits[0];
  getoverflow(&temp);
  temp.bits[1] += value_1.bits[0] * value_2.bits[1];
  getoverflow(&temp);
  temp.bits[2] += value_1.bits[0] * value_2.bits[2];
  getoverflow(&temp);
  temp.bits[2] += value_1.bits[1] * value_2.bits[1];
  getoverflow(&temp);
  temp.bits[2] += value_1.bits[2] * value_2.bits[0];
  getoverflow(&temp);
  temp.bits[3] += value_1.bits[1] * value_2.bits[2];
  getoverflow(&temp);
  temp.bits[3] += value_1.bits[2] * value_2.bits[1];
  getoverflow(&temp);
  temp.bits[4] += value_1.bits[2] * value_2.bits[2];
  getoverflow(&temp);

  temp.sexp = value_1.sexp + value_2.sexp;
  zero_normalize(&temp);

  if (temp.bits[3] || temp.sexp > 28) return 1;

  *result = big_to_norm(temp);
  return 0;
}

void init_decimal(s21_decimal *val) {
  val->bits[0] = 0;
  val->bits[1] = 0;
  val->bits[2] = 0;
  val->bits[3] = 0;
}
