// Implementation of Unix "cat" utility
// Common features with "grep" must be used after
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

bool process_arguments(int argc, char *argv[]) {
    flags options = {};
    for (int i = 1; i < argc; ++i) {
        if (*argv[i] == '-') {
            if (!parse_argument(&options, ++(argv[i]))) {
                return false;
            }
        }
    }
}

bool parse_argument(flags *options, char *arg) {
    // ++arg;
    // long options
    if (*arg == '-') {
        if (strcmp(arg, "--number-nonblank") == 0) {
            options->b_number_nonblank = true;
        } else if (strcmp(arg, "--number") == 0) {
            options->n_number_lines = true;
        } else if (strcmp(arg, "--squeeze-blank") == 0) {
            options->s_squeeze_blank = true;
        } else {
            fprintf(stderr, "cat: illegal option -- %c\nusage: cat [-benstuv] [file ...]\n");
            return false;
        }
    }

    // short options
    for (char *char_p = arg; *char_p; ++char_p) {
        if (*char_p == 'b') {
            options->b_number_nonblank = true;
        } else if (*char_p == 'e') {
            options->e_vE_equiv = true;
            options->v_show_nonprinting = true;
            options->E_show_EOLs = true;
        } else if (*char_p == 'E') {
            options->E_show_EOLs = true;
        } else if (*char_p == 'n') {
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
            fprintf(stderr, "cat: illegal option -- %c", char_p);
            return false;
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
    int opt = -1;

    const int buffer_size = 4096;
    char buffer[buffer_size] = "";
    int current_file = optind;
    FILE *fp = NULL;

    while (current_file <= argc) {
        if (current_file != argc) {
            fp = fopen(argv[current_file], "rb");
            if (fp == NULL) {
                fprintf(stderr, "%s: %s: No such file or directory", argv[0], argv[current_file]);
                exit(EXIT_FAILURE);
            }
        }

        bool last_line_blank = false;
        int line_number = 1;

        while (fgets(buffer, buffer_size, (fp == NULL ? stdin : fp))) {
            int len_buf = strlen(buffer);
            buffer[len_buf - 1] = '\0';

            if (s_flag) {
                len_buf = strlen(buffer);
                int current_line_blank = (len_buf <= 1) ? 1 : 0;
                if (last_line_blank && current_line_blank) {
                    continue;
                }
                last_line_blank = current_line_blank;
            }

            if (b_flag) {
                len_buf = strlen(buffer);
                if (len_buf >= 1) {
                    char *tmp = strdup(buffer);
                    buffer[0] = '\0';
                    sprintf(buffer, "%*d\t", 6, line_number++);
                    strcat(buffer, tmp);
                }
            }

            else if (n_flag) {
                char *tmp = strdup(buffer);
                buffer[0] = '\0';
                sprintf(buffer, "%*d\t", 6, line_number++);
                strcat(buffer, tmp);
            }

            if (e_flag) {
                len_buf = strlen(buffer);
                buffer[len_buf] = '$';
                buffer[len_buf + 1] = '\0';
            }

            fprintf(stdout, "%s\n", buffer);
        }

        fclose(fp);
        ++current_file;
    }

    return EXIT_SUCCESS;
}
