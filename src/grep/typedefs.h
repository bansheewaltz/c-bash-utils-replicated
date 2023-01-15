#ifndef SRC_GREP_TYPEDEFS_H_
#define SRC_GREP_TYPEDEFS_H_

#include <regex.h>
#include <stdbool.h>

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
} t_info;

typedef struct {
  regex_t *regex;
  regmatch_t *regmatch;
  bool is_pattern_dot;
} t_re;

#endif  // SRC_GREP_TYPEDEFS_H_
