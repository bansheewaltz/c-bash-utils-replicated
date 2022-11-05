// Implementation of Unix "cat" utility
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    bool b_number_nonblank;
    bool e_vE_equiv;
    bool E_show_EOLs;
    bool n_number_lines;
    bool s_squeeze_blank;
    bool t_Tv_equiv;
    bool T_show_tabs;
    bool v_show_nonprinting;
} flags;

bool parse_option(flags *options, char *arg) {
    // long options
    if (*arg == '-') {
        arg++;
        if (strcmp(arg, "number-nonblank") == 0) {
            options->b_number_nonblank = true;
        } else if (strcmp(arg, "number") == 0) {
            options->n_number_lines = true;
        } else if (strcmp(arg, "squeeze-blank") == 0) {
            options->s_squeeze_blank = true;
        } else {
            fprintf(stderr, "s21_cat: illegal option -- %s\n", arg);
            return false;
        }
        return true;
    }

    // short options
    for (char *char_p = arg; *char_p; ++char_p) {
        if (*char_p == 'b') {
            options->b_number_nonblank = true;
            options->n_number_lines = false;
        } else if (*char_p == 'e') {
            options->e_vE_equiv = true;
            options->v_show_nonprinting = true;
            options->E_show_EOLs = true;
        } else if (*char_p == 'E') {
            options->E_show_EOLs = true;
        } else if (*char_p == 'n') {
            if (options->b_number_nonblank == true) {
                continue;
            }
            options->n_number_lines = true;
        } else if (*char_p == 's') {
            options->s_squeeze_blank = true;
        } else if (*char_p == 't') {
            options->t_Tv_equiv = true;
            options->T_show_tabs = true;
            options->v_show_nonprinting = true;
        } else if (*char_p == 'T') {
            options->T_show_tabs = true;
        } else if (*char_p == 'v') {
            options->v_show_nonprinting = true;
        } else {
            fprintf(stderr, "s21_cat: illegal option -- %c", *char_p);
            return false;
        }
    }

    return true;
}

int process_arguments(int argc, char *argv[], flags *options) {
    int i = 1;
    for (; i < argc; ++i) {
        if (*argv[i] == '-') {
            // sends argument string with an option itself without '-' delimiter
            if (!parse_option(options, ++(argv[i]))) {
                return 0;
            }
        } else {
            break;
        }
    }
    // returns number of given options + 1 for further filetype argument adressing
    return i;
}

void process_files(int argc, char *argv[], int current_file, flags *options) {
    FILE *file_p = NULL;

    for (int i = 1; current_file < argc; ++i, ++current_file) {
        // if the filename is not given => read from stdin
        file_p = (current_file == argc && i == 1) ? stdin : fopen(argv[current_file], "r");

        if (file_p == NULL) {
            fprintf(stderr, "s21_cat: %s: No such file or directory", argv[current_file]);
        } else {
            int str_count = 1;
            int empty_str_count = 0;
            int last_ch = '\n';

            while (!feof(file_p)) {
                int cur_ch = fgetc(file_p);

                if (cur_ch == EOF) {
                    break;
                }
                if (options->s_squeeze_blank && cur_ch == '\n' && last_ch == '\n') {
                    empty_str_count++;
                    if (empty_str_count > 1) {
                        continue;
                    }
                } else {
                    empty_str_count = 0;
                }
                if (last_ch == '\n' && ((options->b_number_nonblank && cur_ch != '\n') || options->n_number_lines))
                    printf("%6d\t", str_count++);
                if (options->t_Tv_equiv && cur_ch == '\t') {
                    printf("^");
                    cur_ch = 'I';
                }
                if (options->e_vE_equiv && cur_ch == '\n') {
                    if (last_ch == '\n' && options->b_number_nonblank) {
                        printf("%*s\t$", 6, "");
                    } else {
                        printf("$");
                    }
                }
                if (options->v_show_nonprinting) {
                    if ((cur_ch >= 0 && cur_ch < 9) || (cur_ch > 10 && cur_ch < 32) ||
                        (cur_ch > 126 && cur_ch <= 160)) {
                        printf("^");
                        if (cur_ch > 126) {
                            cur_ch -= 64;
                        } else {
                            cur_ch += 64;
                        }
                    }
                }

                printf("%c", cur_ch);
                last_ch = cur_ch;
            }
        }
        fclose(file_p);
    }
}

int main(int argc, char *argv[]) {
    int first_file_arg_n = 0;
    flags options = {};

    if ((first_file_arg_n = process_arguments(argc, argv, &options))) {
        process_files(argc, argv, first_file_arg_n, &options);
    }

    return EXIT_SUCCESS;
}
