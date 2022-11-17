#include "command.h"
#include <stdlib.h>

command init_command(const char *name, func funcs[], int size) {
  command c;
  c.name = name;
  c.options_funcs = realloc(NULL, size);
  for (int i = 0; i < size; ++i) {
    c.options_funcs[i] = funcs[i];
  }
  c.size = size;
  return c;
}

command_info init_info(const char *options, const char *regex) {
  command_info info;
  info.options = options;
  info.regex = regex;
  return info;
}

void clear_args(command c) {
  for (int i = 0; i < 10; ++i) {
    c.args[i] = NULL;
  }
}