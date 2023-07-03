#include <stdio.h>
#include <string.h>

#include "globals.h"

void print_error(char message[]) {
  fprintf(stderr, "%s: %s\n", PROGRAM_NAME, message);
}

void t_setprogname(char const invocation_name[]) {
  int i = strlen(invocation_name) - 1;
  while (invocation_name[i] != '/') {
    --i;
  }
  ++i;
  PROGRAM_NAME = &(invocation_name[i]);
}

void add_delim_unsafe(char *re_str, int *re_len, char delimiter[]) {
  strcat(re_str, delimiter);
  *re_len += strlen(delimiter);
}
