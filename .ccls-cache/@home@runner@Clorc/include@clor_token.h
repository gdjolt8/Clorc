#ifndef CLORC_CLORC_TOKEN_H
#define CLORC_CLORC_TOKEN_H

enum TokenType
{
  END_OF_FILE,

  Identifier,
  Integer,
  FloatLit,
  String,
  Char,

  Plus, PlusEq, PlusPlus, Minus, MinusEq, MinusMinus, Asterisk, AsteriskEq, Slash, SlashEq, Modulo, ModuloEq,
  Lparen, Rparen, Lbrace, Rbrace, Lbrack, Rbrack,
  Comma, Dot, Colon, Semicolon,
  Shl, Shr, Gt, Lt, Gte, Lte, Eq, Eqe, Neq, 
  And, Not, Or,
  BitAnd, BitAndEq, BitOr, BitOrEq, BitXor, BitXorEq,

  // keyword tokens
  KW_start,
  DT_start,
  KW_i8, KW_i16, KW_i32, KW_i64, KW_i128, KW_u8, KW_u16, KW_u32, KW_u64, KW_u128, KW_f32, KW_f64, KW_f128, KW_char, KW_string, KW_boolean,
  DT_end,
  KW_func, KW_if, KW_for, KW_while, KW_else, KW_true, KW_false, KW_return, KW_break, KW_continue, KW_class, KW_new, KW_null,
  KW_end,
};

typedef struct TOKEN_STRUCT {
  char *value;
  int type;
} ClorToken_T;

ClorToken_T* init_clor_token(char* value, int type);
const char * token_type_to_string(int type);
enum TokenType return_keyword (char *value);
#endif