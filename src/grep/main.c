#include "main.h"

int main(int argc, char *options[]) {
  command_info info = init_info(OPTIONS_STR, GREP_OPTIONS_REGEX);
  for (int i = 1; i < argc; i++) {
    check_option(options[i], info);
  }
}