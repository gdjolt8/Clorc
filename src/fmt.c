#include "../include/utils/fmt.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
char *format_literal(char* str) {
  char *result = (char*)calloc(1, sizeof(char));
  result[0] = '\0';
  for (int i = 0; i < strlen(str); i++) {
    char c;
    switch (str[i]) {
      case '\\': {
        char escape = str[i+1];
        if (escape == 'n') c = '\n';
        if (escape == 't') c = '\t';
        i++;
        break;
      }
      default: {
        c = str[i];
        break;
      }
    }
    result = (char*)realloc(result, strlen(result) + 2);
    strcat(result, (char[]){c, '\0'});
  }
  return result;
}

int is_digit_char (char c) {
  return isdigit(c);
}
int is_alpha_char (char c) {
  return isalpha(c);
}
int is_alnum_char (char c) {
  return isalnum(c);
}
int is_whitespace_char (char c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\b' || c == '\r';
}

char *char_to_string_lit(char c) {
  char *buffer = (char*)calloc(2, sizeof(char));
  *buffer = c;
  *(buffer + 1) = '\0';
  return buffer;
}