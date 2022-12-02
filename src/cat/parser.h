#ifndef SRC_CAT_PARSER_H_
#define SRC_CAT_PARSER_H_

#include "structs.h"

bool parse_options(int argc, char *argv[], s_options *flags,
                   int *filename_was_given);
void print_error_no_file(char *argv[], int file_ndx);

#endif  // SRC_CAT_PARSER_H
