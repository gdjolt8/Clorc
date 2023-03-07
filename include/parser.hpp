#ifndef CLORC_PARSER_HPP
#define CLORC_PARSER_HPP
#include "lexer.hpp"
#include "AST.hpp"
#include "scope.hpp"
class Parser {
public:
  Lexer *l;
  Token *curToken;
  Token *peekToken;
  std::string filename;
  std::vector<std::string> errors;
  std::vector<std::string> warnings;
  Scope* scope;

  Parser(Lexer *l);
  void nextToken();
  void addError(std::string msg);
  void addWarning(std::string msg);
  void match(TokenType kind);
  bool curTokenIs(TokenType kind);
  bool peekTokenIs(TokenType kind);
  bool curTokenDatatype();
  ASTExpr* parseInteger(DataType *dt);
  ASTExpr* parseFloat(DataType *dt);
  ASTExpr* parseChar();
  ASTExpr* parseString();
  ASTExpr* parseArray();
  ASTExpr* parseBoolean();
  ASTExpr* parseIdentifier();
  ASTExpr* parseIfStatement();
  ASTExpr* parseForStatement();
  ASTExpr* parseWhileStatement();
  ASTExpr* parseClassStatement();
  ASTExpr* parseBlockStatement();
  ASTExpr* parseVariable();
  ASTExpr* parseVariableDefinition();
  ASTExpr* parseSource();
  ASTExpr* parseReturn();
  ASTExpr* parseNewStatement();
  DataType* parseDatatype();
  ASTExpr* parseNull();
  ASTExpr* parseFunctionCall();
  ASTExpr* parseFunctionDefinition();
  ASTExpr* parseStatement();
  ASTExpr* parseFactor();
  ASTExpr* parseTerm();
  ASTExpr* parseExpression();
  ASTExpr* parseExpr();
  ASTExpr* parseBreak(); 
  ASTExpr* parseContinue();
  ASTExpr* parseThis();
  std::vector<ASTExpr*> parseArgumentList();
};
#endif