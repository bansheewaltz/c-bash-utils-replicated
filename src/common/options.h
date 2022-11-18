#ifndef MY_REGEX_H
#define MY_REGEX_H

#define ILLEGAL_OPTION "illegal option -- "
#define NO_ARGUMENT "option requires an argument --"
#define NO_SUCH_FILE "No such file or directory"

#include <regex.h>
#include "command.h"

// Regex
void rmchr(const char *str, char *dest, char c);
int matches(const char *arg, const char *regex_str);
int is_options_valid(const char *option, command_info *info);

// Option
int has_argument(option opt, const char *regex_str);
void add_argument(option *option, const char *arg);

// Print errors


#endif // MY_REGEX_H