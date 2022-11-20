#ifndef MY_REGEX_H
#define MY_REGEX_H

#define ERRORS                                                                 \
  "illegal option -- ", "option requires an argument --",                      \
      "No such file or directory"

#include "command.h"
#include <regex.h>

// Regex
void rmchr(const char *str, char *dest, char c);
int matches(const char *arg, const char *regex_str);
int find_valid_options(const char *option, command *utility);

// Option
int has_argument(option *opt, const char *regex_str);
void add_option_argument(option *option, const char *arg);
int save_options(command *utility, int count, char *options[], int *i);
int option_arguments_satisfied(command *utility, char *options[], int count,
                               int *i);

// Arguments
int save_arguments(command *utility, int count, char *options[], int *i);

// Print errors

#endif // MY_REGEX_H