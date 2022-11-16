#include "options.h"

#include <string.h>

void print_file() {}

void rmchr(char* str, char* dest, char c) {
  int j, n = strlen(str);
  for (int i = j = 0; i < n; i++)
    if (str[i] != c) dest[j++] = str[i];

  dest[j] = '\0';
}
