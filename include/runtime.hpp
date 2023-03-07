#ifndef CLORC_RUNTIME_HPP
#define CLORC_RUNTIME_HPP

#include "AST.hpp"
#include "scope.hpp"
#include <memory>
class Runtime {
Scope *scope;
public:
  ASTExpr* visitAny(ASTExpr* node);
  ASTExpr* visitInteger(ASTExpr* node);
  ASTExpr* visitFloat(ASTExpr* node);
  ASTExpr* visitString(ASTExpr* node);
  ASTExpr* visitChar(ASTExpr* node);
  ASTExpr* visitBoolean(ASTExpr* node);
  ASTExpr* visitArray(ASTExpr* node);
  ASTExpr* visitVariableDefinition(ASTExpr* node);
  ASTExpr* visitVariable(ASTExpr* node);
  ASTExpr* visitFunctionCall(ASTExpr* node);
  ASTExpr* visitFunctionDefinition(ASTExpr* node);
  ASTExpr* visitIfStatement(ASTExpr* node);
  ASTExpr* visitForStatement(ASTExpr* node);
  ASTExpr* visitWhileStatement(ASTExpr* node);
  ASTExpr* visitClassStatement(ASTExpr* node);
  ASTExpr* visitBinop(ASTExpr* node);
  ASTExpr* visitUnop(ASTExpr* node);
  ASTExpr* visitAccess(ASTExpr* node);
  ASTExpr* visitReturn(ASTExpr* node);
  ASTExpr* visitListAccess(ASTExpr* node);
ASTExpr* visitTypeConversion(ASTExpr* node);
ASTExpr* visitCompound(ASTExpr* node);
ASTExpr* visitNew(ASTExpr* node);
  
  void expectArguments(std::vector<ASTExpr*> arguments, int argc, std::unique_ptr<int*> args);
};
#endif