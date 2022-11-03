#ifndef OPTIONS_H
#define OPTIONS_H

#define COMMAND "s21_cat:", "s21_grep:"
#define CAT_OPTIONS_REGEX \
  "(--number-nonblank$|--number$|--squeeze-blank$|^[^-]*-?[beEnstTuv]+$)+"

#define ILLEGAL_OPTION ": illegal option -- "
#define NO_SUCH_FILE ": No such file or directory"
#define REGEX "beEnstTuv", "--number-nonblank", "--number", "--squeeze-blank"
#define POSTPROCCESS ""

#include "regex.h"

#endif  // OPTIONS_H
