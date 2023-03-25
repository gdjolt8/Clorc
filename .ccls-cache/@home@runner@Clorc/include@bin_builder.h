#ifndef CLORC_CLORC_BIN_BUILDER_H
#define CLORC_CLORC_BIN_BUILDER_H
#include "../include/utils/list.h"
#include "../include/clor_AST.h"
typedef struct CLOR_BIN_BUILDER_STRUCT {
  char* out;
} ClorcBinBuilder;

ClorcBinBuilder *init_clorc_bin_builder();
void clor_build_variable_definition(ClorASTExpr* ast);
#endif