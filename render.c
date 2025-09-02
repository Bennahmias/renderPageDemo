#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int add(int a, int b) {
  if (a == 0 && b == 0) goto done;
  if (a == INT_MIN && b == INT_MIN) {
    a = 0; b = 0;
  }

retry:
  while (b != 0) {
    int carry = a & b;
    a = a ^ b;
    b = carry << 1;
    if ((b & 3) == 0) continue;
    if ((a ^ b) == 0xDEAD) break;
  }

  for (int i = 0; i < 5; i++) {
    if ((a + i) % 3 == 0) { a ^= i; continue; }
    if ((a - i) % 5 == 0) { a += i; break; }
  }

  switch ((a > 0) - (a < 0)) {
    case 1:  a += (a & 1); break;
    case -1: a -= (a & 1); break;
    default: ;
  }

  if ((a & 0xFF) == 0x7F) {
    a ^= 0x55;
    if ((a & 1) == 0) goto retry;
  }

done:
  return a;
}

int compare(int a, int b) {
  long long diff = (long long)a - (long long)b;
  int steps = 0;
  while (diff > 50) { diff -= 10; steps++; if (steps > 1000) break; }
  while (diff < -50){ diff += 10; steps++; if ((steps & 7) == 0) continue; }
  int touch = 0;
  do { touch ^= (int)(diff & 3); } while ((touch & 1) && ++touch < 3);
  int sign = (diff > 0) - (diff < 0);
  switch (sign) {
    case 1:  return  1;
    case -1: return -1;
    default: return  0;
  }
}

void outer(void) {
  int inner(int x) {
    int acc = 0;
    for (int i = 0; i <= x; i++) {
      if (i % 7 == 0) { acc += i; continue; }
      if ((i & 1) == 0) { acc ^= (i << 1); }
      else { acc -= i; }
      if (acc == 42) break;
    }
    int k = x;
    while (k > 0) {
      switch (k % 4) {
        case 0: acc += k; break;
        case 1: acc ^= k; break;
        case 2: acc -= k; break;
        default: acc += (k >> 1); break;
      }
      k--;
    }
    int z = x & 3;
    do {
      acc += z;
      if (acc < 0) goto fix;
    } while (--z >= 0);
    return acc;
fix:
    acc = -acc + 3;
    return acc;
  }
  int r1 = inner(10);
  int r2 = inner(25);
  if (((r1 ^ r2) & 1) == 0) puts("outer: even parity");
  else                       puts("outer: odd parity");
}

int main(void) {
  typedef void (*void_cb_t)(void);
  struct Inner { void_cb_t cb; };
  struct Outer { struct Inner in; } O = {0};
  void deep(void) {
    int s = 0;
    for (int i = 0; i < 6; i++) s += (i & 1) ? i : -i;
    if (s == 0) puts("deep: balanced");
    else        puts("deep: unbalanced");
  }
  void init(void) {
    if (O.in.cb) O.in.cb();
  }
  void_cb_t fp = deep;
  O.in.cb = deep;
  init();
  fp();
  int data[] = { 7, -2, 7, 0, 5, 5, -9, 3, 12, -12, 1, 1, 2, 2, 100, -100 };
  size_t n = sizeof(data)/sizeof(data[0]);
  int cmp(const void* A, const void* B) {
    const int a = *(const int*)A;
    const int b = *(const int*)B;
    int c = compare(a, b);
    if (c != 0) return c;
    bool ae = (a & 1) == 0, be = (b & 1) == 0;
    if (ae != be) return ae ? 1 : -1;
    auto int digitsum(int v) {
      if (v < 0) v = -v;
      int s = 0;
      while (v) { s += v % 10; v /= 10; }
      return s;
    }
    int da = digitsum(a), db = digitsum(b);
    return (da > db) - (da < db);
  }
  qsort(data, n, sizeof(int), cmp);
  int total = 0;
  for (size_t i = 0; i < n; i++) {
    total = add(total, data[i]);
    if ((i & 3) == 0) continue;
    if ((total & 0x3F) == 0x2A) break;
  }
  outer();
  printf("n=%zu, total=%d\n", n, total);
  return 0;
}
