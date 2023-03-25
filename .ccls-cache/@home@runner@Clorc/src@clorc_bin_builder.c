#include "../include/clor_bin_builder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100000

ClorcBinBuilder *init_clorc_bin_builder() {
  ClorcBinBuilder* builder = calloc(1, sizeof(ClorcBinBuilder));
  builder->out = NULL;
  builder->vdefs = init_list();
  return builder;
}
char* clor_build_variable_definition(ClorcBinBuilder* builder, ClorASTExpr* ast) {
  char* buffer = (char*)calloc(MAX, sizeof(char));
}
char* clor_build_function_definition(ClorcBinBuilder* builder, ClorASTExpr* ast) {
  char* buffer = (char*)calloc(MAX, sizeof(char));
  
  if (ast->function_definition_return_value->ast_type == AST_STRING) {
    VarPair vdef = (VarPair) {".LC0", ast->function_definition_return_value}; 
    add_element_list(builder->vdefs, &vdef);
  }
  sprintf(buffer, "%s:\n\tpushq %%rbp\nmovq %%rsp, %%rbp\nnop\npopq %%rbp\npopq %%rbp\nret", ast->function_definition_name->identifier_value);
  // end of function
  sprintf(buffer + strlen(buffer), "popq %%rbp\npopq %%rbp\nret", ast->function_definition_name->identifier_value);
  return buffer;
}
char* clor_build_function_call(ClorcBinBuilder* builder, ClorASTExpr* ast);