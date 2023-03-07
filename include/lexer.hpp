#ifndef CLORC_LEXER_HPP
#define CLORC_LEXER_HPP

#include "token.hpp"
#include <vector>
class Lexer {
public:
  string source;
  size_t position;  
  int current;
  std::string filename;
  size_t line;
  size_t column;
  string current_line;

  std::vector<std::string> errors;

  Lexer(string source);
  void nextChar();
  int peekChar();
  void skipWhitespace();
  void skipComment();
  void addFormattedError(std::string msg);
  Token* readNextToken();
  Token* readNumberToken();
  string readIntegerString();
  string readStringString();
  string readIdentifierString();
  string readCharacterString();
};
#endif