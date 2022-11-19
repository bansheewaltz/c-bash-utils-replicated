#ifndef C3_SIMPLEBASHUTILS_0_FUNC_H
#define C3_SIMPLEBASHUTILS_0_FUNC_H

typedef struct utility_option {
  char name;
  const char *argument;
} option;

typedef struct option_function {
  void *addr;
  const char *option;
  int priority;
} func;

typedef struct command_constants {
  const char *options_str;
  const char *validity_regex;
  const char *arguments_options_regex;
  const char *errors[3];
} consts;

typedef struct utility {
  // Options
  option *current_options;
  int opt_count;
  // Functions
  func *options_funcs;
  int func_count;
  // Command name and arguments
  const char *name;
  const char **args;
  int args_count;
  // Info
  consts info;
} command;

command init_command(const char *name, func funcs[], int size, consts info);
consts init_info(const char *options, const char *regex,
                 const char *options_with_args, char *errors[]);

void add_option(command *info, char name);
void add_utility_argument(command *utility, const char *arg);

option init_option(char name);
option *last_option(command *info);

void clear_args(command c);

#endif // C3_SIMPLEBASHUTILS_0_FUNC_H
