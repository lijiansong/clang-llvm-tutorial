#include <stdio.h>
#include <stdlib.h>

struct test_t {
  int (*add)(int a, int b);
  int (*sub)(int a, int b);
  int (*mult)(int a, int b);
};

static int test_add(int a, int b) { return (a + b); }

static int test_sub(int a, int b) { return (a - b); }

static int test_mult(int a, int b) { return (a * b); }

void print_usage() { printf("./test <num1> <num2>\n"); }

int main() {
  print_usage();
  struct test_t test_var;
  test_var.add = test_add;
  test_var.sub = test_sub;
  test_var.mult = test_mult;

  int add_rst;
  int sub_rst;
  int mult_rst;
  add_rst = test_var.add(5, 6);
  sub_rst = test_var.sub(5, 6);
  mult_rst = test_var.mult(5, 6);
  printf("add_rst: %d; sub_rst: %d; mult_rst: %d\n", add_rst, sub_rst,
         mult_rst);

  return 0;
}
