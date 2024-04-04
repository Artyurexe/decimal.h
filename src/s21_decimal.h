#ifndef SRC_S21_DECIMAL_H
#define SRC_S21_DECIMAL_H

#include <stdint.h>
#define TRUE 1
#define FALSE 0
#include <math.h>
#include <stdbool.h>
#define UINT_MAX 4294967295
#define MINUS 0x80000000
#define EXP 0x00ff0000
#define MANTISSA 0x003FFFFF

typedef struct {
  uint32_t bits[4];
} s21_decimal;

typedef struct {
  uint64_t bits[7];
  uint32_t sexp;
} s21_big_decimal;

typedef enum { OK = 0, INF, NEG_INF, DIV_ZERO } arythm_error;

typedef enum { CAST_ERROR = 1, CALC_ERROR = 1 } tranformation_error;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_less(s21_decimal src1, s21_decimal src2);
int s21_is_less_or_equal(s21_decimal src1, s21_decimal src2);
int s21_is_greater(s21_decimal src1, s21_decimal src2);
int s21_is_greater_or_equal(s21_decimal src1, s21_decimal src2);
int s21_is_equal(s21_decimal src1, s21_decimal src2);
int s21_is_not_equal(s21_decimal src1, s21_decimal src2);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

bool check_exp(s21_decimal src);
bool get_bit(unsigned int src, unsigned int index);
bool get_sign(s21_decimal src);
bool is_null(s21_decimal src);
bool is_even(unsigned int num);
bool getoverflow(s21_big_decimal *value);
bool is_overflow(s21_big_decimal value);
bool pointleft(s21_big_decimal *value);
bool normalize(s21_big_decimal *value);
bool scale_normalize(s21_big_decimal *value_1, s21_big_decimal *value_2);
bool add(s21_big_decimal value_1, s21_big_decimal value_2, s21_decimal *result);
void sub(s21_big_decimal value_1, s21_big_decimal value_2, s21_decimal *result);
void set_1_bit(unsigned int *src, unsigned int index);
void negate_bit(unsigned int *src, unsigned int index);
void set_exp(s21_decimal *value, unsigned int exp);
void set_sign(s21_decimal *src, bool sign);
void zero_normalize(s21_big_decimal *value);
uint32_t get_exp(s21_decimal src);
uint32_t pointright(s21_big_decimal *value);
s21_decimal big_to_norm(s21_big_decimal src);
s21_big_decimal norm_to_big(s21_decimal src);
bool mul(s21_big_decimal value_1, s21_big_decimal value_2, s21_decimal *result);
int get_last_fractional_part(s21_decimal src);
int find_bit(s21_decimal src, int index);
int get_scale(s21_decimal dst);
void init_decimal(s21_decimal *val);

#endif  // SRC_S21_DECIMAL_H
