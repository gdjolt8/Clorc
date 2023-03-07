#include "../include/token.hpp"

std::string keywords[] = {
  "i8", "i16", "i32", "i64", "i128", "f32", "f64", "f128", "u8", "u16", "u32", "u64", "u128", "char", "bool", "string", "func", "true", "false", "if", "else", "for", "while", "const", "new", "class", "null", "return", "break", "continue", "this"
};

enum TokenType keywordi[] = {
  KW_I8, KW_I16, KW_I32, KW_I64, KW_I128, KW_F32, KW_F64, KW_F128, KW_U8, KW_U16, KW_U32, KW_U64, KW_U128, KW_Char, KW_Boolean, KW_String, KW_Func,

  KW_True, KW_False, KW_If, KW_Else, KW_For, KW_While, KW_Const, KW_New,

  KW_Class, KW_Null, KW_Return, KW_Break, KW_Continue, KW_This
};

Token::Token(string text, TokenType kind) {
  this->text = text;
  this->kind = kind;
}
TokenType Token::returnKeyword(string text) {
  for(int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
    if(keywords[i] ==  text) {
      return keywordi[i];
    }
  }
  return Ident;
}
string Token::as_string() {
  //return "Token("+text+", "+Token::ttString(kind)+")";
  return "Token<text=\""+this->text+"\", " + "kind="+Token::ttString(this->kind)+">";
}

string Token::ttString(TokenType kind) {
  switch(kind) {
    case Ident: return "identifier";
    case Int: return "int";
    case Float: return "float";
    case String: return "string";
    case Char: return "char";
    case Plus: return "+";
case PlusEq: return "+=";
    case Minus: return "-";
case MinusEq: return "-=";
    case Asterisk: return "*";
case AsteriskEq: return "*=";
    case Slash: return "/";
case SlashEq: return "/=";
    case Lparen: return "(";
    case Rparen: return ")";
case Lbrace: return "{";
    case Rbrace: return "}";
case Lbrack: return "[";
    case Rbrack: return "]";
    case Semicolon: return ";";
    case Colon: return ":";
    case Dot: return ".";
    case Comma: return ",";
    case Shl: return ">>";
    case Shr: return "<<";
    case Eq: return "=";
    case Eqe: return "==";
    case Gt: return ">";
    case Lt: return "<";
    case Lte: return "<=";
    case Gte: return ">=";
    case bXor: return "^";
    case bOr: return "|";
    case KW_If: return "if";
    case KW_Else: return "else";
    case KW_For: return "for";
    case KW_While: return "while";
    case KW_True: return "true";
    case KW_False: return "false";
    case KW_Boolean: return "bool";
    case KW_I8: return "i8";
    case KW_I16: return "i16";
    case KW_I32: return "i32";
    case KW_I64: return "i64";
    case KW_I128: return "i128";
    case KW_U8: return "u8";
    case KW_U16: return "u16";
    case KW_U32: return "u32";
    case KW_U64: return "u64";
    case KW_U128: return "u128";
    case KW_Char: return "char";
    case KW_String: return "string";
    case KW_Const: return "const";
    case KW_New: return "new";
    case KW_Null: return "null";
    case EOF_: return "<EOF>";
  }
  

  return std::to_string(kind);
}