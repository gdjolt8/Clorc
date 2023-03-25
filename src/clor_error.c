 #include "../include/clor_error.h"
#include <stdlib.h>
#include <string.h> 

#define nlen 20
ClorError_T* init_clor_error(const char *dir, char *msg, unsigned long line, unsigned long column) {
  ClorError_T* error = (ClorError_T*)calloc(1, sizeof(ClorError_T));
  error->directory = dir;
  error->msg = msg;
  error->line = line;
  error->column = column;
  return error;
}

char *read_clor_error(ClorError_T* error) {
  char *estimated_buffer = (char*)calloc(nlen * 2 + 50 + 1000, sizeof(char));
  sprintf(estimated_buffer, "%s:%lu:%lu: %s", error->directory, error->line, error->column, error->msg);
  free(error);
  return estimated_buffer;
}