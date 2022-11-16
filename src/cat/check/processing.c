#include "processing.h"

static bool condition_show_nonprinting(s_info *info) {
    unsigned char ch = (unsigned char)info->cur_ch;
    return (info->flags->opt_v_show_nonprinting && info->already_printed == false) &&
           (!isgraph(ch) && ch != '\n' && ch != '\t' && ch != ' ');
}

static void show_nonprinting(s_info *info) {
    unsigned char ch = (unsigned char)info->cur_ch;
    ch < ASCII_SHIFT_BOUND ? (ch += ASCII_SHIFT) : (ch -= ASCII_SHIFT);
    printf("^%c", ch);
    info->already_printed = true;
    info->cur_ch = (char)ch;
}

static bool condition_show_EOLs(s_info *info) {
    return (info->flags->opt_E_show_EOLs && info->already_printed == false) &&
           (info->cur_ch == '\n');
}

static void show_EOLs(s_info *info) {
    if (info->prev_ch == '\n' && info->flags->opt_b_number_nonblank) {
        printf("%*.0s\t", 6, "");
    }
    putchar('$');
}

static bool condition_show_tabs(s_info *info) {
    return (info->flags->opt_T_show_tabs && info->already_printed == false) &&
           (info->cur_ch == '\t');
}

static void show_tabs(s_info *info) {
    fputs("^I", stdout);
    info->already_printed = true;
}

static void squeeze_blank(s_info *info) {
    if (info->cur_ch == '\n' && info->prev_ch == '\n') {
        info->empty_str_count++;
        if (info->empty_str_count > 1) {
            info->already_printed = true;
        }
    } else {
        info->empty_str_count = 0;
    }
}

static bool condition_number_lines(s_info *info) {
    return info->prev_ch == '\n' && info->already_printed == false &&
           ((info->flags->opt_b_number_nonblank && info->cur_ch != '\n') ||
            info->flags->opt_n_number_lines);
}

static void number_lines(s_info *info) {
    printf("%6d\t", info->str_count++);
}

void process_text(FILE *file_p, s_options *flags) {
    s_info info = {.str_count = 1, .prev_ch = '\n', .flags = flags};

    while ((info.cur_ch = getc(file_p)) != EOF) {
        info.already_printed = false;
        if (isgraph(info.cur_ch) && info.prev_ch != '\n') {
            putchar(info.cur_ch);
        } else {
            if (flags->opt_s_squeeze_blank) {
                squeeze_blank(&info);
            }
            if (condition_number_lines(&info)) {
                number_lines(&info);
            }
            if (condition_show_tabs(&info)) {
                show_tabs(&info);
            }
            if (condition_show_EOLs(&info)) {
                show_EOLs(&info);
            }
            if (condition_show_nonprinting(&info)) {
                show_nonprinting(&info);
            }
            if (info.already_printed == false) {
                putchar(info.cur_ch);
            }
        }
        info.prev_ch = info.cur_ch;
    }
}
