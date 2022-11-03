#ifndef MY_REGEX_H
#define MY_REGEX_H

#include <regex.h>

regex_t get_regex(const char* regex_str);
int matches(char* arg, regex_t regex, regmatch_t matches[], size_t size);

#endif  //
