/* Realization of Unix[-like] "cat" utility based on macOS-specific version
** with some GNU-specific features added.
** Character by character processing approach */
#include "main.h"

int main(int argc, char *argv[]) {
  s_options flags = FALSE_ALL_BOOLS;
  int filename_was_given = false;  // argv index of the first file

  if (parse_options(argc, argv, &flags, &filename_was_given)) {
    FILE *file_p = NULL;

    if (!filename_was_given) {
      file_p = stdin;
      fputs("Interactive mode is on...\n\n", stderr);
      process_text(file_p, &flags);
    }

    for (int file_ndx = filename_was_given; file_ndx && file_ndx < argc;
         ++file_ndx) {
      file_p = fopen(argv[file_ndx], "r");
      if (file_p != NULL) {
        process_text(file_p, &flags);
      } else {
        print_error_no_file(argv, file_ndx);
      }
      fclose(file_p);
    }
  }

  return EXIT_SUCCESS;
}
