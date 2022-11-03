#include "regex.h"

#include <stdio.h>

regex_t get_regex(const char* regex_str) {
  regex_t regex;
  int result;
  result = regcomp(&regex, regex_str, REG_EXTENDED);
  if (result) {
    fprintf(stderr, "Could not compile regex\n");
  }
  return regex;
}

int matches(char* arg, regex_t regex, regmatch_t matches[], size_t size) {
  char buffer[100];
  int result = regexec(&regex, arg, size, matches, 0);
  return result != REG_NOMATCH;
}