#ifndef SRC_GREP_UTILS_H_
#define SRC_GREP_UTILS_H_

void print_error(char message[]);
void t_setprogname(char const invocation_name[]);
void add_delim_unsafe(char *re_str, int *re_len, char delimiter[]);

#endif  // SRC_GREP_UTILS_H_
