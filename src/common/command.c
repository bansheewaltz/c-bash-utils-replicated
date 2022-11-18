#include "command.h"
#include <stdlib.h>
#include <stdio.h>

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
  info.options_str = options;
  info.validity_regex = regex;
  info.count = 0;
  info.current_options = NULL;
  return info;
}

void add_option_info(command_info* info, char option) {
  info->current_options = realloc(info->current_options, info->count + 1);
  info->current_options[info->count++] = option;
}

void clear_args(command c) {
  for (int i = 0; i < 10; ++i) {
    c.args[i] = NULL;
  }
}