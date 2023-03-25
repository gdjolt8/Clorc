#ifndef CLORC_CLORC_AST_H
#define CLORC_CLORC_AST_H
#include "utils/list.h"
#include "clor_data_type.h"
#include "clor_token.h"
#include <stdint.h>

enum AST_Type {
  AST_COMPOUND,
  AST_INT,
  AST_FLOAT,
  AST_CHAR,
  AST_STRING,
  AST_BOOLEAN,
  AST_ARRAY,
  AST_IDENT,
  AST_VARIABLE,
  AST_VARIABLE_DEFINITION,
  AST_FUNCTION_CALL,
  AST_FUNCTION_DEFINITION,
  AST_IF,
  AST_FOR,
  AST_WHILE,
  AST_RETURN,
  AST_CLASS,
  AST_BINOP,
  AST_UNOP,
  AST_NULL,
  AST_NEW,
  AST_ACCESS,
  AST_LIST_ACCESS,
  AST_BREAK,
  AST_CONTINUE,
  AST_TYPE_CONVERSION,
  AST_POINTER,
  AST_CONSTRUCTOR,
  AST_THIS,
  AST_CLASSPTR,
  AST_CLASS_IDENT,
  AST_NOOP
};

typedef struct CLOR_ASTEXPR_STRUCT {
  int ast_type;
unsigned int is_global;
ClorDataType* data_type;
  list_T* compound_children;
  list_T* array_children;

  struct CLOR_ASTEXPR_STRUCT* function_definition_name;
  list_T* function_definition_arguments;
  struct CLOR_ASTEXPR_STRUCT* function_definition_body;
  struct CLOR_ASTEXPR_STRUCT* function_definition_return_value;
  char *string_value;
  unsigned int boolean_value;
  char *identifier_value;
  char char_value;

  struct CLOR_ASTEXPR_STRUCT* variable_name;

  struct CLOR_ASTEXPR_STRUCT* function_call_name;
  list_T* function_call_arguments;

  struct CLOR_ASTEXPR_STRUCT* variable_definition_value;
  struct CLOR_ASTEXPR_STRUCT* variable_definition_name;
  
  uint8_t u8_value;
  uint16_t u16_value;
  uint32_t u32_value;
  uint64_t u64_value;
  uintmax_t u128_value;

  int8_t i8_value;
  int16_t i16_value;
  int32_t i32_value;
  int64_t i64_value;
  intmax_t i128_value;

  float f32_value;
  double f64_value;
  long double f128_value;

  struct CLOR_ASTEXPR_STRUCT* binop_left;
  enum TokenType binop_op;
  struct CLOR_ASTEXPR_STRUCT* binop_right;

  struct CLOR_ASTEXPR_STRUCT* unop_right;
  enum TokenType unop_op;

  struct CLOR_ASTEXPR_STRUCT* class_name;
  list_T* class_variable_definitions;
  list_T* class_function_definitions;
  list_T* class_constructors;
  list_T* class_types;
  struct CLOR_ASTEXPR_STRUCT* class_body;
   struct CLOR_ASTEXPR_STRUCT* if_condition;
  struct CLOR_ASTEXPR_STRUCT* if_body;
  struct CLOR_ASTEXPR_STRUCT* else_body;

  struct CLOR_ASTEXPR_STRUCT* for_variable;
  struct CLOR_ASTEXPR_STRUCT* for_iterator;
  struct CLOR_ASTEXPR_STRUCT* for_body;

  struct CLOR_ASTEXPR_STRUCT* while_condition;
  struct CLOR_ASTEXPR_STRUCT* while_body;

  struct CLOR_ASTEXPR_STRUCT* return_value;

  struct CLOR_ASTEXPR_STRUCT* new_value;

  struct CLOR_ASTEXPR_STRUCT* list_access_pointer;
  struct CLOR_ASTEXPR_STRUCT* list_value;

  struct CLOR_ASTEXPR_STRUCT* type_conversion_to;
  
  char *pointer_address_str;
  struct CLOR_ASTEXPR_STRUCT* point_to;

  struct CLOR_ASTEXPR_STRUCT* class_identifier;
  list_T* class_identifier_types;
  struct CLOR_SCOPE_STRUCT* scope;

} ClorASTExpr;

ClorASTExpr* init_ast_expr(int type);
char const* ast_type_to_string(int type);
void free_ast_expr(ClorASTExpr* ast);
#endif