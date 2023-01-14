#ifndef SRC_GREP_SUPPORT_FUNCTIONS_H_
#define SRC_GREP_SUPPORT_FUNCTIONS_H_

#include "typedefs.h"

bool arguments_are_enough(int argc, char *argv[], t_info *re_pattern);
bool more_than_one_file(int argc, int optind);
void print_usage(void);
bool is_line_empty(char *line);
bool is_pattern_dot(t_info *re_pattern);
void print_regerror(regex_t *regex, int result);

#endif  // SRC_GREP_SUPPORT_FUNCTIONS_H_
