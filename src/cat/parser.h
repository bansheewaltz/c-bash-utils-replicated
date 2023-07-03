#ifndef SRC_CAT_PARSER_H_
#define SRC_CAT_PARSER_H_

#include "typedefs.h"

bool parse_options(int argc, char *argv[], t_options *s_flags,
                   int *filename_was_given);

#endif  // SRC_CAT_PARSER_H_
