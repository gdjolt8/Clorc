#include "../include/scope.hpp"
#include "../include/AST.hpp"

Scope::Scope() {
  this->variable_definitions = std::vector<ASTExpr*>();
  this->function_definitions = std::vector<ASTExpr*>();
  this->class_definitions =  std::vector<ASTExpr*>();
}

void Scope::add_variable(ASTExpr* vdef) {

  for(int i = 0; i < variable_definitions.size();i++) {
    if(variable_definitions[i]->variable_definition_name->identifier_value == vdef->variable_definition_name->identifier_value) {
      variable_definitions[i] = vdef;
    }
  }
  variable_definitions.push_back(vdef);
}
void Scope::add_function(ASTExpr* fdef) {
  function_definitions.push_back(fdef);
}
void Scope::add_class(ASTExpr* cls) {
  class_definitions.push_back(cls);
}

ASTExpr* Scope::retrieve_variable(std::string vname) {
  for(auto& i : variable_definitions) {
    if (i->variable_definition_name->identifier_value == vname) {
        return i;
    }
  }
  return nullptr;
}
ASTExpr* Scope::retrieve_function(std::string fname) {
  for(auto& i : function_definitions) {
    if (i->function_definition_name->identifier_value == fname) {
        return i;
    }
  }
  return nullptr;
}
ASTExpr* Scope::retrieve_class(std::string cname) {
  for(auto& i : class_definitions) {
    if (i->class_name->identifier_value == cname) {
        return i;
    }
  }
  return nullptr;
}

Scope* Scope::adapt_scope(Scope* s){
  Scope* ns = new Scope();
  for(auto i : s->variable_definitions) ns->add_variable(i);
  for(auto i : s->function_definitions) ns->add_function(i);
  for(auto i : s->class_definitions) ns->add_class(i);
  return ns;
}