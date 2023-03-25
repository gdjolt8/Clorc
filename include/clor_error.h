#ifndef CLORC_CLORC_ERROR_H
#define CLORC_CLORC_ERROR_H

typedef struct 
{
  const char* directory;
  char *msg;
  unsigned long line;
  unsigned long column;
} ClorError_T;

ClorError_T* init_clor_error(const char *dir, char *msg, unsigned long line, unsigned long column);

char *read_clor_error(ClorError_T* error);
#endif