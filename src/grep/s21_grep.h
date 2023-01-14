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
#define PATTERN_BUF 16  // initial size
#define REGERROR_BUF 128
#define DELIM_LEN 2
// codes for add_pattern function
#define FROM_FILE 0
#define FROM_E 1

typedef struct {
  bool c;  // output only count of succeeded search requests;
  bool e;  // add pattern specified after;
  bool f;  // read search patterns from a file specified after;
  bool h;  // output result without filenames;
  bool i;  // ignore search pattern case;
  bool l;  // output filenames only;
  bool n;  // output number of matched lines;
  bool o;  // output matched lines' parts;
  bool s;  // supress file-accessing erros;
  bool v;  // invert search result;
  bool show_filenames;
  // as internal helper
} t_options;

typedef struct {
  char *str;
  int len;
  int capacity;
  bool specified_through_option;
  int regex_flag;
} t_info;

typedef struct {
  regex_t *regex;
  regmatch_t *regmatch;
  bool is_pattern_dot;
} t_re;

#endif  // SRC_GREP_S21_GREP_H_
