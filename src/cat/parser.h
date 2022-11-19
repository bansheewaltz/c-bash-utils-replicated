#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "structs.h"
#define BIN_NAME (argv[0] + 2)

void print_error_illegal_option(char const *argv[], int invalid_len,
                                char *invalid_opt);
void options_conflict_resolution(s_options *flags);
void sort_argv(int argc, char *argv[], int *filename_was_given);
void move_arg_to_the_end(int i, int argc, char *argv[]);
void validate_option(s_options *flags, char arg[], char **invalid_opt,
                     int *invalid_len);
bool parse_options(int argc, char *argv[], s_options *flags,
                   int *filename_was_given);
void print_error_no_file(char *argv[], int file_ndx);