#include "../include/file_sys.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file_contents (const char *path) {
  FILE *fp = fopen(path, "rb");

  if (!fp) {
    fprintf(stderr, "'%s' is not a valid file or directory.", path);
    exit(1);
  }
  fseek(fp, 0, SEEK_END);
  long length = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *buffer = (char*)calloc(length, length);

  fread(buffer, length, 1, fp);
  return buffer;
}
void write_file_contents (const char *path, char *contents) {
  FILE* fp = fopen(path, "wb");
  fprintf(fp, "%s", contents);
}