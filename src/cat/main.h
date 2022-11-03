#ifndef MAIN_H
#define MAIN_H

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/options.h"
#include "../common/regex.h"

#define COMMAND "s21_cat:"

#define CAT_OPTIONS_REGEX                                                    \
  "(^[-]*--number$|^[-]*--number-nonblank$|^[^-]*--squeeze-blank$|^[^-]*-?[" \
  "beEnstT]+$)+"

#define OPTIONS_STR "beEnstTnumbernumbernonblanksqueezeblank"

#endif