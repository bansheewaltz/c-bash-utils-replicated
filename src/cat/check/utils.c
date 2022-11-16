#include "utils.h"

void print_error_no_file(char *argv[], int file_ndx) {
    fprintf(stderr, "%s: %s: No such file or directory\n", BIN_NAME, argv[file_ndx]);
}

void print_error_illegal_option(char *argv[], int invalid_len, char *invalid_opt) {
    fprintf(stderr, "%s: illegal option -- %.*s\n", BIN_NAME, invalid_len, invalid_opt);
    fprintf(stderr,
            "usage: ./%s "
            "[-[beEnstTv] | --[number-nonblank|number|squeeze-blank]] "
            "[file...]\n",
            BIN_NAME);
}
