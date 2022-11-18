#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int matches(const char *arg, const char *regex_str) {
  regex_t regex;
  regcomp(&regex, regex_str, REG_EXTENDED);
  return regexec(&regex, arg, 0, NULL, 0) != REG_NOMATCH;
}

void rmchr(const char *str, char *dest, char c) {
  int j, n = strlen(str);
  for (int i = j = 0; i < n; i++)
    if (str[i] != c)
      dest[j++] = str[i];
  dest[j] = '\0';
}

int is_options_valid(const char *option, command_info *info) {
  int valid;
  if ((valid = matches(option, info->validity_regex))) {
    char *buffer = malloc(strlen(option));
    rmchr(option, buffer, '-');
    for (int j = 0; j < strlen(buffer); j++) {
      char name = *strstr(info->options_str, &buffer[j]);
      add_option(info, name);
    }
    free(buffer);
  }
  return valid;
}

int has_argument(option opt, const char *regex_str) {
  return matches(&opt.name, regex_str);
}

option init_option(char name) {
  option opt;
  opt.name = name;
  opt.argument = NULL;
  return opt;
}
void add_argument(option *option, const char *arg) {
  option->argument = arg;
}