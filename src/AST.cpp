#include "../include/AST.hpp"
#include "../include/scope.hpp"
ASTExpr::ASTExpr(int type)
{
  this->type = type;
  this->scope =  nullptr;
  this->array_children = std::vector<ASTExpr*>();
  this->function_call_arguments = std::vector<ASTExpr*>();
}

std::string ASTExpr::ASTType_String(AST_Type kind) {
  switch (kind) {
    case AST_INT: return "integer";
    case AST_FLOAT: return "float";
    case AST_STRING: return "string";
    case AST_BOOLEAN: return "boolean";
    case AST_CHAR: return "char";
    case AST_ARRAY: return "{array}";
    case AST_NULL: return "null";
  }
  return "none";
}