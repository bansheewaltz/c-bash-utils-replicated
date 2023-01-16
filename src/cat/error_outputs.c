#include <stdio.h>

#include "globals.h"

void print_error_illegal_option(int invalid_opt_len, char *invalid_opt) {
  fprintf(stderr,                                       //
          "%s: illegal option -- %.*s\n",               //
          PROGRAM_NAME, invalid_opt_len, invalid_opt);  //
  fprintf(stderr,                                       //
          "usage: ./%s [-beEnstTv] [file ...]\n"        //
          "long options: \n"                            //
          "\t--number-nonblank\n"                       //
          "\t--number\n"                                //
          "\t--squeeze-blank\n",                        //
          PROGRAM_NAME);                                //
}

void print_error_no_file(char const file_name[]) {
  fprintf(stderr,                                 //
          "%s: %s: No such file or directory\n",  //
          PROGRAM_NAME, file_name);               //
}
