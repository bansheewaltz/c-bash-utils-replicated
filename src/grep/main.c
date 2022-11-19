#include "main.h"

int check_for_arguments(command_info *info, char *options[], int count, int *i);

int main(int count, char *options[]) {
  command_info info = init_info(OPTIONS_STR, GREP_OPTIONS_REGEX);
  int error = 0;
  for (int i = 1; i < count && !error; i++) {
    error = !is_options_valid(options[i], &info);
    if (!error) {
      error = check_for_arguments(&info, options, count, &i);
    }
  }

  for (int i = 0; i < info.count; ++i) {
    printf("%c %s", info.current_options[i].name,
           info.current_options[i].argument);
  }
}

int check_for_arguments(command_info *info, char *options[], int count,
                        int *i) {
  if (has_argument(last_option(info), OPTIONS_WITH_ARGUMENTS)) {
    printf("here");
    if (count <= *i + 1) {
      fprintf(stderr, "%s %s", COMMAND, NO_ARGUMENT);
    } else {
      add_argument(last_option(info), options[++(*i)]);
    }
  }
}