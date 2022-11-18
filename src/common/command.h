#ifndef C3_SIMPLEBASHUTILS_0_FUNC_H
#define C3_SIMPLEBASHUTILS_0_FUNC_H


typedef struct {
  char name;
  const char *argument;
} option;

// Option implementation
typedef struct {
  void *addr;
  const char *option;
  int priority;
} func;

// Terminal utility representation
typedef struct {
  func *options_funcs;
  int size;
  const char *name;
  char *args[10];
} command;


// Info about current usage of utility
typedef struct {
  const char *options_str;
  const char *validity_regex;
  const char *arguments_options_regex;
  option *current_options;
  int count;
} command_info;

command init_command(const char *name, func funcs[], int size);
command_info init_info(const char *options, const char *regex);

void add_option(command_info* info, char name);
option init_option(char name);
option* last_option(command_info* info);

void clear_args(command c);

#endif // C3_SIMPLEBASHUTILS_0_FUNC_H
