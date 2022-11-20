#include "command.h"
#include <stdio.h>
#include <stdlib.h>

command init_command(const char *name, func funcs[], int size, consts info) {
  command c;
  // Options
  c.opt_count = 0;
  c.current_options = NULL;
  // Functions
  c.options_funcs = realloc(NULL, size);
  for (int i = 0; i < size; ++i) {
    c.options_funcs[i] = funcs[i];
  }
  c.func_count = size;
  // General
  c.name = name;
  c.info = info;
  // Arguments
  c.args = NULL;
  c.args_count = 0;
  return c;
}

consts init_info(const char *options, const char *regex,
                 const char *options_with_args, char *errors[]) {
  consts info;
  info.options_str = options;
  info.validity_regex = regex;
  info.arguments_options_regex = options_with_args;
  for (int i = 0; i < 3; ++i) {
    info.errors[i] = errors[i];
  }
  return info;
}

void add_option(command *utility, char name) {
  utility->current_options = realloc(utility->current_options,
                                     (utility->opt_count + 1) * sizeof(option));
  utility->current_options[utility->opt_count++] = init_option(name);
}

void add_utility_argument(command *utility, const char *arg) {
  utility->args = realloc(utility->args, (utility->args_count + 1));
  utility->args[utility->args_count++] = arg;
}

void clear_args(command c) {
  for (int i = 0; i < 10; ++i) {
    c.args[i] = NULL;
  }
}

option *last_option(command *utility) {
  return &(utility->current_options[utility->opt_count - 1]);
}

option init_option(char name) {
  option opt;
  opt.name = name;
  opt.argument = NULL;
  return opt;
}