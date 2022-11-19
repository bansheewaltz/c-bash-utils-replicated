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

int find_valid_options(const char *option, command *utility) {
  int valid;
  if ((valid = matches(option, utility->info.validity_regex))) {
    for (int j = 1; j < strlen(option); j++) {
      add_option(utility, option[j]);
    }
  }
  return valid;
}

int has_argument(option *opt, const char *regex_str) {
  return matches(&opt->name, regex_str);
}

void add_option_argument(option *option, const char *arg) { option->argument = arg; }

int save_options(command *utility, int count, char *options[], int *i) {
  int error = 0;
  for (; *i < count && !error; (*i)++) {
    error = !find_valid_options(options[*i], utility);
    if (!error) {
      error = option_arguments_satisfied(utility, options, count, i);
    } else if (count == *i) {
      fprintf(stderr, "%s: %s %c", utility->name, utility->info.errors[1],
              last_option(utility)->name);
    }
  }
  return error;
}

int option_arguments_satisfied(command *utility, char *options[], int count,
                               int *i) {
  int need_arg =
      has_argument(last_option(utility), utility->info.arguments_options_regex);
  if (need_arg) {
    if (count <= *i + 1) {
      fprintf(stderr, "%s: %s %c", utility->name, utility->info.errors[1],
              last_option(utility)->name);
    } else {
      add_option_argument(last_option(utility), options[++(*i)]);
    }
  }
  return need_arg && count <= *i + 1;
}

int save_arguments(command *utility, int count, char *options[], int *i) {
  printf("%d %d --\n", *i, count);
  for (int j = *i; j < count; j++) {
    add_utility_argument(utility, options[j]);
  }
}