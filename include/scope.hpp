#ifndef CLORC_SCOPE_HPP
#define CLORC_SCOPE_HPP

#include <vector>
#include "AST.hpp"
class Scope {
public:
  std::vector<ASTExpr*> function_definitions;
  std::vector<ASTExpr*> variable_definitions;
  std::vector<ASTExpr*> class_definitions;

  Scope();
  void add_variable(ASTExpr* vdef);
  void add_function(ASTExpr* fdef);
  void add_class(ASTExpr* cls);

  ASTExpr* retrieve_variable(std::string vname);
  ASTExpr* retrieve_function(std::string fname);
  ASTExpr* retrieve_class(std::string cname);
  Scope* adapt_scope(Scope* s);
};
#endif