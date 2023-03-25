#ifndef CLORC_CLORC_LEXER_H
#define CLORC_CLORC_LEXER_H

#include "clor_token.h"
#include "utils/list.h"
#include "clor_error.h"
typedef struct CLOR_LEXER_STRUCT
{
  const char *directory;
  char *source_code;
  unsigned char current_char;
  unsigned long current_position;

  unsigned long line_offset;
  unsigned long char_offset;
  list_T* error_list;
} ClorLexer_T;

ClorLexer_T* init_clor_lexer(char *source_code);
void clor_lexer_add_error_msg(ClorLexer_T* lexer, char* err);
void clor_lexer_read_char(ClorLexer_T* lexer);
unsigned char clor_lexer_peek_char(ClorLexer_T* lexer);
void clor_lexer_skip_whitespace(ClorLexer_T* lexer);
void clor_lexer_skip_comment(ClorLexer_T* lexer);
ClorToken_T* clor_lexer_read_next_token(ClorLexer_T* lexer);
char *clor_lexer_read_integer_string(ClorLexer_T* lexer);
char *clor_lexer_read_identifier_string(ClorLexer_T* lexer);
char *clor_lexer_read_string_string(ClorLexer_T* lexer);
char *clor_lexer_read_character_string(ClorLexer_T* lexer);
ClorToken_T *clor_lexer_read_number_token(ClorLexer_T* lexer);
#endif