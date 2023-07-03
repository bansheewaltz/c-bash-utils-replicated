#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "error_outputs.h"
#include "typedefs.h"
#define ASCII_SHIFT 64
#define ASCII_BOUND 127

static bool condition_show_nonprinting(t_info *s_info) {
  uchar ch = s_info->cur_ch;
  return 1                                           //
         && s_info->s_flags->opt_v_show_nonprinting  //
         && s_info->already_printed == false         //
         && !isgraph(ch)                             //
         && !isblank(ch)                             //
         && ch != '\n';                              //
}

static void show_nonprinting(t_info *s_info) {
  uchar ch = s_info->cur_ch;

  ch < ASCII_BOUND ? (ch += ASCII_SHIFT) : (ch -= ASCII_SHIFT);
  printf("^%c", ch);
  s_info->already_printed = true;
  s_info->cur_ch = ch;
}

static bool condition_show_EOLs(t_info *s_info) {
  return 1                                    //
         && s_info->s_flags->opt_E_show_EOLs  //
         && s_info->already_printed == false  //
         && (s_info->cur_ch == '\n');         //
}

static void show_EOLs(t_info *s_info) {
  if (s_info->prev_ch == '\n' && s_info->s_flags->opt_b_number_nonblank) {
#ifndef __UBUNTU__
    printf("%*.0s\t", 6, "");
#endif
  }
  putchar('$');
}

static bool condition_show_tabs(t_info *s_info) {
  return 1                                    //
         && s_info->s_flags->opt_T_show_tabs  //
         && s_info->already_printed == false  //
         && (s_info->cur_ch == '\t');         //
}

static void show_tabs(t_info *s_info) {
  fputs("^I", stdout);
  s_info->already_printed = true;
}

static void squeeze_blank(t_info *s_info) {
  if (s_info->cur_ch == '\n' && s_info->prev_ch == '\n') {
    s_info->empty_str_count++;
    if (s_info->empty_str_count > 1) {
      s_info->already_printed = true;
    }
  } else {
    s_info->empty_str_count = 0;
  }
}

static bool condition_number_lines(t_info *s_info) {
  return 1                                               //
         && s_info->prev_ch == '\n'                      //
         && s_info->already_printed == false             //
         && (s_info->s_flags->opt_n_number_lines         //
             || (s_info->s_flags->opt_b_number_nonblank  //
                 && s_info->cur_ch != '\n'));            //
}

static void number_lines(t_info *s_info) {
  printf("%6d\t", s_info->str_count++);
}

void process_text(FILE *p_file, t_options *s_flags) {
  t_info s_info = {.str_count = 1, .prev_ch = '\n', .s_flags = s_flags};

  while ((signed char)(s_info.cur_ch = getc(p_file)) != EOF) {
    s_info.already_printed = false;
    if (isgraph(s_info.cur_ch) && s_info.prev_ch != '\n') {
      putchar(s_info.cur_ch);
    } else {
      if (s_flags->opt_s_squeeze_blank) {
        squeeze_blank(&s_info);
      }
      if (condition_number_lines(&s_info)) {
        number_lines(&s_info);
      }
      if (condition_show_tabs(&s_info)) {
        show_tabs(&s_info);
      }
      if (condition_show_EOLs(&s_info)) {
        show_EOLs(&s_info);
      }
      if (condition_show_nonprinting(&s_info)) {
        show_nonprinting(&s_info);
      }
      if (s_info.already_printed == false) {
        putchar(s_info.cur_ch);
      }
    }
    s_info.prev_ch = s_info.cur_ch;
  }
#ifdef __ALPINE__
  if (s_flags->opt_b_number_nonblank) {
    puts("");
  }
#endif
}
