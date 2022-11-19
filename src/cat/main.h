#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "structs.h"
#define FALSE_ALL_BOOLS {false};

void process_text(FILE *file_p, s_options *flags);
void print_error_no_file(char *argv[], int file_ndx);
bool parse_options(int argc, char *argv[], s_options *flags, int *filename_was_given);