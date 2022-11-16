/* This file was automatically generated.  Do not edit! */
#undef INTERFACE
void print_error_illegal_option(char *argv[], int invalid_len, char *invalid_opt);
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct {
    bool opt_b_number_nonblank;
    /* macOS-specific option, equivalent to E + v combination in GNU versioin */
    // bool opt_e_Ev_equiv; // redundant without OS detection
    bool opt_E_show_EOLs;
    bool opt_n_number_lines;
    bool opt_s_squeeze_blank;
    /* macOS-specific option, equivalent to T + v combination in GNU version */
    // bool opt_t_Tv_equiv; // redundant without OS detection
    bool opt_T_show_tabs;
    bool opt_v_show_nonprinting;
} s_options;
bool parse_options(int argc, char *argv[], s_options *flags, int *filename_was_given);
