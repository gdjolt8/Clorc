#ifndef CLORC_CLORC_BIN_BUILDER_H
#define CLORC_CLORC_BIN_BUILDER_H
#include "../include/utils/list.h"
#include "../include/clor_AST.h"
typedef struct UTIL_PAIR_STRUCT {
  char* vdef_name;
  ClorASTExpr* vdef_value;
} VarPair;

typedef struct CLOR_BIN_BUILDER_STRUCT {
  char* out;
  list_T* vdefs;
} ClorcBinBuilder;

ClorcBinBuilder *init_clorc_bin_builder();
char* clor_build_variable_definition(ClorcBinBuilder* builder, ClorASTExpr* ast);
char* clor_build_function_definition(ClorcBinBuilder* builder, ClorASTExpr* ast);
char* clor_build_function_call(ClorcBinBuilder* builder, ClorASTExpr* ast);
#endif