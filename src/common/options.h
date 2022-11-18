#ifndef MY_REGEX_H
#define MY_REGEX_H

#define ILLEGAL_OPTION ": illegal option -- "
#define NO_SUCH_FILE ": No such file or directory"

#include "command.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rmchr(const char *str, char *dest, char c);
int matches(const char *arg, const char *regex_str);
int is_options_valid(const char *option, command_info *info);

#endif // MY_REGEX_H