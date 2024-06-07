// struct for big integers
struct bignum {
  // vorerst int, es könnte long oder länger gefordert werden(hängt vom
  // maximalen n ab)
  int len;
  unsigned char *arr;
} bignum;

// Type für die Matrix selbst
struct bigmatrix {
  // Das Array enthält die einzelnen Zellen der Matrix
  // 1. Zeile: 0, 1
  // 2. Zeile: 2, 3
  struct bignum m[4];
} bigmatrix;