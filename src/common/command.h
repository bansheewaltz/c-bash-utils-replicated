#ifndef C3_SIMPLEBASHUTILS_0_FUNC_H
#define C3_SIMPLEBASHUTILS_0_FUNC_H

typedef struct {
  void *addr;
  const char *option;
  int priority;
} func;

typedef struct {
  func *options_funcs;
  int size;
  const char *name;
  char *args[10];
} command;

typedef struct {
  const char *options;
  const char *regex;
} command_info;

command init_command(const char *name, func funcs[], int size);
command_info init_info(const char *options, const char *regex);
void clear_args(command c);

#endif // C3_SIMPLEBASHUTILS_0_FUNC_H
