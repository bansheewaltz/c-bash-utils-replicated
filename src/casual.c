// Implementation of Unix "cat" utility
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    bool b_number_nonblank;
    /* macOS-specific option, equivalent to E + v combination in GNU */
    // bool e_Ev_equiv; // redundant without OS detection
    bool E_show_EOLs;
    bool n_number_lines;
    bool s_squeeze_blank;
    /* macOS-specific option, equivalent to T + v combination in GNU */
    // bool t_Tv_equiv; // redundant without OS detection
    bool T_show_tabs;
    bool v_show_nonprinting;
} options;

bool validate_long_option(options *flags, char *arg) {
    bool is_all_good = true;

    if (strcmp(arg, "number-nonblank") == 0) {
        flags->b_number_nonblank = true;
        flags->n_number_lines = false;
    } else if (strcmp(arg, "number") == 0) {
        if (flags->b_number_nonblank == false) {
            flags->n_number_lines = true;
        }
    } else if (strcmp(arg, "squeeze-blank") == 0) {
        flags->s_squeeze_blank = true;
    } else {
        fprintf(stderr, "s21_cat: illegal option -- %s\n", arg);
        is_all_good = false;
    }

    return is_all_good;
}

bool validate_option(options *flags, char *arg) {
    bool is_all_good = true;

    if (*arg == '-') {
        is_all_good = validate_long_option(flags, ++arg);
    } else if (is_all_good == true) {
        for (char *char_p = arg; *char_p; ++char_p) {
            if (*char_p == 'b') {
                flags->b_number_nonblank = true;
                flags->n_number_lines = false;
            } else if (*char_p == 'e') {
                // flags->e_Ev_equiv = true; // redundant without OS detection
                flags->v_show_nonprinting = true;
                flags->E_show_EOLs = true;
            } else if (*char_p == 'E') {
                flags->E_show_EOLs = true;
            } else if (*char_p == 'n') {
                if (flags->b_number_nonblank == false) {
                    flags->n_number_lines = true;
                }
            } else if (*char_p == 's') {
                flags->s_squeeze_blank = true;
            } else if (*char_p == 't') {
                // flags->t_Tv_equiv = true; // redundant without OS detection
                flags->T_show_tabs = true;
                flags->v_show_nonprinting = true;
            } else if (*char_p == 'T') {
                flags->T_show_tabs = true;
            } else if (*char_p == 'v') {
                flags->v_show_nonprinting = true;
            } else {
                fprintf(stderr, "s21_cat: illegal option -- %c", *char_p);
                is_all_good = false;
            }
        }
    }

    return is_all_good;
}

int parse_arguments(int argc, char *argv[], options *flags) {
    int arg_n = 1;

    for (; arg_n < argc && arg_n > 0; ++arg_n) {
        if (*argv[arg_n] == '-') {
            ++argv[arg_n];
            if (!validate_option(flags, argv[arg_n])) {
                arg_n = 0;
            }
        } else {
            break;
        }
    }

    return arg_n;
}

void symb_in_bounds() {
    ;
}

void show_nonprinting(int *cur_ch) {
    if ((0 <= *cur_ch && *cur_ch < 9) || (10 < *cur_ch && *cur_ch < 32) || (126 < *cur_ch && *cur_ch <= 160)) {
        printf("^");
        if (*cur_ch > 126) {
            *cur_ch -= 64;
        } else {
            *cur_ch += 64;
        }
    }
}

void process_files(int argc, char *argv[], int current_file, options *flags) {
    FILE *file_p = NULL;

    for (int i = 1; current_file < argc; ++i, ++current_file) {
        // if the filename is not given => read from stdin
        file_p = (current_file == argc && i == 1) ? stdin : fopen(argv[current_file], "r");

        if (file_p == NULL) {
            fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[current_file]);
        } else {
            int str_count = 1;
            int empty_str_count = 0;
            int prev_ch = '\n';

            while (!feof(file_p)) {
                int cur_ch = fgetc(file_p);

                if (cur_ch == EOF) {
                    break;
                }
                if (flags->s_squeeze_blank && cur_ch == '\n' && prev_ch == '\n') {
                    empty_str_count++;
                    if (empty_str_count > 1) {
                        continue;
                    }
                } else {
                    empty_str_count = 0;
                }
                if (prev_ch == '\n' && ((flags->b_number_nonblank && cur_ch != '\n') || flags->n_number_lines))
                    printf("%6d\t", str_count++);
                if (flags->T_show_tabs && cur_ch == '\t') {
                    printf("^");
                    cur_ch = 'I';
                }
                if (flags->E_show_EOLs && cur_ch == '\n') {
                    if (prev_ch == '\n' && flags->b_number_nonblank) {
                        printf("%*s\t$", 6, "");
                    } else {
                        printf("$");
                    }
                }
                if (flags->v_show_nonprinting) {
                    show_nonprinting(&cur_ch);
                }

                printf("%c", cur_ch);
                prev_ch = cur_ch;
            }
        }
        fclose(file_p);
    }
}

int main(int argc, char *argv[]) {
    int first_file_arg_n = 0;
    options flags = {};

    if ((first_file_arg_n = parse_arguments(argc, argv, &flags))) {
        process_files(argc, argv, first_file_arg_n, &flags);
    }

    return EXIT_SUCCESS;
}
