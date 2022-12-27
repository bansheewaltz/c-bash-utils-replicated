#ifndef SRC_CAT_TYPEDEFS_H_
#define SRC_CAT_TYPEDEFS_H_

#include <stdbool.h>

typedef unsigned char uchar;

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
} t_options;

typedef struct {
  t_options *s_flags;
  uchar cur_ch;
  uchar prev_ch;
  int str_count;
  int empty_str_count;
  bool already_printed;
} t_info;

#endif  // SRC_CAT_TYPEDEFS_H_#endif
