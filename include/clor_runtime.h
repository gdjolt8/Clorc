#include "../include/clor_parser.h"
#include "../include/clor_AST.h"

typedef struct CLOR_RUNTIME_STRUCT {
  struct CLOR_SCOPE_STRUCT* global_scope;
} ClorRuntime;

ClorRuntime* init_clor_runtime(struct CLOR_SCOPE_STRUCT* scope);
ClorASTExpr* clor_runtime_visit(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_integer(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_float(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_char(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_string(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_boolean(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_identifier(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_array_value(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_function_call(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_function_definition(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_class_definition(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_variable(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_variable_definition(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_compound(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_binary_operation(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_unary_operation(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_list_access(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_if(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_for(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_while(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_null(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_return(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_access(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_break(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_continue(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_new(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_pointer(ClorRuntime* runtime, ClorASTExpr* node);
ClorASTExpr* clor_runtime_visit_type_conversion(ClorRuntime* runtime, ClorASTExpr* node);
void clor_runtime_expect_arguments(ClorRuntime* runtime, list_T* args, int argc, int arguments[], const char* fname);