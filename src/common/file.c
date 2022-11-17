#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readfile(char *filename, bool *error) {
  char *buffer = 0;
  long length;
  FILE *f = fopen(filename, "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer) {
      fread(buffer, 1, length, f);
    }
    fclose(f);
  } else {
    *error = true;
  }
  return buffer;
}