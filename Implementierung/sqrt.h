#include "division.h"
#include "expm.h"

/**
 * @brief [MAIN implementation] This function sqrt2 takes an exponent and
 * multiplies the (0 1, 1 2) matrix with itself using the fast exponentiation.
 * @param s the exponent
 * @return a bignum that contains the decimal places in binary form
 */
struct bignum sqrt2(size_t s) {
  struct bigmatrix *matr = expm(4*s);

  struct bignum d = *divide(&matr->m[1], &matr->m[3], 4*s);

  return d;
}

/**
 * @brief [SECOND implementation] This function sqrt2_V1 takes an exponent and
 * multiplies the (0 1, 1 2) matrix with itself using the fast exponentiation.
 * @param s the exponent
 * @return a bignum that contains the decimal places in binary form
 */
struct bignum sqrt2_V1(size_t s) {
  struct bigmatrix *matr = expm_V1(4*s);

  struct bignum d = *divide(&matr->m[1], &matr->m[3], 4*s);

  return d;
}
