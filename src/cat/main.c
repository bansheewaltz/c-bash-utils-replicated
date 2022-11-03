#include <regex.h>
#include <stdio.h>
#include <string.h>

#include "../common/regex.h"
#include "options.h"

int main(int argc, char* argv[]) {
  regex_t regex = get_regex(CAT_OPTIONS_REGEX);
  for (int i = 1; i < argc; i++) {
    printf("%d", matches(argv[i], regex, NULL, 0));
  }
}
