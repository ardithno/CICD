#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct c_flags {
  int e;
  int v;
  int c;
  int i;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} t_flags;

#endif  // SRC_GREP_S21_GREP_H_
