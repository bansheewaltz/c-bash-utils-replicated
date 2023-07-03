/* grep Unix[-like] utility program
** * the ability of specifying several patterns implemented
**   as a one cumulative array of pattern strings
**   where each pattern is delimited with a pipe symbol '|' (the GNU way)
** * tests output matches Ubuntu grep utility
*/
#define _GNU_SOURCE  // getline function
#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR -1
#define SUCCESS 0
#define PATTERN_INITIAL_BUF 16
#define DELIM_LEN 2
// codes for add_pattern function
#define FROM_FILE 0
#define FROM_E 1

#include "support_functions.h"
#include "utils.h"

const char *PROGRAM_NAME;

int increase_re_capacity(t_info *re_pattern, int size) {
  int new_capacity;
  void *tmp = NULL;

  if (re_pattern->capacity * 1.5 >= re_pattern->capacity + size) {
    new_capacity = re_pattern->capacity * 1.5;
    tmp = realloc(re_pattern->str, new_capacity);
  }

  if (tmp == NULL) {
    new_capacity = re_pattern->capacity + size;
    tmp = realloc(re_pattern->str, new_capacity);
    if (tmp == NULL) {
      return ERROR;
    }
  }
  re_pattern->capacity = new_capacity;
  re_pattern->str = tmp;

  return SUCCESS;
}

int add_pattern_to_re(t_info *re_pattern, char *pattern) {
  int pattern_len = strlen(pattern);
  int request_size = DELIM_LEN + pattern_len;

  if (re_pattern->capacity <= re_pattern->len + request_size) {
    if (increase_re_capacity(re_pattern, request_size + 1) != SUCCESS) {
      print_error("Heap memory overflow");
      return ERROR;
    }
  }

  if (re_pattern->len != 0) {
    add_delim_unsafe(re_pattern->str, &re_pattern->len, "\\|");
  }
  strcat(re_pattern->str, pattern);
  re_pattern->len += pattern_len;

  return SUCCESS;
}

int add_pattern_from_file(t_info *re_pattern, char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "s21_grep: %s: %s\n", filename, strerror(errno));
    return ERROR;
  }

  int result = ERROR;
  char *line = NULL;
  size_t capacity;
  int line_len;
  while ((line_len = getline(&line, &capacity, file)) > 0) {
    if (line[line_len - 1] == '\n') {
      line[--line_len] = '\0';
    }
    if ((result = add_pattern_to_re(re_pattern, line)) != SUCCESS) {
      break;
    }
  }
  free(line);
  fclose(file);

  return result;
}

int add_pattern(t_info *re_pattern, char *optarg, int arg) {
  if (arg != FROM_FILE && arg != FROM_E) {
    return ERROR;
  }

  int result = SUCCESS;
  if (re_pattern->capacity == 0) {
    char *tmp = (char *)calloc(PATTERN_INITIAL_BUF, sizeof(char));
    if (tmp != NULL) {
      re_pattern->str = tmp;
      re_pattern->capacity = PATTERN_INITIAL_BUF;
    } else {
      result = ERROR;
    }
  }

  if (result == SUCCESS) {
    if (arg == FROM_FILE) {
      result = add_pattern_from_file(re_pattern, optarg);
    }
    if (arg == FROM_E) {
      result = add_pattern_to_re(re_pattern, optarg);
    }
    if (result == ERROR) {
      free(re_pattern->str);
    }
  }

  return result;
}

int reduce_str_capacity_to_fit_len(t_info *re_pattern) {
  if (re_pattern->capacity > re_pattern->len + 1) {
    void *tmp = (char *)realloc(re_pattern->str, re_pattern->len + 1);
    if (tmp != NULL) {
      re_pattern->str = tmp;
    } else {
      return ERROR;
    }
  }

  return SUCCESS;
}

/* getopt_long is used for portability
** because of classic getopt has different implementations
** in standard libraries for different platforms
** while getopt_long realization is universal as a GNU extension
*/
int parse_options(int argc, char *argv[], t_options *flags,
                  t_info *re_pattern) {
  opterr = 0;
  int opt;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (opt) {
      case 'e':
        flags->e = true;
        if (add_pattern(re_pattern, optarg, FROM_E) != SUCCESS) {
          return ERROR;
        }
        break;
      case 'i':
        flags->i = true;
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
        if (add_pattern(re_pattern, optarg, FROM_FILE) != SUCCESS) {
          return ERROR;
        }
        re_pattern->specified_through_option = true;
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

  return reduce_str_capacity_to_fit_len(re_pattern);
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

bool dont_need_line_output(t_options *flags, bool is_pattern_dot,
                           bool line_empty) {
  return flags->l ||                      //
         flags->c ||                      //
         (flags->v && flags->o) ||        //
         (is_pattern_dot && line_empty);  //
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
  size_t capacity = 0;
  int matched_n = 0;
  int line_ndx = 0;  // +1 -- human readable

  while (getline(&line, &capacity, file) > 0) {
    ++line_ndx;
    int result = regexec(re->regex, line, 1, re->regmatch, 0);

    if ((result == SUCCESS && !flags->v) ||
        (result == REG_NOMATCH && flags->v)) {
      bool line_empty = is_line_empty(line);

      if (!re->is_pattern_dot || !line_empty) {
        matched_n++;
      }
      if (dont_need_line_output(flags, re->is_pattern_dot, line_empty)) {
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

void cook_search(int argc, char *argv[], t_options *flags, t_info *re_pattern) {
  char *re_str = re_pattern->str;
  t_re re = {.regex = &(regex_t){},
             .regmatch = &(regmatch_t){},
             .is_pattern_dot = is_pattern_dot(re_pattern)};
#ifdef DEBUG
  // re_str = "((\\((^";
#endif

  int result = regcomp(re.regex, re_str, flags->i ? REG_ICASE : 0);
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

void options_collision_resolution(int argc, t_options *flags) {
  if (more_than_one_file(argc, optind) && !flags->h) {
    flags->show_filenames = true;
  }
  if (flags->h && flags->l) {
    flags->h = false;
  }
  if (flags->c && flags->l) {
    flags->c = false;
  }
}

int main(int const argc, char *argv[]) {
  t_setprogname((char const *)argv[0]);
  t_options flags = {0};
  t_info re_pattern = {0};

  if (arguments_are_enough(argc, argv, &re_pattern) &&
      parse_options(argc, argv, &flags, &re_pattern) == SUCCESS) {
    if (re_pattern.specified_through_option == false) {
      re_pattern.str = argv[optind++];
    }
    options_collision_resolution(argc, &flags);
#ifdef DEBUG
    printf("RE cumulative pattern string: \"%s\"\n\n", re_pattern.str);
#endif
    cook_search(argc, argv, &flags, &re_pattern);
    if (re_pattern.specified_through_option == true) {
      free(re_pattern.str);
    }
  }

  return EXIT_SUCCESS;
}
