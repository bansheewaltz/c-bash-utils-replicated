/* grep utility program
** * realized with static array of patterns
** * combination of patterns implemented as one cumulative array of pattern
**   strings by delimiting singular patterns with pipe symbol '|' (the GNU way)
** * tests output matches Ubuntu grep utility */
#include "s21_grep.h"

const char *PROGRAM_NAME;

void print_usage(void) {
  fprintf(stderr,
          "Usage: ./%s [OPTION]... PATTERNS [FILE]...\n"
          "Try 'grep --help' for more information.\n",
          PROGRAM_NAME);
}

void add_delim(t_info *pattern_info) {
  pattern_info->str[pattern_info->len++] = '\\';
  pattern_info->str[pattern_info->len++] = '|';
}

void add_line(t_info *pattern_info, char *str, size_t real_len) {
  int *re_len = &pattern_info->len;
  char *re_pattern = pattern_info->str;

  if (*re_len != 0) {
    add_delim(pattern_info);
  }
  real_len = strlen(str);
  if (str[real_len - 1] == '\n') {
    str[--real_len] = '\0';
  }
  strcat(re_pattern, str);
  *re_len += real_len;
}

int add_pattern_from_file(t_info *pattern_info, char *filename) {
  int result = SUCCESS;

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "s21_grep: %s: %s\n", filename, strerror(errno));
    result = ERROR;
  } else {
    char *str = NULL;
    size_t len = 0;
    size_t real_len = 0;
    while (getline(&str, &len, file) > 0) {
      add_line(pattern_info, str, real_len);
    }
    free(str);
    fclose(file);
  }

  return result;
}

void add_pattern(t_info *pattern_info, char *pattern) {
  int pattern_len = strlen(pattern);
  int *re_len = &pattern_info->len;
  char *re_pattern = pattern_info->str;

  if (*re_len != 0) {
    add_delim(pattern_info);
  }
  for (int i = 0; i < pattern_len; ++i) {
    re_pattern[(*re_len)++] = pattern[i];
  }
  re_pattern[*re_len] = '\0';
}

int parse_options(int argc, char *argv[], t_options *flags,
                  t_info *pattern_info) {
  opterr = 0;
  for (int opt = 0; (opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1;) {
    switch (opt) {
      case 'e':
        flags->e = true;
        add_pattern(pattern_info, optarg);
        pattern_info->pattern_specified = true;
        break;
      case 'i':
        flags->i = true;
        pattern_info->regex_flag |= REG_ICASE;
        break;
      case 'v':
        flags->v = true;
        break;
      case 'c':
        flags->c = true;
        break;
      case 'l':
        flags->l = true;
        break;
      case 'n':
        flags->n = true;
        break;
      case 'h':
        flags->h = true;
        break;
      case 's':
        flags->s = true;
        break;
      case 'f':
        flags->f = true;
        if (add_pattern_from_file(pattern_info, optarg) != SUCCESS) {
          return ERROR;
        }
        pattern_info->pattern_specified = true;
        break;
      case 'o':
        flags->o = true;
        break;
      case '?':
        fprintf(stderr, "%s: invalid option -- '%c'\n", PROGRAM_NAME, optopt);
        print_usage();
        return ERROR;
    }
  }
  return SUCCESS;
}

void line_related_output(char *line, t_options *flags, regmatch_t *regmatch,
                         int line_ndx, char *filename) {
  if (flags->show_filenames) {
    printf("%s:", filename);
  }
  if (flags->n) {
    printf("%d:", line_ndx);
  }
  if (flags->o) {
    printf("%.*s\n", (int)(regmatch->rm_eo - regmatch->rm_so),
           line + regmatch->rm_so);
  } else {
    printf("%s", line);
    if (line[strlen(line) - 1] != '\n') {
      putchar('\n');
    }
  }
}

bool is_pattern_dot(t_info *pattern_info) {
  char *str = pattern_info->str;
  size_t len = strlen(str);
  char *pattern = ".";

  // dot only
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

bool is_line_empty(char *line) { return strlen(line) == 1 && line[0] == '\n'; }

void file_related_output(t_options *flags, char *filename, int matched_n) {
  if (flags->c) {
    if (flags->show_filenames) {
      printf("%s:", filename);
    }
    printf("%d\n", matched_n);
  } else if (flags->l && matched_n) {
    printf("%s\n", filename);
  }
}

bool no_line_output(t_options *flags, bool dot_pattern, bool line_empty) {
  return flags->l ||                   //
         flags->c ||                   //
         (flags->v && flags->o) ||     //
         (dot_pattern && line_empty);  //
}

void handle_option_o(t_re *re, t_options *flags, char *line, int line_ndx,
                     char *filename) {
  char *remaining = line + re->regmatch->rm_eo;
  while (regexec(re->regex, remaining, 1, re->regmatch, 0) == SUCCESS) {
    line_related_output(remaining, flags, re->regmatch, line_ndx, filename);
    remaining = remaining + re->regmatch->rm_eo;
  }
}

void process_file(FILE *file, char *filename, t_options *flags, t_re *re) {
  char *line = NULL;
  size_t len = 0;
  int matched_n = 0;
  int line_ndx = 0;  // +1 -- human readable

  while (getline(&line, &len, file) > 0) {
    ++line_ndx;
    int result = regexec(re->regex, line, 1, re->regmatch, 0);

    if ((result == SUCCESS && !flags->v) ||
        (result == REG_NOMATCH && flags->v)) {
      bool line_empty = is_line_empty(line);

      if (!re->dot_pattern || !line_empty) {
        matched_n++;
      }
      if (no_line_output(flags, re->dot_pattern, line_empty)) {
        continue;
      }
      line_related_output(line, flags, re->regmatch, line_ndx, filename);
      if (flags->o) {
        handle_option_o(re, flags, line, line_ndx, filename);
      }
    }
  }
  free(line);
  file_related_output(flags, filename, matched_n);
}

void print_regerror(regex_t *regex, int result) {
  char *error_message = (char *)malloc(sizeof(char) * REGERROR_BUF);
  regerror(result, regex, error_message, REGERROR_BUF);
  fprintf(stderr, "%s: RegEx compilation error: %s\n", PROGRAM_NAME,
          error_message);
  free(error_message);
}

void cook_search(int argc, char *argv[], t_options *flags,
                 t_info *pattern_info) {
  char *re_pattern = pattern_info->str;
  int regex_flag = pattern_info->regex_flag;
  t_re re = {.regex = &(regex_t){},
             .regmatch = &(regmatch_t){},
             .dot_pattern = is_pattern_dot(pattern_info)};
#ifdef DEBUG
  // re_pattern = "((\\((^";
#endif

  int result = regcomp(re.regex, re_pattern, regex_flag);
  if (result == SUCCESS) {
    for (char **filename = &argv[optind]; filename != &argv[argc]; ++filename) {
      FILE *file = fopen(*filename, "r");
      if (file != NULL) {
        process_file(file, *filename, flags, &re);
        fclose(file);
      } else if (flags->s == false) {
        fprintf(stderr, "%s: %s: %s\n", PROGRAM_NAME, *filename,
                strerror(errno));
      }
    }
  } else {
    print_regerror(re.regex, result);
  }
  regfree(re.regex);
}

bool file_is_specified(int argc) { return optind < argc; }

void add_argv_pattern(char *argv[], t_info *pattern_info) {
  memcpy(pattern_info->str, argv[optind], strlen(argv[optind]));
  pattern_info->len = strlen(argv[optind]);
  pattern_info->pattern_specified = true;
  ++optind;
}

bool arguments_are_enough(int argc, t_info *pattern_info) {
  return ((argc - optind >= 0) && pattern_info->pattern_specified) ||
         ((argc - optind >= 1) && !pattern_info->pattern_specified);
}

bool more_than_one_file(int argc) { return argc - optind > 1; }

void options_combination_resolution(int argc, t_options *flags) {
  if (more_than_one_file(argc) && !flags->h) {
    flags->show_filenames = true;
  }
  if (flags->h && flags->l) {
    flags->h = false;
  }
  if (flags->c && flags->l) {
    flags->c = false;
  }
}

void set_prog_name(const char *argv[]) {
  int i = strlen(argv[0]) - 1;
  while (argv[0][i] != '/') {
    --i;
  }
  ++i;
  PROGRAM_NAME = &(argv[0][i]);
}

int main(int argc, char *argv[]) {
  set_prog_name((const char **)argv);
  char re_pattern[PATTERN_BUF] = "";
  t_options flags = {0};
  t_info pattern_info = {.str = re_pattern,          //
                         .len = 0,                   //
                         .len_limit = PATTERN_BUF};  //

  if (parse_options(argc, argv, &flags, &pattern_info) == SUCCESS) {
    if (!arguments_are_enough(argc, &pattern_info)) {
      fprintf(stderr, "%s: Pattern was not specified\n", PROGRAM_NAME);
    } else {
      if (pattern_info.pattern_specified == false) {
        add_argv_pattern(argv, &pattern_info);
      }
#ifdef DEBUG
      printf("RE cumulative pattern string: \"%s\"\n\n", pattern_info.str);
#endif
      if (file_is_specified(argc)) {
        options_combination_resolution(argc, &flags);
        cook_search(argc, argv, &flags, &pattern_info);
      } else {
        fprintf(stderr, "%s: File was not specified\n", PROGRAM_NAME);
      }
    }
  }

  return EXIT_SUCCESS;
}
