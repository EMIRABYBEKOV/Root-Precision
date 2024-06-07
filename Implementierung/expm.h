#include <stdbool.h>

/**
 * @brief This function mulm takes two matrices and multiplies each component or
 * bignum (like matrices are multiplied) and returns the pointer to this new
 * matrix.
 * @param x a matrix
 * @param y another matrix
 * @return a pointer to a new matrix
 */
struct bigmatrix *mulm(struct bigmatrix *x, struct bigmatrix *y) {
  // Allocate enough space for the matrix
  struct bigmatrix *ret = (struct bigmatrix *)malloc(sizeof(struct bigmatrix));
  // Assign each entry in the matrix it's new value (standard matrix
  // multiplication but with bignums)
  struct bignum *k1 = kmul(&x->m[0], &y->m[0]);
  struct bignum *k2 = kmul(&x->m[1], &y->m[2]);
  struct bignum *k3 = kmul(&x->m[0], &y->m[1]);
  struct bignum *k4 = kmul(&x->m[1], &y->m[3]);
  struct bignum *k5 = kmul(&x->m[2], &y->m[0]);
  struct bignum *k6 = kmul(&x->m[3], &y->m[2]);
  struct bignum *k7 = kmul(&x->m[2], &y->m[1]);
  struct bignum *k8 = kmul(&x->m[3], &y->m[3]);

  struct bignum *a1 = add(k1, k2);
  struct bignum *a2 = add(k3, k4);
  struct bignum *a3 = add(k5, k6);
  struct bignum *a4 = add(k7, k8);

  // Free allocated mem
  free(k1);
  free(k2);
  free(k3);
  free(k4);
  free(k5);
  free(k6);
  free(k7);
  free(k8);

  ret->m[0] = *a1;
  ret->m[1] = *a2;
  ret->m[2] = *a3;
  ret->m[3] = *a4;

  // Free allocated mem
  free(a1);
  free(a2);
  free(a3);
  free(a4);

  return ret;
}

/**
 * @brief This function mulm takes two matrices and multiplies each component or
 * bignum (like matrices are multiplied) and returns the pointer to this new
 * matrix.
 * @param x a matrix
 * @param y another matrix
 * @return a pointer to a new matrix
 */
struct bigmatrix *mulm_V1(struct bigmatrix *x, struct bigmatrix *y) {
  // Allocate enough space for the matrix
  struct bigmatrix *ret = (struct bigmatrix *)malloc(sizeof(struct bigmatrix));
  // Assign each entry in the matrix it's new value (standard matrix
  // multiplication but with bignums)
  struct bignum *k1 = kmul_v1(&x->m[0], &y->m[0]);
  struct bignum *k2 = kmul_v1(&x->m[1], &y->m[2]);
  struct bignum *k3 = kmul_v1(&x->m[0], &y->m[1]);
  struct bignum *k4 = kmul_v1(&x->m[1], &y->m[3]);
  struct bignum *k5 = kmul_v1(&x->m[2], &y->m[0]);
  struct bignum *k6 = kmul_v1(&x->m[3], &y->m[2]);
  struct bignum *k7 = kmul_v1(&x->m[2], &y->m[1]);
  struct bignum *k8 = kmul_v1(&x->m[3], &y->m[3]);

  struct bignum *a1 = add(k1, k2);
  struct bignum *a2 = add(k3, k4);
  struct bignum *a3 = add(k5, k6);
  struct bignum *a4 = add(k7, k8);

  // Free allocated mem
  free(k1);
  free(k2);
  free(k3);
  free(k4);
  free(k5);
  free(k6);
  free(k7);
  free(k8);

  ret->m[0] = *a1;
  ret->m[1] = *a2;
  ret->m[2] = *a3;
  ret->m[3] = *a4;

  // Free allocated mem
  free(a1);
  free(a2);
  free(a3);
  free(a4);

  return ret;
}

/**
 * @brief This function findZ takes the exponent and calculates a bignum z which
 * is used for further calculation.
 * @param n a long number (exponent)
 * @return a long number (z)
 */
static long findZ(long n) {
  long c = 0;
  while (n > 0) {
    n = n >> 1;
    c++;
  }
  return c;
}

/**
 * @brief This function endMul multiplies all matrices where its index
 * corresponds to a 1 at the index in n.
 * @param n a long value containing the exponent
 * @param biga a pointer to the bigmatrix array of all as so to speak. (a0, a1,
 * ..., an-1, an)
 * @return a bigmatrix containing the final result of the multiplication
 */
static struct bigmatrix *endMul(long n, struct bigmatrix *biga) {
  struct bigmatrix *r = (struct bigmatrix *)malloc(sizeof(struct bigmatrix));
  long c = 0;
  bool set = false;
  // Iterate over the binary representation of the number
  while (n > 0) {
    // Binary AND with 0x1 to check if the least significant bit is equal to 1
    // and if so, multiply the current return matrix with biga(counter)
    if ((n & 0x1) == 1) {
      if (!set) {
        r = &biga[c];
        set = true;
      } else {
        r = mulm(r, &biga[c]);
      }
    }
    // Shift the binary representation of the number one bit to the right
    n = n >> 1;
    c++;
  }

  return r;
}

/**
 * @brief This function endMul multiplies all matrices where its index
 * corresponds to a 1 at the index in n.
 * @param n a long value containing the exponent
 * @param biga a pointer to the bigmatrix array of all as so to speak. (a0, a1,
 * ..., an-1, an)
 * @return a bigmatrix containing the final result of the multiplication
 */
static struct bigmatrix *endMul_V1(long n, struct bigmatrix *biga) {
  struct bigmatrix *r = (struct bigmatrix *)malloc(sizeof(struct bigmatrix));
  long c = 0;
  bool set = false;
  // Iterate over the binary representation of the number
  while (n > 0) {
    // Binary AND with 0x1 to check if the least significant bit is equal to 1
    // and if so, multiply the current return matrix with biga(counter)
    if ((n & 0x1) == 1) {
      if (!set) {
        r = &biga[c];
        set = true;
      } else {
        r = mulm_V1(r, &biga[c]);
      }
    }
    // Shift the binary representation of the number one bit to the right
    n = n >> 1;
    c++;
  }
  return r;
}

/**
 * @brief This function calcSquares calculates a table of squared matrices for
 * the multiplication at the end.
 * @param a the pointer to the matrix array, where all matrices should be stored
 * @param z a long value containing the exponent (= number of squares)
 */
static void calcSquares(struct bigmatrix *a, long z) {
  // Note that the loop starts at index 1, since the first entry is the default
  // matrix
  for (long i = 1; i < z; i++) {
    struct bigmatrix *temp = mulm(&a[i - 1], &a[i - 1]);
    a[i] = *temp;
    // free the allocated space after pushing it
    free(temp);
  }
}

/**
 * @brief This function calcSquares calculates a table of squared matrices for
 * the multiplication at the end.
 * @param a the pointer to the matrix array, where all matrices should be stored
 * @param z a long value containing the exponent (= number of squares)
 */
static void calcSquares_V1(struct bigmatrix *a, long z) {
  // Note that the loop starts at index 1, since the first entry is the default
  // matrix
  for (long i = 1; i < z; i++) {
    struct bigmatrix *temp = mulm_V1(&a[i - 1], &a[i - 1]);
    a[i] = *temp;
    // free the allocated space after pushing it
    free(temp);
  }
}

/**
 * @brief [MAIN version] This function takes a n (the exponent) and calculates the (0 1, 1 2)
 * to the power of n matrix using the fast exponentiation.
 * @param n the exponent
 * @return a big matrix (containing only bignums)
 */
struct bigmatrix *expm(long n) {
  // Create the default matrix
  static unsigned char arr[4][1] = {{0}, {1}, {1}, {2}};
  struct bignum a0 = {1, arr[0]};
  struct bignum a1 = {1, arr[1]};
  struct bignum a2 = {1, arr[2]};
  struct bignum a3 = {1, arr[3]};
  struct bigmatrix m = {{a0, a1, a2, a3}};

  // Calculate the number of squares
  long z = findZ(n);

  // Calculate all squares for the matrix m with the max of z
  struct bigmatrix *array =
      (struct bigmatrix *)malloc(z * sizeof(struct bigmatrix));
  array[0] = m;
  calcSquares(array, z);

  // === Testing purposes ===
  // for (long i = 0; i < z; i++) {
  //   for (long j = 0; j < array[i].m[3].len; j++) {
  //     printf("%d ", array[i].m[3].arr[j]);
  //   }
  //   printf("\n");
  // }
  // printf("\n");

  // Check which index should be multiplied and multiply them to one matrix
  struct bigmatrix *biga = endMul(n, array);

  free(array);

  return biga;
}

/**
 * @brief [SECOND version] This function takes a n (the exponent) and calculates the (0 1, 1 2)
 * to the power of n matrix using the fast exponentiation.
 * @param n the exponent
 * @return a big matrix (containing only bignums)
 */
struct bigmatrix *expm_V1(long n) {
  // Create the default matrix
  static unsigned char arr[4][1] = {{0}, {1}, {1}, {2}};
  struct bignum a0 = {1, arr[0]};
  struct bignum a1 = {1, arr[1]};
  struct bignum a2 = {1, arr[2]};
  struct bignum a3 = {1, arr[3]};
  struct bigmatrix m = {{a0, a1, a2, a3}};

  // Calculate the number of squares
  long z = findZ(n);

  // Calculate all squares for the matrix m with the max of z
  struct bigmatrix *array =
      (struct bigmatrix *)malloc(z * sizeof(struct bigmatrix));
  array[0] = m;
  calcSquares_V1(array, z);

  // Check which index should be multiplied and multiply them to one matrix
  struct bigmatrix *biga = endMul_V1(n, array);

  free(array);

  return biga;
}
