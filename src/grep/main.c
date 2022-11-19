#include "main.h"

int main(int count, char *options[]) {
  consts info = init_info(OPTIONS_STR, GREP_OPTIONS_REGEX,
                          OPTIONS_WITH_ARGUMENTS, (char *[]){ERRORS});
  command utility = init_command(COMMAND, NULL, 0, info);
  int i = 1;

  save_options(&utility, count, options, &i);
  i--;
  save_arguments(&utility, count, options, &i);
  printf("%d %d %d\n", utility.opt_count, utility.args_count, utility.func_count);

  for (i = 0; i < utility.opt_count; i++) {
    printf("(%c, %s)\n", utility.current_options[i].name,
           utility.current_options[i].argument);
  }
  for (i = 0; i < utility.args_count; i++) {
    printf("(%s, %s)\n", utility.name,
           utility.args[i]);
  }
}
