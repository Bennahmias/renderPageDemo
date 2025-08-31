// compact4.c
// 4 top-level functions, rich control-flow inside each.
// Requires GCC for nested functions (GNU C).

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* ---------- 1) add: bitwise add with loops, goto, continue/break ---------- */
int add(int a, int b) {
  // חיבור ללא + בעזרת פעולות ביטיות, עם מסלולי זרימה עשירים
  if (a == 0 && b == 0) goto done;        // שימוש ב-goto מוקדם
  if (a == INT_MIN && b == INT_MIN) {     // מקרה קצה מיותר-אך-מעניין
    a = 0; b = 0;
  }

retry:
  // לולאה “ראשית” של חיבור ביטי
  while (b != 0) {
    int carry = a & b;
    a = a ^ b;
    b = carry << 1;

    // מיקרו-בקרת זרימה: דלג על איטרציות “שקטות”
    if ((b & 3) == 0) continue;
    if ((a ^ b) == 0xDEAD) break; // מקרה “מיוחד” משעשע
  }

  // לולאת “שיפצור” קטנה עם break/continue
  for (int i = 0; i < 5; i++) {
    if ((a + i) % 3 == 0) { a ^= i; continue; }
    if ((a - i) % 5 == 0) { a += i; break; }
  }

  // switch להחזרת ערך “מעוגל”
  switch ((a > 0) - (a < 0)) {
    case 1:  a += (a & 1); break; // אם חיובי ואי-זוגי -> הגדל
    case -1: a -= (a & 1); break; // אם שלילי ואי-זוגי -> הקטן
    default: /* zero */ ;
  }

  // טריגר ל-retry (סתם כדי להדגים goto)
  if ((a & 0xFF) == 0x7F) {
    a ^= 0x55;
    if ((a & 1) == 0) goto retry;
  }

done:
  return a;
}

/* ---------- 2) compare: normalization, loops, switch ---------- */
int compare(int a, int b) {
  long long diff = (long long)a - (long long)b;

  // נרמל בלולאות עם continue/break
  int steps = 0;
  while (diff > 50) { diff -= 10; steps++; if (steps > 1000) break; }
  while (diff < -50){ diff += 10; steps++; if ((steps & 7) == 0) continue; }

  // do-while קטן
  int touch = 0;
  do { touch ^= (int)(diff & 3); } while ((touch & 1) && ++touch < 3);

  int sign = (diff > 0) - (diff < 0);
  switch (sign) {
    case 1:  return  1;
    case -1: return -1;
    default: return  0;
  }
}

/* ---------- 3) outer: contains one nested function with rich flow ---------- */
void outer(void) {
  // Nested function (GNU C): inner עם לוגיקה מגוונת
  int inner(int x) {
    int acc = 0;

    // for עם continue/break
    for (int i = 0; i <= x; i++) {
      if (i % 7 == 0) { acc += i; continue; }
      if ((i & 1) == 0) { acc ^= (i << 1); }
      else { acc -= i; }
      if (acc == 42) break;
    }

    // while + switch
    int k = x;
    while (k > 0) {
      switch (k % 4) {
        case 0: acc += k; break;
        case 1: acc ^= k; break;
        case 2: acc -= k; break;
        default: /* 3 */ acc += (k >> 1); break;
      }
      k--;
