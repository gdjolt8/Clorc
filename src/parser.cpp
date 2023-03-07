#include "../include/parser.hpp"
#include "../include/main.hpp"
#include <iostream>
#define default_int_type DT_I32
#define default_float_type DT_F64
using namespace std;
Parser::Parser(Lexer *l) {
  this->l = l;
  this->curToken = nullptr;
  this->peekToken = nullptr;
  this->errors = std::vector<std::string>();
  this->scope = new Scope();
  nextToken();
  nextToken();
}
void Parser::nextToken() {
  curToken = peekToken;
  peekToken = l->readNextToken();
  if(l->errors.size() > 0)
    ClorAssert_True(false, l->errors[0]);
}
void Parser::addError(std::string msg) { l->addFormattedError(msg); }

void Parser::addWarning(std::string msg) {
  warnings.push_back("Warning: " + msg);
}
void Parser::match(TokenType kind) {
  if (kind != curToken->kind) {
    addError("Expected '" + Token::ttString(kind) + "', but got '" +
                     Token::ttString(curToken->kind) + "'.");

  }
  nextToken();
}

bool Parser::curTokenIs(TokenType kind) { return kind == curToken->kind; }
bool Parser::peekTokenIs(TokenType kind) { return kind == peekToken->kind; }
bool Parser::curTokenDatatype() {
  return (curToken->kind > dt_start && curToken->kind < dt_end);
}
ASTExpr *Parser::parseInteger(DataType *dt) {
  ASTExpr *integer = new ASTExpr(AST_INT);
  integer->data_type = dt;
  integer->i128_value = strtoll(curToken->text.c_str(), NULL, 10);
  integer->scope = scope;
  return integer;
}
ASTExpr *Parser::parseFloat(DataType *dt) {
  ASTExpr *float_ = new ASTExpr(AST_FLOAT);
  float_->data_type = dt;
  float_->f128_value = strtold(curToken->text.c_str(), NULL);
  float_->scope = scope;
  return float_;
}

ASTExpr *Parser::parseChar() {
  ASTExpr *character = new ASTExpr(AST_CHAR);
  character->data_type = new DataType(DT_CHAR);
  character->char_value = curToken->text[0];
  character->scope = scope;
  return character;
}

ASTExpr *Parser::parseString() {
  ASTExpr *str = new ASTExpr(AST_STRING);
  str->data_type = new DataType(DT_STRING);
  str->string_value = curToken->text;
  str->scope = scope;
  return str;
}
ASTExpr *Parser::parseBoolean() {
  ASTExpr *boolean = new ASTExpr(AST_BOOLEAN);
  boolean->data_type = new DataType(DT_BOOL);
  boolean->bool_value = (curTokenIs(KW_True));
  boolean->scope = scope;
  return boolean;
}
ASTExpr *Parser::parseNull() {
  ASTExpr *nil = new ASTExpr(AST_NULL);
  nil->scope = scope;
  return nil;
}

vector<ASTExpr*> Parser::parseArgumentList() {
  vector<ASTExpr*> list = vector<ASTExpr*>();
  match(Lparen);
  if(!curTokenIs(Rparen)) {
    ASTExpr *ident = parseIdentifier();
    match(Ident);
    match(Colon);
    ident->data_type = parseDatatype();
    nextToken();
    list.push_back(ident);
  }
  while (curTokenIs(Comma)) {
    match(Comma);
    ASTExpr *ident = parseIdentifier();
    match(Ident);
    match(Colon);
    ident->data_type = parseDatatype();
    nextToken();
    list.push_back(ident);
  }
  match(Rparen);
  return list;
}
ASTExpr *Parser::parseIfStatement() {
  ASTExpr *ifstmt = new ASTExpr(AST_IF);
  ifstmt->scope = scope;
  match(KW_If);

  if (curTokenIs(Lparen)) {
    addWarning("Left parenthesis not needed in if statement.");
    match(Lparen);
  }

  ifstmt->if_condition = parseExpr();

  if (curTokenIs(Rparen)) {
    addWarning("Right parenthesis not needed in if statement.");
    match(Rparen);
  }

  ifstmt->if_body = parseBlockStatement();

  if (curTokenIs(KW_Else)) {
    match(KW_Else);
    if (curTokenIs(KW_If)) {
      ifstmt->else_body = parseIfStatement();
    } else {
      ifstmt->else_body = parseBlockStatement();
    }
  }
  return ifstmt;
}
ASTExpr *Parser::parseBlockStatement() {
  ASTExpr *compound = new ASTExpr(AST_COMPOUND);
  compound->scope = scope;
  if (!curTokenIs(Lbrace)) {
    ASTExpr *stmt = parseStatement();
    stmt->scope = compound->scope;
    compound->compound_children.push_back(stmt);
    if(curTokenIs(Semicolon)) match(Semicolon);
    return compound;
  }
  
  match(Lbrace);
  while (!curTokenIs(Rbrace)) {
    ASTExpr *stmt = parseStatement();
    stmt->scope = compound->scope;
    if (stmt)
      compound->compound_children.push_back(stmt);
    if(curTokenIs(Semicolon)) match(Semicolon);
  }
  match(Rbrace);
  return compound;
}
ASTExpr *Parser::parseSource() {
  ASTExpr *compound = new ASTExpr(AST_COMPOUND);
  compound->is_global=true;
  compound->scope = scope;
  while (!curTokenIs(EOF_)) {

    ASTExpr *stmt = parseStatement();
    stmt->scope = compound->scope;
    if (curTokenIs(Semicolon))
      match(Semicolon);
    compound->compound_children.push_back(stmt);
  }

  return compound;
}

DataType *Parser::parseDatatype() {
  DataType *dt = new DataType(DT_I8);
  if (curTokenIs(KW_I16))
    dt->data_type = DT_I16;
  else if (curTokenIs(KW_I32))
    dt->data_type = DT_I32;
  else if (curTokenIs(KW_I64))
    dt->data_type = DT_I64;
  else if (curTokenIs(KW_I128))
    dt->data_type = DT_I128;
  else if (curTokenIs(KW_U8))
    dt->data_type = DT_U8;
  else if (curTokenIs(KW_U16))
    dt->data_type = DT_U16;
  else if (curTokenIs(KW_U32))
    dt->data_type = DT_U32;
  else if (curTokenIs(KW_U64))
    dt->data_type = DT_U64;
  else if (curTokenIs(KW_U128))
    dt->data_type = DT_U128;
  else if (curTokenIs(KW_F32))
    dt->data_type = DT_F32;
  else if (curTokenIs(KW_F64))
    dt->data_type = DT_F64;
  else if (curTokenIs(KW_F128))
    dt->data_type = DT_F128;
  else if (curTokenIs(KW_Char))
    dt->data_type = DT_CHAR;
  else if (curTokenIs(KW_String))
    dt->data_type = DT_STRING;
  else if (curTokenIs(KW_Boolean))
    dt->data_type = DT_BOOL;
  else {
    dt->data_type = DT_CLASS;
    dt->node = parseIdentifier();
  }
  if (peekTokenIs(Asterisk)) {
    nextToken();
    while (curTokenIs(Asterisk)) {
      dt->pointing_to = dt;
      dt->data_type = DT_POINTER;
      match(Asterisk);
    }
  }
  if (peekTokenIs(Lbrack)) {
    nextToken();
    while (curTokenIs(Lbrack)) {
      match(Lbrack);
      dt->array_ptr = dt;
      dt->data_type = DT_ARRAYTYPE;
      match(Rbrack);
    }
  }
  return dt;
}
ASTExpr *Parser::parseForStatement() {
  ASTExpr *forstmt = new ASTExpr(AST_FOR);
  forstmt->scope = scope;
  match(KW_For);

  if (curTokenIs(Lparen)) {
    addWarning("Left parenthesis not needed in if statement.");
    match(Lparen);
  }

  DataType *dt = parseDatatype();
  nextToken();
  ASTExpr *var = parseVariable();
  var->data_type = dt;
  forstmt->for_variable = var;
  match(Colon);
  forstmt->for_iterator = parseExpr();

  if (curTokenIs(Rparen)) {
    addWarning("Right parenthesis not needed in if statement.");
    match(Rparen);
  }

  forstmt->for_body = parseBlockStatement();
  return forstmt;
}
ASTExpr *Parser::parseWhileStatement() {
  ASTExpr *whilestmt = new ASTExpr(AST_WHILE);
  match(KW_While);

  if (curTokenIs(Lparen)) {
    addWarning("Left parenthesis not needed in while statement.");
    match(Lparen);
  }

  whilestmt->while_condition = parseExpr();

  if (curTokenIs(Rparen)) {
    addWarning("Right parenthesis not needed in while statement.");
    match(Rparen);
  }

  whilestmt->while_body = parseBlockStatement();
  return whilestmt;
}
ASTExpr *Parser::parseClassStatement() {
  ASTExpr *cls = new ASTExpr(AST_CLASS);
  match(KW_Class);

  cls->class_name = parseIdentifier();
  match(Ident);

  if (curTokenIs(Lt)) {
      match(Lt);

      std::vector<ASTExpr*> class_types = std::vector<ASTExpr*>();
      if (!curTokenIs(Gt)) {
          class_types.push_back(parseIdentifier());
          match(Ident);
      }

      while (curTokenIs(Comma)) {
          match(Comma);
          class_types.push_back(parseIdentifier());
          match(Ident);
      }
      cls->class_types = class_types;
      match(Gt);
  }
  
  cls->class_body = parseBlockStatement();

  for(auto i : cls->class_body->compound_children) {
    if (i->type == AST_VARIABLE_DEFINITION) cls->class_variables.push_back(i);
    if (i->type == AST_FUNCTION_DEFINITION) cls->class_functions.push_back(i);
    if (i->type == AST_CONSTRUCTOR) cls->class_constructors.push_back(i);
  }
  return cls;
}
ASTExpr *Parser::parseFunctionCall() {
  ASTExpr *function_call = new ASTExpr(AST_FUNCTION_CALL);
  function_call->function_call_name = parseIdentifier();
  function_call->scope = scope;
  match(Ident);
  match(Lparen);

  if (!curTokenIs(Rparen)) {
    ASTExpr *expr = parseExpr();
  
    function_call->function_call_arguments.push_back(expr);
  }
  while (curTokenIs(Comma)) {
    match(Comma);
    ASTExpr *expr = parseExpr();
    function_call->function_call_arguments.push_back(expr);
  }
  match(Rparen);
  return function_call;
}
ASTExpr *Parser::parseFunctionDefinition() {
  match(KW_Func);
bool is_void = true;
  DataType* dt = nullptr;
  if (curTokenIs(Lparen)) {
    match(Lparen);
    DataType* dt = parseDatatype();
    nextToken();
    match(Rparen);
    is_void = false;
  }
  ASTExpr *function_definition = new ASTExpr(AST_FUNCTION_DEFINITION);
  function_definition->data_type = (is_void ? new DataType(DT_VOID) : dt);
  function_definition->function_definition_name = parseIdentifier();
  function_definition->scope = scope;
  match(Ident);
  function_definition->function_definition_arguments = parseArgumentList();
  function_definition->function_definition_body = parseBlockStatement();
 return function_definition;
}
ASTExpr *Parser::parseStatement() {
  switch (curToken->kind) {
    case KW_If:
      return parseIfStatement();
    case KW_For:
      return parseForStatement();
    case KW_While:
      return parseWhileStatement();
    case KW_Func:
      return parseFunctionDefinition();
    case KW_Class:
      return parseClassStatement();
    case KW_Return:
      return parseReturn();
    case KW_Break:
      return parseBreak();
    case KW_Continue:
      return parseContinue();
    case KW_New:
    {
      nextToken();
      vector<ASTExpr*> list = parseArgumentList();

      if (curTokenIs(Lbrace)) {
        ASTExpr* cons = new ASTExpr(AST_CONSTRUCTOR);
        cons->function_definition_arguments = list;
        cons->function_definition_body = parseBlockStatement();
        return cons;
      }  
    }
    default: {
      if (curTokenDatatype()) {
        return parseVariableDefinition();
      }
      return parseExpression();
    }
  }
  printf("current token %s not statement\n", Token::ttString(curToken->kind).c_str());
  exit(1);
  return new ASTExpr(AST_NOOP);
}


ASTExpr *Parser::parseThis() {
  match(KW_This);
  return new ASTExpr(AST_THIS);
}
ASTExpr *Parser::parseFactor() {
  while (curTokenIs(Plus) || curTokenIs(Minus) || curTokenIs(Not) || curTokenIs(Asterisk) || curTokenIs(bAnd)) {
    ASTExpr *unop = new ASTExpr(AST_UNOP);
    unop->unop_operator = curToken->kind;
    unop->scope = scope;
    nextToken();
    unop->unop_right = parseTerm();
    return unop;
  }
  ASTExpr *ast = nullptr;
  if (curTokenIs(Ident)) {
    ast = parseVariable();
    return ast;
  }
  else if (curTokenIs(Lparen)) {
    match(Lparen);
    ast = parseExpr();
    match(Rparen);
  }
  else if (curTokenDatatype()) {
    DataType* dt = parseDatatype();
    nextToken();
    match(Colon);

    ASTExpr* type_conversion = new ASTExpr(AST_TYPE_CONVERSION);
    type_conversion->scope = scope;
    type_conversion->data_type= dt;
    type_conversion->type_conversion_to = parseExpr();
    ast = type_conversion;
    return ast;
  }
  if (curTokenIs(KW_True) || curTokenIs(KW_False))
    ast = parseBoolean();
  else if (curTokenIs(Int))
    ast = parseInteger(new DataType(default_int_type));
  else if (curTokenIs(Float))
    ast = parseFloat(new DataType(default_float_type));
  else if (curTokenIs(String))
    ast = parseString();
  else if (curTokenIs(Char))
    ast = parseChar();
  else if (curTokenIs(Lbrack))
    ast = parseArray();
  else if (curTokenIs(KW_This))
    ast = parseThis();
  else if(curTokenIs(KW_New)) {
      ast = parseNewStatement();
 
  }
  TokenType lst = curToken->kind;
  nextToken();
  
  if (!ast) {
    std::cerr << ("Unexpected token: `" + Token::ttString(lst) +
                  "`\n");
    exit(1);
  }

  return ast;
}
ASTExpr *Parser::parseTerm() {
  ASTExpr *node = parseFactor();

  while (curTokenIs(Asterisk) || curTokenIs(Slash)) {
    ASTExpr *ast = new ASTExpr(AST_BINOP);
    ast->scope = scope;
    TokenType kind = curToken->kind;
    nextToken();
    ast->binop_left = node;
    ast->binop_op = kind;
    ast->binop_right = parseFactor();
    node = ast;
  }

  

  while (curTokenIs(Dot)) {
    ASTExpr *ast = new ASTExpr(AST_ACCESS);
    ast->scope = scope;
    TokenType kind = curToken->kind;
    nextToken();
    ast->binop_left = node;
    ast->binop_op = kind;
    ast->binop_right = parseFactor();
    node = ast;
  }
  

  while (curTokenIs(Lbrack)) {
    ASTExpr *ast = new ASTExpr(AST_LIST_ACCESS);
    ast->scope = scope;
    match(Lbrack);
    ast->list_value = node;
    ast->list_access_pointer = parseExpr();
    match(Rbrack);
    node = ast;
  }

  while (curTokenIs(Shl) || curTokenIs(Shr)) {
    ASTExpr *ast = new ASTExpr(AST_BINOP);
    ast->scope = scope;
    nextToken();
    ast->binop_left = node;
    ast->binop_op = curToken->kind;
    ast->binop_right = parseFactor();
    node = ast;
  }
   while (curTokenIs(Modulo) || curTokenIs(Gt) || curTokenIs(Lt) ||
         curTokenIs(Eqe)) {
    ASTExpr *ast = new ASTExpr(AST_BINOP);
    ast->scope = scope;
    TokenType kind = curToken->kind;
    nextToken();
    ast->binop_left = node;
    ast->binop_op = kind;
    ast->binop_right = parseTerm();
    node = ast;
  }
  return node;
}
ASTExpr *Parser::parseExpression() {
  ASTExpr *node = parseTerm();
  while (curTokenIs(Plus) || curTokenIs(Minus)) {
    ASTExpr *ast = new ASTExpr(AST_BINOP);
    ast->scope = scope;
    TokenType kind = curToken->kind;
    nextToken();
    ast->binop_left = node;
    ast->binop_op =kind;
    ast->binop_right = parseTerm();
    node = ast;
  }

  while (curTokenIs(Eq)) {
      ASTExpr *ast = new ASTExpr(AST_BINOP);
      ast->scope = scope;
      ast->binop_left = node;
      ast->binop_op = curToken->kind;
      nextToken();
      ast->binop_right = parseTerm();
  }

 
  return node;
}
ASTExpr *Parser::parseExpr() {
  ASTExpr *node = parseExpression();

  while (curTokenIs(And) || curTokenIs(Or) || curTokenIs(Neq)) {
    ASTExpr *ast = new ASTExpr(AST_BINOP);
    ast->scope = scope;
    ast->binop_left = node;
    ast->binop_op = curToken->kind;
    ast->binop_right = parseExpression();
    node = ast;
  }


  return node;
}
ASTExpr *Parser::parseArray() {
  match(Lbrack);
  ASTExpr *array = new ASTExpr(AST_ARRAY);
  array->scope = scope;
  if (!curTokenIs(Rbrack)) {
    ASTExpr *expr = parseExpr();
    array->array_children.push_back(expr);
  }
  while (curTokenIs(Comma)) {
    match(Comma);
    array->array_children.push_back(parseExpr());
  }
  match(Rbrack);
  array->scope = scope;
  return array;
}
ASTExpr *Parser::parseIdentifier() {
  ASTExpr *ident = new ASTExpr(AST_IDENT);
  ident->identifier_value = curToken->text;
  ident->scope = scope;
  return ident;
}
ASTExpr *Parser::parseVariable() {
  ASTExpr *variable = new ASTExpr(AST_VARIABLE);
  variable->scope = scope;
  variable->variable_name = parseIdentifier();
  
  if (peekTokenIs(Lparen)) 
    return parseFunctionCall();
  else if(peekTokenIs(Ident)) 
    return parseVariableDefinition();
  nextToken();
  return variable;
}
ASTExpr *Parser::parseVariableDefinition() {
  ASTExpr *vdef = new ASTExpr(AST_VARIABLE_DEFINITION);
  vdef->scope = scope;
  vdef->data_type = parseDatatype();
  nextToken();
  vdef->variable_definition_name = parseIdentifier();

  nextToken();
  match(Eq);

  vdef->variable_definition_value = parseExpr();
  vdef->variable_definition_value->data_type = vdef->data_type;
  return vdef;
}

ASTExpr *Parser::parseReturn() {
  ASTExpr *ret = new ASTExpr(AST_RETURN);
  ret->scope = scope;
  match(KW_Return);
  ret->return_value = parseExpr();
  return ret;
}
ASTExpr *Parser::parseBreak() {
  ASTExpr *ret = new ASTExpr(AST_BREAK);
  ret->scope = scope;
  match(KW_Break);
  return ret;
}
ASTExpr *Parser::parseContinue() {
  ASTExpr *ret = new ASTExpr(AST_CONTINUE);
  ret->scope = scope;
  match(KW_Continue);
  return ret;
}
ASTExpr *Parser::parseNewStatement() {
  ASTExpr *newstmt = new ASTExpr(AST_NEW);
  newstmt->scope = scope;
  match(KW_New);
  newstmt->new_value = parseExpr();
  return newstmt;
}