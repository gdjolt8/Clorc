#include "../include/lexer.hpp"
#include "../include/utils.hpp"
#include "../include/main.hpp"
#include <iostream>
#include <vector>
#include <sstream>
std::vector<std::string> splitString(const std::string& str, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}


Lexer::Lexer(string source) {
  this->source = source;
  this->position = 0;
  this->current = (int)source.at(position);
  this->line = 1;
  this->column = 1;
  this->errors = std::vector<std::string>();
  this->current_line = splitString(source, '\n')[line-1];
}

void Lexer::nextChar() {
  this->position++;
  this->column++;
  
  if(position >= source.length()) {
    current = '\0';
  } else {
    current = (int)source.at(position);
  }
  if (current == '\n') {
    this->line += 1;
    this->column = 0;
    this->current_line = splitString(source, '\n')[line-1];
  }
}
int Lexer::peekChar() {
  if(position + 1 >= source.length()) return '\0';
  return source[position + 1];
}
void Lexer::skipWhitespace() {
  while(current == '\n' || current == ' ') {
    nextChar();
  }
}
void Lexer::skipComment() {
  if(current == '/' && peekChar() == '/') {
    while(current != '\n' && current != '\0') {
      nextChar();
    }
    skipWhitespace();
  }

  if(current == '/' && peekChar() == '*') {
    while(true) {
      if(current == '*' && peekChar() == '/') {
        break;
      }
      nextChar();
    }
    nextChar();
    nextChar();
    skipWhitespace();
  }
}
void  Lexer::addFormattedError(std::string msg) {
  string fmt_msg = "File \"" + filename + "\": Line " + std::to_string(line) + ", Column " + std::to_string(column) + ": \n\t" + current_line + "\n\n";
  fmt_msg += msg;
  errors.push_back(fmt_msg);
}
Token*  Lexer::readNextToken() {
  skipWhitespace();
  skipComment();

  Token* token = nullptr;

  switch (current) {

    case '+': {
      if(peekChar() == '=') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, PlusEq);
      } else {
        token = new Token(string(1, current), Plus);
      }
      break;
    }
    case '-': {
      if(peekChar() == '=') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, MinusEq);
      } else {
        token = new Token(string(1, current), Minus);
      }
      break;
    }
    case '*': {
      if(peekChar() == '=') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, AsteriskEq);
      } else {
        token = new Token(string(1, current), Asterisk);
      }
      break;
    }
    case '/': {
      if(peekChar() == '=') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, SlashEq);
      } else {
        token = new Token(string(1, current), Slash);
      }
      break;
    }
    case '(': {
      token = new Token(string(1, current), Lparen);
      break;
    }
    case ')': {
      token = new Token(string(1, current), Rparen);
      break;
    }
    case '{': {
      token = new Token(string(1, current), Lbrace);
      break;
    }
    case '}': {
      token = new Token(string(1, current), Rbrace);
      break;
    }
    case '[': {
      token = new Token(string(1, current), Lbrack);
      break;
    }
    case ']': {
      token = new Token(string(1, current), Rbrack);
      break;
    }
    case '^': {
      token = new Token(string(1, current), bXor);
      break;
    }
    case ':': {
      token = new Token(string(1, current), Colon);
      break;
    }
    case '.': {
      token = new Token(string(1, current), Dot);
      break;
    }
    case ',': {
      token = new Token(string(1, current), Comma);
      break;
    }
    case ';': {
      token = new Token(string(1, current), Semicolon);
      break;
    }
    case '&': {
      if(peekChar() == '=') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, bAndEq);
      } else if(peekChar() == '&') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, And);
      } else {
        token = new Token(string(1, current), bAnd);
      }
      break;
    }
    case '|': {
      if(peekChar() == '=') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, bOrEq);
      } else if(peekChar() == '|') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, Or);
      } else {
        token = new Token(string(1, current), bOr);
      }
      break;
    }
    case '>': {
      if(peekChar() == '=') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, Gte);
      }else if(peekChar() == '>') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, Shr);
      }  else {
        token = new Token(string(1, current), Gt);
      }
      break;
    }
    case '<': {
      if(peekChar() == '=') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, Lte);
      }else if(peekChar() == '<') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, Shl);
      } else {
        token = new Token(string(1, current), Lt);
      }
      break;
    }
    case '=': {
      if(peekChar() == '=') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, Eqe);
      }  else {
        token = new Token(string(1, current), Eq);
      }
      break;
    }
     case '%': {
      if(peekChar() == '=') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, ModuloEq);
      }  else {
        token = new Token(string(1, current), Modulo);
      }
      break;
    }
    case '!': {
      if(peekChar() == '=') {
        string buf;

        buf += current;
        buf += peekChar();

        nextChar();

        token = new Token(buf, Neq);
      }  else {
        token = new Token(string(1, current), Not);
      }
      break;
    }
    case '\0': {
      token = new Token("<EOF>", EOF_);
      break;
    }

    default: {
      if (isdigit(current)) {
        token = readNumberToken(); 
      }

      if(isalpha(current) || current == '_') {
        string ident = readIdentifierString();
        TokenType kind =  Token::returnKeyword(ident);

        token = new Token(ident, kind);
      }

      if(current == '"') {
        token = new Token(readStringString(), String);
      }

      if(current == '\'') {
        token = new Token(readCharacterString(), Char);
      }

      
      break;
    }
  }
    if (!token) {
      addFormattedError("unexpected character: '" + string(1,(char)current) + "'");
    }
  nextChar();
  return token;
}
string  Lexer::readIntegerString() {
  string integer;
  size_t start = position;
  while (isdigit(peekChar())) {
    nextChar();
  }

  integer = source.substr(start, position - start + 1);
  return integer;
}

Token*  Lexer::readNumberToken() {
  string intPart = readIntegerString();
  if(peekChar() != '.') {
    return new Token(intPart, Int);
  }
  nextChar();
  nextChar();
  string floatPart = readIntegerString();
  return new Token(intPart+"."+floatPart, Float);
}
string  Lexer::readStringString() {
  nextChar();
  string stringf;
  size_t start = position;
  while ((char)current != '"') {
    nextChar();
  }

  stringf = source.substr(start, position - start );
  stringf = string_formatted(stringf);
  return stringf;
}
string  Lexer::readIdentifierString() {
  string identifier;
  size_t start = position;
  while (isalnum(peekChar()) || peekChar() == '_') {
    nextChar();
  }

  identifier = source.substr(start, position - start + 1);
  return identifier;
}
string  Lexer::readCharacterString() {
  nextChar();
  string character;
  size_t start = position;
  while (current != '\'') {
    nextChar();
  }

  character = source.substr(start, position - start );
  character = string_formatted(character);

  if( character.length() == 0) {
    addFormattedError("character literal cannot be empty\n");
  }
  if( character.length() > 1) {
    addFormattedError("character literal has too many characters\n");
  }
  return character;
}