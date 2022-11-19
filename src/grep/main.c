#include "main.h"

typedef int (*preprocess_args_func)(command *, int, char *[], int *);

int main(int count, char *options[]) {
  consts info = init_info(OPTIONS_STR, GREP_OPTIONS_REGEX,
                          OPTIONS_WITH_ARGUMENTS, (char *[]){ERRORS});
  command utility = init_command(COMMAND, NULL, 0, info);
  preprocess_args_func preprocess[2] = {(preprocess_args_func)save_options, (preprocess_args_func)save_arguments};

  int i = 1, error = 0;
  for (int j = 0; j < 2 && !error; j++) {
    error = preprocess[j](&utility, count, options, &i);
  }
  printf("%d %d %d\n", utility.opt_count, utility.args_count,
         utility.func_count);

  for (i = 0; i < utility.opt_count; i++) {
    printf("(%c, %s)\n", utility.current_options[i].name,
           utility.current_options[i].argument);
  }
  for (i = 0; i < utility.args_count; i++) {
    printf("(%s, %s)\n", utility.name, utility.args[i]);
  }
}
