#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#define _GNU_SOURCE
#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR -1
#define SUCCESS 0
#define PATTERN_BUF 4096
#define REGERROR_BUF 128

typedef struct {
  bool e;  // add pattern;
  bool i;  // ignore case;
  bool v;  // invert result;
  bool c;  // count only;
  bool l;  // show filenames only;
  bool n;  // number matched lines;
  bool h;  // show without filenames;
  bool s;  // supress errors;
  bool f;  // read patterns from file;
  bool o;  // output matched parts;
  bool show_filenames;
} t_options;

typedef struct {
  char *str;
  int len;
  int len_limit;
  bool specified;
  int regex_flag;
} t_info;

typedef struct {
  regex_t *regex;
  regmatch_t *regmatch;
  bool dot_pattern;
} t_re;

#endif  // SRC_GREP_S21_GREP_H_
