#include "log.h"

void log_msg(const char *filename, const char *msg, ...) {
  FILE *fptr;
  fptr = fopen(filename, "a");

  va_list args;

  va_start(args, msg);
  vfprintf(fptr, msg, args);
  fprintf(fptr, "\n");
  va_end(args);

  fclose(fptr);
}

void clear_log(const char *filename) {
  FILE *fptr;
  fptr = fopen(filename, "w");
  fprintf(fptr, "");
  fclose(fptr);
}
