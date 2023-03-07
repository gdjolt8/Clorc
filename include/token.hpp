#ifndef CLORC_TOKEN_HPP
#define CLORC_TOKEN_HPP

#include <string>
using std::string;
enum TokenType {
  EOF_,

  Ident,
  Int,
  Float,
  String,
  Char,

  Plus, PlusEq,
  Minus, MinusEq,
  Asterisk, AsteriskEq,
  Slash, SlashEq,
  Modulo, ModuloEq,

  Lparen, Rparen, Lbrace, Rbrace, Lbrack, Rbrack,

  Comma, Semicolon, Colon, Dot,

  Gt, Gte, Lt, Lte, Eq, Eqe, Neq,

  And, Not, Or, bAnd, bAndEq, bXor, bXorEq, bOr, bOrEq,
  Shl, Shr,

  kw_start, 

  dt_start,
  KW_I8, KW_I16, KW_I32, KW_I64, KW_I128, KW_F32, KW_F64, KW_F128, KW_U8, KW_U16, KW_U32, KW_U64, KW_U128, KW_Char, KW_Boolean, KW_String, dt_end, KW_Func,

  KW_True, KW_False, KW_If, KW_Else, KW_For, KW_While, KW_Const, KW_New,
  KW_Return,
  KW_Class, KW_Null, KW_Break, KW_Continue, KW_This, kw_end
};

class Token {
public:
  string text;
  TokenType kind;

  Token(string text, TokenType kind);
  static TokenType returnKeyword(string text);
  static string ttString(TokenType kind);
  string as_string();
};
#endif