#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#define BIN_NAME (argv[0] + 2)

static void validate_option(s_options *flags, char arg[], char **invalid_opt,
                            int *invalid_len) {
  if (*arg == '-' && *arg++) {
    if (strcmp(arg, "number-nonblank") == 0) {
      flags->opt_b_number_nonblank = true;
    } else if (strcmp(arg, "number") == 0) {
      flags->opt_n_number_lines = true;
    } else if (strcmp(arg, "squeeze-blank") == 0) {
      flags->opt_s_squeeze_blank = true;
    } else {
      *invalid_opt = arg;
      *invalid_len = -1;  // as string precision specifier sets no size limit
    }
  } else {
    for (char *char_p = arg; !*invalid_opt && *char_p; ++char_p) {
      if (*char_p == 'b') {
        flags->opt_b_number_nonblank = true;
      } else if (*char_p == 'e') {
        flags->opt_E_show_EOLs = true;
        flags->opt_v_show_nonprinting = true;
      } else if (*char_p == 'E') {
        flags->opt_E_show_EOLs = true;
      } else if (*char_p == 'n') {
        flags->opt_n_number_lines = true;
      } else if (*char_p == 's') {
        flags->opt_s_squeeze_blank = true;
      } else if (*char_p == 't') {
        flags->opt_T_show_tabs = true;
        flags->opt_v_show_nonprinting = true;
      } else if (*char_p == 'T') {
        flags->opt_T_show_tabs = true;
      } else if (*char_p == 'v') {
        flags->opt_v_show_nonprinting = true;
      } else {
        *invalid_opt = char_p;
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
    if (argv[i][0] == '-') {
      ++opt_c;
    }
  }
  file_c ? (*filename_was_given = opt_c + 1) : 0;
}

static void options_conflict_resolution(s_options *flags) {
  if (flags->opt_b_number_nonblank == true) {  // options сonflict resolution
    flags->opt_n_number_lines = false;
  }
}

static void print_error_illegal_option(char const *argv[], int invalid_len,
                                       char *invalid_opt) {
  fprintf(stderr, "%s: illegal option -- %.*s\n", BIN_NAME, invalid_len,
          invalid_opt);
  fprintf(stderr,
          "usage: ./%s "
          "[-[beEnstTv] | --[(number-nonblank)|(number)|(squeeze-blank)]] "
          "[file...]\n",
          BIN_NAME);
}

bool parse_options(int argc, char *argv[], s_options *flags,
                   int *filename_was_given) {
  char *invalid_opt = NULL;
  int invalid_len = 1;  // assume that a chance of a short invalid option
                        // occurrence is higher;
  sort_argv(
      argc, argv,
      filename_was_given);  // in a case if there are some options
                            // that were passed after filenames (GNU feature)
  for (int arg_ndx = 1; arg_ndx < argc && *argv[arg_ndx] == '-' && !invalid_opt;
       ++arg_ndx) {
    validate_option(flags, ++argv[arg_ndx], &invalid_opt, &invalid_len);
  }

  if (!invalid_opt) {
    options_conflict_resolution(flags);
  } else {
    print_error_illegal_option((const char **)argv, invalid_len, invalid_opt);
  }

  return invalid_opt ? false : true;
}

void print_error_no_file(char *argv[], int file_ndx) {
  fprintf(stderr, "%s: %s: No such file or directory\n", BIN_NAME,
          argv[file_ndx]);
}
