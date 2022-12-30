#include "s21_grep.h"
#define ERROR -1
#define SUCCESS 1
#define FAIL 0

int main(int argc, char *argv[]) {
  if (argc < 3) {  // exe name + pattern[s] + file[s]
    fputs("Not enough options\n", stderr);
    return EXIT_FAILURE;
  }

  t_options s_flags = {0};
  char patternE[BUFFER_SIZE] = {0};

  if (parse_options(argc, argv, &s_flags, patternE)) {
    grep(&s_flags, argc, argv, patternE);
  }

  return EXIT_SUCCESS;
}

int parse_options(int argc, char *argv[], t_options *s_flags, char *patternE) {
  for (int opt = 0; (opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1;) {
    switch (opt) {
      case 'e':
        s_flags->e = 1;
        snprintf(patternE, BUFFER_SIZE, "%s", optarg);
        break;
      case 'i':
        s_flags->i = 1;
        break;
      case 'v':
        s_flags->v = 1;
        break;
      case 'c':
        s_flags->c = 1;
        break;
      case 'l':
        s_flags->l = 1;
        break;
      case 'n':
        s_flags->n = 1;
        break;
      case 'h':
        s_flags->h = 1;
        break;
      case 's':
        s_flags->s = 1;
        break;
      case 'f':
        s_flags->f = 1;
        snprintf(patternE, BUFFER_SIZE, "%s", optarg);
        break;
      case 'o':
        s_flags->o = 1;
        break;
      default:
        fprintf(stderr, "illegal option -- %c\n", opt);
        return FAIL;
    }
  }
  return SUCCESS;
}

void processing(t_options *s_flags, FILE *fp, regex_t reg, char *file) {
  char text[BUFFER_SIZE] = {0};
  regmatch_t pmatch[1];
  int line_matches = 0, nline = 1;
  while (fgets(text, BUFFER_SIZE - 1, fp) != NULL) {
    int match = 0;
    int success = regexec(&reg, text, 1, pmatch, 0);
    if (strchr(text, '\n') == NULL) {
      strcat(text, "\n");
    }
    if (success == 0 && !s_flags->v) {
      match = 1;
    }
    if (success == REG_NOMATCH && s_flags->v) {
      match = 1;
    }
    if (match && !s_flags->l && !s_flags->c && s_flags->n) {
      printf("%d:", nline);
    }
    if (match && !s_flags->l && !s_flags->c && !s_flags->o) {
      printf("%s", text);
    }
    if (match && s_flags->o) {
      for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
        printf("%c", text[i]);
      }
      printf("\n");
    }
    line_matches += match;
    nline++;
  }
  if (s_flags->l && line_matches > 0) {
    printf("%s\n", file);
  }
  if (s_flags->c && !s_flags->l) {
    printf("%d\n", line_matches);
  }
}

void grep_file(t_options *s_flags, char *pattern, char *filename) {
  int cflags = REG_EXTENDED;
  regex_t reg;
  FILE *file;
  file = fopen(filename, "r");
  if (s_flags->i) {
    cflags = REG_ICASE;
  }
  if (file != NULL) {
    regcomp(&reg, pattern, cflags);
    processing(s_flags, file, reg, filename);
    regfree(&reg);
    fclose(file);
  }
}

int read_regex_from_file(char *pattern, char *filename) {
  FILE *file = NULL;
  file = fopen(filename, "r");

  if (file == NULL) {
    printf("error\n");
    return ERROR;
  }

  int i = 0;
  for (char ch; (ch = fgetc(file)) != EOF;) {
    if (ch == '\n') {
      pattern[i++] = '|';
    } else {
      pattern[i++] = ch;
    }
  }
  if (pattern[i - 1] == '|') {
    pattern[i - 1] = '\0';
  }

  fclose(file);
  return i;
}

void grep(t_options *s_flags, int argc, char *argv[], char buff[]) {
  char pattern[BUFFER_SIZE] = {0};
  int f_patt = 0;
  if (!s_flags->f && !s_flags->e) {
    snprintf(pattern, BUFFER_SIZE, "%s", argv[optind++]);
  }
  if (s_flags->f) {
    f_patt = read_regex_from_file(pattern, buff);
  }
  if (!s_flags->f && s_flags->e) {
    snprintf(pattern, BUFFER_SIZE, "%s", buff);
  }
  if (f_patt != -1) {
    int file_c = 0;
    if (argc - optind > 1) {
      file_c = 1;
    }
    for (int i = optind; i < argc; i++) {
      if (file_c && !s_flags->h && !s_flags->l) {
        printf("%s:", argv[i]);
      }
      grep_file(s_flags, pattern, argv[i]);
    }
  }
}
