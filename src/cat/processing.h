#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "structs.h"
#define ASCII_SHIFT 64
#define ASCII_SHIFT_BOUND 126

void process_text(FILE *file_p, s_options *flags);