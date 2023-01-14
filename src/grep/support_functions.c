#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "typedefs.h"
#include "utils.h"

#define REGERROR_BUF 128

void print_usage(void) {
  fprintf(stderr,
          "Usage: ./%s [OPTION]... PATTERNS [FILE]...\n"
          "Try 'grep --help' for more information.\n",
          PROGRAM_NAME);
}

bool arg_is_pattern_option(char arg[]) {
  return arg[0] == '-' &&                         //
         (strchr(arg, 'f') || strchr(arg, 'e'));  //
}

void print_opterror(char arg[]) {
  char message[35] = "Option requires an argument -- ";
  sprintf(strchr(message, '\0'), "'%c'", strchr(arg, 'f') ? 'f' : 'e');
  print_error(message);
  print_usage();
}

bool arguments_are_enough(int argc, char *argv[], t_info *re_pattern) {
  bool pattern_from_argv = false;
  bool pattern_from_option = false;
  bool file_given = false;
  bool result = true;

  int i = 1;
  while (i < argc) {
    if (arg_is_pattern_option(argv[i])) {
      if (i + 1 == argc) {
        print_opterror(argv[i]);
        result = false;
      }
      if (pattern_from_argv) {
        pattern_from_argv = false;
        file_given = true;
      }
      pattern_from_option = true;
      re_pattern->specified_through_option = true;
      i += 2;
      continue;
    }
    if (!pattern_from_option &&  //
        !pattern_from_argv) {
      pattern_from_argv = true;
      ++i;
      continue;
    }
    file_given = true;
    ++i;
  }

  if (!pattern_from_option && !pattern_from_argv) {
    print_usage();
    result = false;
  } else if (!file_given) {
    print_error("File was not specified_through_option");
    result = false;
  }
  return result;
}

bool more_than_one_file(int argc, int optind) { return argc - optind > 1; }

bool is_line_empty(char *line) { return strlen(line) == 1 && line[0] == '\n'; }

bool is_pattern_dot(t_info *re_pattern) {
  char *str = re_pattern->str;
  size_t len = strlen(str);
  char *pattern;

  // dot only
  pattern = ".";
  if (len == 1 && str[0] == *pattern) {
    return true;
  }
  // begginning of the cumulative pattern string
  pattern = ".\\|";
  if (strstr(str, pattern) != NULL) {
    return true;
  }
  // end of the string
  pattern = "|.";
  if (strstr(str, pattern) != NULL) {
    return true;
  }
  // somewhere in the middle
  pattern = "|.\\|";
  if (strstr(str, pattern) != NULL) {
    return true;
  }

  return false;
}

void print_regerror(regex_t *regex, int result) {
  char *error_message = (char *)malloc(sizeof(char) * REGERROR_BUF);
  regerror(result, regex, error_message, REGERROR_BUF);
  fprintf(stderr, "%s: RegEx compilation error: %s\n", PROGRAM_NAME,
          error_message);
  free(error_message);
}
