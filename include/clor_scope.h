#ifndef CLORC_CLORC_SCOPE_H
#define CLORC_CLORC_SCOPE_H
#include "utils/list.h"
#include "clor_AST.h"
typedef struct CLOR_SCOPE_STRUCT {
  list_T* scope_variable_definitions;
  list_T* scope_function_definitions;
  list_T* scope_class_definitions;
} ClorScope_T;

ClorScope_T* init_clor_scope();
void free_clor_scope(ClorScope_T* scope);
void add_variable_definitions_scope(ClorScope_T* scope, ClorASTExpr* variable_definition);
void add_function_definitions_scope(ClorScope_T* scope, ClorASTExpr* function_definition);
void add_class_definitions_scope(ClorScope_T* scope, ClorASTExpr* class_definition);

ClorASTExpr* retrieve_variable_definition(ClorScope_T* scope, const char *vname);
ClorASTExpr* retrieve_function_definition(ClorScope_T* scope, const char *fname);
ClorASTExpr* retrieve_class_definition(ClorScope_T* scope, const char *cname);
#endif