// Covers all requested extraction cases (including GNU C nested functions).

#include <stdio.h>
#include <stdlib.h>

// basic definition
int add(int a, int b) { return a + b; }

// inline + static
static inline double square(double x) { return x * x; }

// function pointer assignment
void handler() {}
void (*fp)() = handler;

// typedef function pointer + definition
typedef int (*cmp_t)(int, int);
int compare(int a, int b) { return a - b; }

// struct with callback
void init() {}
struct C { void (*cb)(); } c = { init };

// nested struct callback assignment (assignment done in a function body)
void deep() {}
struct Outer { struct Inner { void (*cb)(); } in; } outer;
void set_deep_callback(void) { outer.in.cb = deep; }

// recursive function
int fact(int n) { return n <= 1 ? 1 : n * fact(n - 1); }

// function passed to qsort
int cmp(const void* a, const void* b) { return 0; }
int arr_for_sort[4] = {3,1,4,2};
size_t n_for_sort = 4;

// multiple functions defined in one line
void start() {} void stop() {} void reset() {}

// duplicate function redefinition (intentionally duplicated, will not compile in strict C)
void ping() {}
void ping() {}


void outer() {
  int inner(int x) { return x + 1; }
  (void)inner(5);
}

void container() {
  void first() {}
  void second() {}
  first(); second();
}

void top() {
  void mid() {
    void bottom() {}
    bottom();
  }
  mid();
}

void runner(int n) {
  for (int i = 0; i < n; i++) {
    int loopfn(int x) { return x * i; }
    (void)loopfn(i);
  }
}

void check(int flag) {
  if (flag) {
    void innerA() {}
    innerA();
  } else {
    void innerB() {}
    innerB();
  }
}

/* A small main so the file is self-contained if you want to compile with GCC (minus the duplicate ping) */
int main(void) {
  // qsort usage that matches the test idea
  qsort(arr_for_sort, n_for_sort, sizeof(int), cmp);

  // call a few to silence warnings
  (void)add(1,2);
  (void)square(3.0);
  if (fp) fp();
  set_deep_callback();
  (void)fact(5);
  start(); stop(); reset();
  outer(); container(); top(); runner(3); check(1);

  puts("Fixture ran (if compiled with GNU C and without duplicate ping).");
  return 0;
}
