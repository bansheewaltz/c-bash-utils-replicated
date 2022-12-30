#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#define BUFFER_SIZE 5000

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool e;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool f;
  bool o;
} t_options;

int parse_options(int argc, char **argv, t_options *flags, char *patternE);
void processing(t_options *flags, FILE *fp, regex_t reg, char *file);
void grep_file(t_options *flags, char *pattern, char *file);
int read_regex_from_file(char *pattern, char *temp);
void grep(t_options *flags, int argc, char **argv, char *temp);

#endif  // SRC_GREP_S21_GREP_H_