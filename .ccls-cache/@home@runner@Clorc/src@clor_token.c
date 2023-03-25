#include "../include/clor_token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *keywords[] = {
  "i8", "i16", "i32", "i64", "i128", "u8", "u16", "u32", "u64", "u128", "f32", "f64", "f128", "char", "string", "boolean", "func", "if", "for", "while", "else", "true", "false", "return", "break", "continue", "class", "new" 
};

enum TokenType keywordi[] = {
  KW_i8, KW_i16, KW_i32, KW_i64, KW_i128, KW_u8, KW_u16, KW_u32, KW_u64, KW_u128, KW_f32, KW_f64, KW_f128, KW_char, KW_string, KW_boolean, KW_func, KW_if, KW_for, KW_while, KW_else, KW_true, KW_false, KW_return, KW_break, KW_continue, KW_class, KW_new
};
ClorToken_T* init_clor_token(char* value, int type) {
  ClorToken_T* clor_token = (ClorToken_T*)calloc(1, sizeof(ClorToken_T));
  clor_token->value = value;
  clor_token->type = type;
  return clor_token;
}
const char* token_type_to_string(int type) {
  switch (type) {
    case Identifier: return "identifier";
    case Integer: return "integer";
    case FloatLit: return "float";
    case String: return "string";
    case Char: return "char";
    case Plus: return "+"; case PlusEq: return "+="; 
    case PlusPlus: return "++"; case MinusMinus: return "--";
    case Minus: return "-"; case MinusEq: return "-=";
    case Asterisk: return "*"; case AsteriskEq: return "*=";
    case Slash: return "/"; case SlashEq: return "/=";
    case Lparen: return "("; case Rparen: return ")";
    case Lbrace: return "{"; case Rbrace: return "}";
    case Lbrack: return "["; case Rbrack: return "]";
    case Colon: return ":"; case Semicolon: return ";";
    case Comma: return ","; case Dot: return ".";
    case Shl: return "<<"; case Shr: return ">>";
    case Gt: return ">"; case Lt: return "<";
    case Gte: return ">="; case Lte: return "<=";
    case Eq: return "="; case Eqe: return "==";
    case Neq: return "!="; case And: return "&&";
    case Not: return "!"; case Or: return "||";
    case BitOr: return "|"; case BitAnd: return "&";
    case BitXor: return "^"; case BitOrEq: return "|=";
    case BitAndEq: return "&="; case BitXorEq: return "^=";
    case END_OF_FILE: return "<EOF>";
    default: {
      if (type > DT_start && type < DT_end) {
        return keywords[type - (DT_start+1)]; 
      }
    }
  }
  printf("None type: '%d'\n", type);
  return "none";
}

enum TokenType return_keyword (char *value) {
  for(int i = 0; i < sizeof(keywordi)/sizeof(keywordi[0]); i++) {
    if (strcmp(value, keywords[i]) == 0) {
      return keywordi[i];
    }
  }
  return Identifier;
}