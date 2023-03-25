#include "../include/clor_lexer.h"
#include "../include/utils/fmt.h"
#include <stdlib.h>
#include <string.h>

ClorLexer_T* init_clor_lexer(char *source_code) {
  ClorLexer_T* lexer = (ClorLexer_T*)calloc(1, sizeof(ClorLexer_T));
  lexer->source_code = source_code;
  lexer->current_position = 0;
  lexer->current_char = source_code[lexer->current_position];
  lexer->line_offset = 1;
  lexer->char_offset = 0;
  lexer->error_list = init_list();
  return lexer;
}

void clor_lexer_add_error_msg(ClorLexer_T* lexer, char* err) {
  ClorError_T* errmsg = init_clor_error(lexer->directory, err, lexer->line_offset, lexer->char_offset);
  add_element_list(lexer->error_list, errmsg);
}

void clor_lexer_read_char(ClorLexer_T* lexer) {
  lexer->current_position++;
  lexer->char_offset += 1;
  
  if (lexer->current_position >= strlen(lexer->source_code)) {
    lexer->current_char = '\0';
  } else {
    lexer->current_char = lexer->source_code[lexer->current_position];
    if(lexer->current_char == '\n') {
      lexer->line_offset++;
      lexer->char_offset = 0;
    }
  }
  
}
unsigned char clor_lexer_peek_char(ClorLexer_T* lexer) {
  if (lexer->current_position + 1 >= strlen(lexer->source_code)) return '\0';
  return lexer->source_code[lexer->current_position + 1];
}
void clor_lexer_skip_whitespace(ClorLexer_T* lexer) {
  while (is_whitespace_char(lexer->current_char)) {
    clor_lexer_read_char(lexer);
  }
}
void clor_lexer_skip_comment(ClorLexer_T* lexer) {
  if (lexer->current_char == '/') {
    if (clor_lexer_peek_char(lexer) == '/') {
      while (lexer->current_char != '\n' && lexer->current_char >= '\0') {
        clor_lexer_read_char(lexer);
      }
      goto exit;
    }
    
    if (clor_lexer_peek_char(lexer) == '*') {
      while (1) {
        clor_lexer_read_char(lexer);

        if (lexer->current_char == '*' && clor_lexer_peek_char(lexer) == '/') {
          clor_lexer_read_char(lexer);
          clor_lexer_read_char(lexer);
          clor_lexer_skip_whitespace(lexer);
          goto exit;
        }
      }
      
    }
  }
  exit: return;
}
ClorToken_T* clor_lexer_read_next_token(ClorLexer_T* lexer) {
  clor_lexer_skip_whitespace(lexer);
  clor_lexer_skip_comment(lexer);
  
  ClorToken_T* token = NULL;

  switch (lexer->current_char) {
    case '+': {
      if (clor_lexer_peek_char(lexer) == '=') {
        char *buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        clor_lexer_read_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_clor_token(buffer, PlusEq);
      } else if (clor_lexer_peek_char(lexer) == '+') {
        char *buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        clor_lexer_read_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_clor_token(buffer, PlusPlus);
      } else {
        token = init_clor_token(char_to_string_lit(lexer->current_char), Plus);
      }
      break;
    }
    case '-': {
      if (clor_lexer_peek_char(lexer) == '=') {
        char *buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        clor_lexer_read_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_clor_token(buffer, MinusEq);
      } else if (clor_lexer_peek_char(lexer) == '-') {
        char *buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        clor_lexer_read_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_clor_token(buffer, MinusMinus);
      } else {
        token = init_clor_token(char_to_string_lit(lexer->current_char), Minus);
      }
      break;
    }
    case '*': {
      if (clor_lexer_peek_char(lexer) == '=') {
        char *buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        clor_lexer_read_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_clor_token(buffer, AsteriskEq);
      } else {
        token = init_clor_token(char_to_string_lit(lexer->current_char), Asterisk);
      }
      break;
    }
    case '/': {
      if (clor_lexer_peek_char(lexer) == '=') {
        char *buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        clor_lexer_read_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_clor_token(buffer, SlashEq);
      } else {
        token = init_clor_token(char_to_string_lit(lexer->current_char), Slash);
      }
      break;
    }
    case '(': {
      token = init_clor_token(char_to_string_lit(lexer->current_char), Lparen);
      break;
    }
    case ')': {
      token = init_clor_token(char_to_string_lit(lexer->current_char), Rparen);
      break;
    }
    case '{': {
      token = init_clor_token(char_to_string_lit(lexer->current_char), Lbrace);
      break;
    }
    case '}': {
      token = init_clor_token(char_to_string_lit(lexer->current_char), Rbrace);
      break;
    }
    case '[': {
      token = init_clor_token(char_to_string_lit(lexer->current_char), Lbrack);
      break;
    }
    case ']': {
      token = init_clor_token(char_to_string_lit(lexer->current_char), Rbrack);
      break;
    }
    case '.': {
      token = init_clor_token(char_to_string_lit(lexer->current_char), Dot);
      break;
    }
    case ',': {
      token = init_clor_token(char_to_string_lit(lexer->current_char), Comma);
      break;
    }
    case ':': {
      token = init_clor_token(char_to_string_lit(lexer->current_char), Colon);
      break;
    }
    case ';': {
      token = init_clor_token(char_to_string_lit(lexer->current_char), Semicolon);
      break;
    }
    case '>': {
      if (clor_lexer_peek_char(lexer) == '=') {
        char *buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        clor_lexer_read_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_clor_token(buffer, Gte);
      } else {
        token = init_clor_token(char_to_string_lit(lexer->current_char), Gt);
      }
      break;
    }
    case '<': {
      if (clor_lexer_peek_char(lexer) == '=') {
        char *buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        clor_lexer_read_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_clor_token(buffer, Lte);
      } else {
        token = init_clor_token(char_to_string_lit(lexer->current_char), Lt);
      }
      break;
    }
    case '!': {
      if (clor_lexer_peek_char(lexer) == '=') {
        char *buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        clor_lexer_read_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_clor_token(buffer, Neq);
      } else {
        token = init_clor_token(char_to_string_lit(lexer->current_char), Not);
      }
      break;
    }
    case '=': {
      if (clor_lexer_peek_char(lexer) == '=') {
        char *buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        clor_lexer_read_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_clor_token(buffer, Eqe);
      } else {
        token = init_clor_token(char_to_string_lit(lexer->current_char), Eq);
      }
      break;
    }
    case '\0': {
      token = init_clor_token("<EOF>", END_OF_FILE);
      break;
    }
    default: {
      if (is_digit_char(lexer->current_char)) {
        token = clor_lexer_read_number_token(lexer);
      } else if (is_alpha_char(lexer->current_char) || lexer->current_char == '_') {
        char *value = clor_lexer_read_identifier_string(lexer);
        enum TokenType type = return_keyword(value);
        token = init_clor_token(value, type);
      } else if (lexer->current_char == '\"') {
        char *value = clor_lexer_read_string_string(lexer);
        token = init_clor_token(value, String);
      } else if (lexer->current_char == '\'') {
        char *value = clor_lexer_read_character_string(lexer);
        token = init_clor_token(value, Char);
      }

      if (!token) {
        char* buffer = calloc(1000, sizeof(char));
        sprintf(buffer, "unexpected character: %c\n", lexer->current_char);
        clor_lexer_add_error_msg(lexer, buffer);
      }
    }
  }
  clor_lexer_read_char(lexer);
  return token;
}
char *clor_lexer_read_integer_string(ClorLexer_T* lexer) {
  char *value = calloc(1, sizeof(char));
  value[0] = '\0';

  while (is_digit_char(clor_lexer_peek_char(lexer))) {
    value = realloc(value, strlen(value) + 2);
    strcat(value, char_to_string_lit(lexer->current_char));
    clor_lexer_read_char(lexer);
  }

  strcat(value, char_to_string_lit(lexer->current_char));
  return value;
}
char *clor_lexer_read_identifier_string(ClorLexer_T* lexer)  {
  char *value = calloc(1, sizeof(char));
  value[0] = '\0';

  while (is_alnum_char(clor_lexer_peek_char(lexer)) || clor_lexer_peek_char(lexer) == '_') {
    value = realloc(value, strlen(value) + 2);
    strcat(value, char_to_string_lit(lexer->current_char));
    clor_lexer_read_char(lexer);
  }

  strcat(value, char_to_string_lit(lexer->current_char));
  return value;
}
char *clor_lexer_read_string_string(ClorLexer_T* lexer) {
  clor_lexer_read_char(lexer);
  char *value = calloc(2, sizeof(char));
  value[1] = '\0';

  while (lexer->current_char != '\"') {
    if(lexer->current_char == '\0') {
      clor_lexer_add_error_msg(lexer, "unmatched string literal");
      break;
    }
    value = realloc(value, strlen(value) + 2);
    strcat(value, char_to_string_lit(lexer->current_char));
    clor_lexer_read_char(lexer);
  }
  value = format_literal(value);
  return value;
}
char *clor_lexer_read_character_string(ClorLexer_T* lexer) {
  clor_lexer_read_char(lexer);
  char *value = calloc(1, sizeof(char));
  value[0] = '\0';

  while (lexer->current_char != '\'') {
    if(lexer->current_char == '\0') {
      clor_lexer_add_error_msg(lexer, "unmatched char literal");
      break;
    }
    value = realloc(value, strlen(value) + 2);
    strcat(value, char_to_string_lit(lexer->current_char));
    clor_lexer_read_char(lexer);
  }

  
  value = format_literal(value);

  if(strlen(value) == 0) {
    clor_lexer_add_error_msg(lexer, "Character literal can't be empty.");
  }

  if(strlen(value) > 1) {
    clor_lexer_add_error_msg(lexer, "Character literal has too many characters. Did you mean to write a string?");
  }
  return value;
}
ClorToken_T *clor_lexer_read_number_token(ClorLexer_T* lexer) {
  char *intPart = clor_lexer_read_integer_string(lexer);
  if (clor_lexer_peek_char(lexer) != '.') {
    return init_clor_token(intPart, Integer);
  }
  clor_lexer_read_char(lexer);
  clor_lexer_read_char(lexer);
  char *floatPart = clor_lexer_read_integer_string(lexer);

  char *number = calloc(strlen(intPart)+strlen(floatPart)+3, sizeof(char));
  sprintf(number, "%s.%s", intPart, floatPart);
  return init_clor_token(number, FloatLit);
}