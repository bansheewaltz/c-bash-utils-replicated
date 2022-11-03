#include "main.h"

int main(int argc, char* options[]) {
  regex_t regex = get_regex(CAT_OPTIONS_REGEX);
  for (int i = 1; i < argc; i++) {
    if (matches(options[i], regex, NULL, 0)) {
      char* buffer = malloc(strlen(options[i]));
      rmchr(options[i], buffer, '-');
      char* ptr = strstr(OPTIONS_STR, buffer);
      if (ptr != NULL) {
        printf("Match: %d (index in options_str)", ptr - OPTIONS_STR);
      }
    }
  }
}
