#include "../include/clor_scope.h"
#include <stdlib.h>
#include <string.h>

ClorScope_T* init_clor_scope() {
  ClorScope_T* scope = (ClorScope_T*)calloc(1, sizeof(ClorScope_T));
  scope->scope_variable_definitions = init_list();
  scope->scope_function_definitions = init_list();
  scope->scope_class_definitions = init_list();
  return scope;
}

void free_clor_scope(ClorScope_T* scope) {
  if(!scope) return;
  free_list(scope->scope_variable_definitions);
  free_list(scope->scope_function_definitions);
  free_list(scope->scope_class_definitions);
  free(scope);
}
void add_variable_definitions_scope(ClorScope_T* scope, ClorASTExpr* variable_definition) {
  add_element_list(scope->scope_variable_definitions, variable_definition);
}
void add_function_definitions_scope(ClorScope_T* scope, ClorASTExpr* function_definition) {
  add_element_list(scope->scope_function_definitions, function_definition);
}
void add_class_definitions_scope(ClorScope_T* scope, ClorASTExpr* class_definition) {
  add_element_list(scope->scope_class_definitions, class_definition);
}

ClorASTExpr* retrieve_variable_definition(ClorScope_T* scope, const char *vname) {
  for(int i = 0; i < scope->scope_variable_definitions->used; i++) {
    ClorASTExpr* item = (ClorASTExpr*)scope->scope_variable_definitions->items[i];
    if(strcmp(item->variable_definition_name->identifier_value, vname) == 0) {
      return item;
    }
  }
  return NULL;
}
ClorASTExpr* retrieve_function_definition(ClorScope_T* scope, const char *fname) {
  for(int i = 0; i < scope->scope_function_definitions->used; i++) {
    ClorASTExpr* item = (ClorASTExpr*)scope->scope_function_definitions->items[i];
    if(strcmp(item->function_definition_name->identifier_value, fname)) {
      return item;
    }
  }
  return NULL;
}
ClorASTExpr* retrieve_class_definition(ClorScope_T* scope, const char *cname) {
  for(int i = 0; i < scope->scope_class_definitions->used; i++) {
    ClorASTExpr* item = (ClorASTExpr*)scope->scope_class_definitions->items[i];
    if(strcmp(item->class_name->identifier_value, cname) == 0) {
      return item;
    }
  }
  return NULL;
}