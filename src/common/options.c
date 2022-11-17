#include "options.h"

int matches(const char *arg, const char *regex_str) {
  char buffer[100];
  regex_t regex;
  regcomp(&regex, regex_str, REG_EXTENDED);
  int result = regexec(&regex, arg, 0, NULL, 0);
  return result != REG_NOMATCH;
}

void rmchr(const char *str, char *dest, char c) {
  int j, n = strlen(str);
  for (int i = j = 0; i < n; i++)
    if (str[i] != c)
      dest[j++] = str[i];
  dest[j] = '\0';
}

command check_option(const char *option, command_info info) {
  command c;
  if (matches(option, info.regex)) {
    char *buffer = malloc(strlen(option));
    rmchr(option, buffer, '-');
    for (int j = 0; j < strlen(option); j++) {
      char *ptr = strstr(info.options, &buffer[j]);
      if (ptr != NULL) {
        printf("Match: %c\n", *ptr);
      }
    }
  }
  return c;
}
