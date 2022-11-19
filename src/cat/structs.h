#include <stdbool.h>

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

typedef struct {
  s_options *flags;
  char cur_ch;
  char prev_ch;
  int str_count;
  int empty_str_count;
  bool already_printed;
} s_info;
