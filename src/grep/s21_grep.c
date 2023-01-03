// grep utility program realized with static array of patterns
// combination of patterns implemented as one cumulative array of pattern
// strings by delimiting singular patterns with pipe symbol '|' (the GNU way)
// tests output matches Ubuntu grep utility
#define _GNU_SOURCE
#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR -1
#define SUCCESS 0
#define PATTERN_BUF 4096
#define REGERROR_BUF 128

const char *PROGRAM_NAME;

typedef struct {
  bool e;  // add pattern;
  bool i;  // ignore case;
  bool v;  // invert result;
  bool c;  // count only;
  bool l;  // show filenames only;
  bool n;  // number matched lines;
  bool h;  // show without filenames;
  bool s;  // supress errors;
  bool f;  // read patterns from file;
  bool o;  // output matched parts;
  bool show_filenames;
} t_options;

typedef struct {
  char *str;
  int len;
  int len_limit;
  bool pattern_specified;
  int regex_flag;
} t_info;

void print_usage(void) {
  fprintf(stderr,
          "Usage: %s [OPTION]... PATTERNS [FILE]...\n"
          "Try 'grep --help' for more information.\n",
          PROGRAM_NAME);
}

void delete_delim(t_info *pattern_info) {
  pattern_info->str[--pattern_info->len] = '\0';
  pattern_info->str[--pattern_info->len] = '\0';
}

void add_delim(t_info *pattern_info) {
  pattern_info->str[pattern_info->len++] = '\\';
  pattern_info->str[pattern_info->len++] = '|';
}

int add_pattern_from_file(t_info *pattern_info, char *filename) {
  int result = SUCCESS;
  int *pattern_str_len = &pattern_info->len;
  char *pattern_str = pattern_info->str;

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "s21_grep: %s: %s\n", filename, strerror(errno));
    result = ERROR;
  } else {
    int *i = pattern_str_len;
    if (*pattern_str_len != 0) {
      add_delim(pattern_info);
    }
    for (signed char ch, prev = '\0'; (ch = getc(file)) != EOF;) {
      if (ch == '\n' && *i > 0) {
        if (prev == '\n') {
          pattern_str[(*i)++] = ch;
        }
        if (pattern_str[*i - 1] != '|') {
          add_delim(pattern_info);
        }
      } else if (ch != '\n') {
        pattern_str[(*i)++] = ch;
      }
      prev = ch;
    }
    if (pattern_str[*i - 1] == '|') {
      delete_delim(pattern_info);
    }
    pattern_info->pattern_specified = true;
    fclose(file);
  }

  return result;
}

void add_pattern(t_info *pattern_info, char *pattern) {
  int pattern_len = strlen(pattern);
  int *pattern_str_len = &pattern_info->len;
  char *pattern_str = pattern_info->str;

  if (*pattern_str_len != 0) {
    add_delim(pattern_info);
  }
  for (int i = 0; i < pattern_len; ++i) {
    pattern_str[(*pattern_str_len)++] = pattern[i];
  }
  pattern_str[*pattern_str_len] = '\0';
}

int parse_options(int argc, char *argv[], t_options *flags,
                  t_info *pattern_info) {
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
      default:
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

void strslice(const char *str, char *result, size_t slice_len, size_t start,
              size_t end) {
  memset(result, '\0', slice_len);
  strncpy(result, str + start, end - start);
}

bool pattern_is_dot(t_info *pattern_info) {
  char *str = pattern_info->str;
  size_t len = strlen(str);

  if (len == 1 && str[0] == '.') {
    return true;
  }

  bool result = false;
  const size_t slice_len = 8;
  char *slice = (char *)malloc(sizeof(char) * slice_len);
  // check begginning of the string
  char *pattern = ".\\|";
  strslice(str, slice, slice_len, 0, strlen(pattern));
  if (strcmp(slice, pattern) == 0) {
    result = true;
  }
  // in the end
  if (result == false) {
    pattern = "|.";
    strslice(str, slice, slice_len, len - strlen(pattern), len);
    if (strcmp(slice, pattern) == 0) {
      result = true;
    }
  }
  // somewhere in the middle
  if (result == false) {
    for (int i = 1; i + strlen(pattern) < len; ++i) {
      pattern = "|.\\|";
      strslice(str, slice, slice_len, i, i + strlen(pattern));
      if (strcmp(slice, pattern) == 0) {
        result = true;
      }
    }
  }

  free(slice);
  return result;
}

bool line_empty(char *line) {
  return strlen(line) == 1 && line[0] == '\n';
}

void scan_file(FILE *file, char *filename, t_options *flags, regex_t *regex,
               t_info *pattern_info) {
  char *line = NULL;
  size_t len = 0;
  regmatch_t regmatch;
  int matched_n = 0;
  int line_ndx = 0;  // +1 -- human readable
  bool dot_pattern = pattern_is_dot(pattern_info);

  while (getline(&line, &len, file) > 0) {
    ++line_ndx;
    int result = regexec(regex, line, 1, &regmatch, 0);
    if ((result == SUCCESS && !flags->v) ||         //
        (result == REG_NOMATCH && flags->v)) {      //
      if (0                                         //
          || !dot_pattern                           //
          || (dot_pattern && !line_empty(line))) {  //
        matched_n++;
      }
      if (0                                        //
          || flags->l                              //
          || flags->c                              //
          || (flags->v && flags->o)                //
          || (dot_pattern && line_empty(line))) {  //
        continue;
      }
      line_related_output(line, flags, &regmatch, line_ndx, filename);

      char *remaining = line + regmatch.rm_eo;
      while (flags->o &&
             regexec(regex, remaining, 1, &regmatch, 0) == SUCCESS) {
        line_related_output(remaining, flags, &regmatch, line_ndx, filename);
        remaining = remaining + regmatch.rm_eo;
      }
    }
  }

  if (flags->c) {
    if (flags->show_filenames) {
      printf("%s:", filename);
    }
    printf("%d\n", matched_n);
  } else if (flags->l && matched_n) {
    printf("%s\n", filename);
  }
  free(line);
}

void cook_search(int argc, char *argv[], t_options *flags,
                 t_info *pattern_info) {
  char *pattern_str = pattern_info->str;
  int regex_flag = pattern_info->regex_flag;
  regex_t regex;
#ifdef DEBUG
  // pattern_str = "((\\((^";
#endif

  int result = regcomp(&regex, pattern_str, regex_flag);
  if (result == SUCCESS) {
    for (char **filename = &argv[optind]; filename != &argv[argc]; ++filename) {
      FILE *file = fopen(*filename, "r");
      if (file != NULL) {
        scan_file(file, *filename, flags, &regex, pattern_info);
        fclose(file);
      } else if (flags->s == false) {
        fprintf(stderr, "s21_grep: %s: %s\n", *filename, strerror(errno));
      }
    }
  } else {
    char *error_message = (char *)malloc(sizeof(char) * REGERROR_BUF);
    regerror(result, &regex, error_message, REGERROR_BUF);
    fprintf(stderr, "%s: RegEx compilation error: %s\n", PROGRAM_NAME,
            error_message);
    free(error_message);
  }

  regfree(&regex);
}

bool file_is_specified(int argc) {
  return optind < argc;
}

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

bool more_than_one_file(int argc) {
  return argc - optind > 1;
}

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

int main(int argc, char *argv[]) {
  PROGRAM_NAME = argv[0];
  char pattern_str[PATTERN_BUF] = "";
  t_options flags = {0};
  t_info pattern_info = {.str = pattern_str,         //
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
      printf("cumulative pattern string: \"%s\"\n\n", pattern_info.str);
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
