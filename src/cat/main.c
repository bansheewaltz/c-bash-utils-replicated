/* Realization of Unix[-like] "cat" utility based on macOS-specific version
** with some GNU-specific features added.
** Character by character processing approach */
#include <stdio.h>
#include <stdlib.h>

#include "error_outputs.h"
#include "globals.h"
#include "parser.h"
#include "processor.h"

#define FALSE_ALL_BOOLS {false};

char const *g_PROGRAM_NAME;

int main(int argc, char *argv[]) {
  t_options s_flags = FALSE_ALL_BOOLS;
  int filename_was_given = false;  // will hold the name of the first file
  g_PROGRAM_NAME = argv[0] + 2;    // to omit "./"

  if (parse_options(argc, argv, &s_flags, &filename_was_given)) {
    FILE *p_file = NULL;

    if (filename_was_given == false) {
      p_file = stdin;
      fputs("Interactive mode is on...\n\n", stderr);
      process_text(p_file, &s_flags);
    }

    for (int file_ndx = filename_was_given; file_ndx != 0 && file_ndx < argc;
         ++file_ndx) {
      p_file = fopen(argv[file_ndx], "r");
      if (p_file != NULL) {
        process_text(p_file, &s_flags);
        fclose(p_file);
      } else {
        print_error_no_file(argv[file_ndx]);
      }
    }
  }

  return EXIT_SUCCESS;
}
