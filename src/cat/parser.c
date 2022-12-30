#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_outputs.h"
#include "globals.h"
#include "typedefs.h"

static void validate_option(t_options *s_flags, char arg[], char **invalid_opt,
                            int *invalid_opt_len) {
  if (*arg == '-' && *++arg) {
    if (strcmp(arg, "number-nonblank") == 0) {
      s_flags->opt_b_number_nonblank = true;
    } else if (strcmp(arg, "number") == 0) {
      s_flags->opt_n_number_lines = true;
    } else if (strcmp(arg, "squeeze-blank") == 0) {
      s_flags->opt_s_squeeze_blank = true;
    } else {
      *invalid_opt = arg;
      *invalid_opt_len = -1;
      // as a string precision specifier sets no size limit
    }
  } else {
    for (char *ch = arg; !*invalid_opt && *ch != '\0'; ++ch) {
      switch (*ch) {
        case 'b':
          s_flags->opt_b_number_nonblank = true;
          break;
        case 'e':
          s_flags->opt_E_show_EOLs = true;
          s_flags->opt_v_show_nonprinting = true;
          break;
        case 'E':
          s_flags->opt_E_show_EOLs = true;
          break;
        case 'n':
          s_flags->opt_n_number_lines = true;
          break;
        case 's':
          s_flags->opt_s_squeeze_blank = true;
          break;
        case 't':
          s_flags->opt_T_show_tabs = true;
          s_flags->opt_v_show_nonprinting = true;
          break;
        case 'T':
          s_flags->opt_T_show_tabs = true;
          break;
        case 'v':
          s_flags->opt_v_show_nonprinting = true;
          break;
        default:
          *invalid_opt = ch;
      }
    }
  }
}

static void move_arg_to_the_end(int i, int argc, char *argv[]) {
  char *t = argv[i];
  for (int j = i; j < argc - 1; ++j) {
    argv[j] = argv[j + 1];
  }
  argv[argc - 1] = t;
}

static void sort_argv(int argc, char *argv[], int *filename_was_given) {
  int opt_c = 0;
  int file_c = 0;

  for (int i = 1; opt_c + file_c < argc - 1; ++i) {
    while (argv[i][0] != '-' && opt_c + file_c < argc - 1) {
      move_arg_to_the_end(i, argc, argv);
      ++file_c;
    }
    if (argv[i][0] == '-') ++opt_c;
  }
  file_c ? (*filename_was_given = opt_c + 1) : 0;
}

static void options_conflict_resolution(t_options *s_flags) {
  if (s_flags->opt_b_number_nonblank == true)
    s_flags->opt_n_number_lines = false;
}

bool parse_options(int argc, char *argv[], t_options *s_flags,
                   int *filename_was_given) {
  char *invalid_opt = NULL;
  int invalid_opt_len = 1;  // assume that a chance of a short invalid
                            // option occurrence is higher;
  sort_argv(argc, argv, filename_was_given);
  // in a case if there are some options
  // that were passed after filenames (GNU feature)
  for (int arg_ndx = 1;                                          //
       arg_ndx < argc && *argv[arg_ndx] == '-' && !invalid_opt;  //
       ++arg_ndx) {                                              //
    validate_option(s_flags, ++argv[arg_ndx], &invalid_opt, &invalid_opt_len);
  }

  if (!invalid_opt) {
    options_conflict_resolution(s_flags);
  } else {
    print_error_illegal_option(invalid_opt_len, invalid_opt);
  }

  return invalid_opt ? false : true;
}
