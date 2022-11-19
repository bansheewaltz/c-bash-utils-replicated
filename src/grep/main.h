#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/command.h"
#include "../common/file.h"
#include "../common/options.h"

#define COMMAND "grep"
#define GREP_OPTIONS_REGEX "^-[eivclnhsf]+$"
#define OPTIONS_STR "eivclnhsf"
#define OPTIONS_WITH_ARGUMENTS "[f]"

#endif // MAIN_H