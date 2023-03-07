#ifndef CLORC_AST_HPP
#define CLORC_AST_HPP

#include "token.hpp"
#include "data_type.hpp"
#include <cstdint>
#include <vector>
typedef long long int128_t;
typedef size_t uint128_t;

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
  AST_NOOP
};
class ASTExpr {
public:
  DataType *data_type;
  int type;
  bool is_global;
  std::vector<ASTExpr*> compound_children;
  uint8_t u8_value;
  uint16_t u16_value;
  uint32_t u32_value;
  uint64_t u64_value;
  uint128_t u128_value;

  int8_t i8_value;
  int16_t i16_value;
  int32_t i32_value;
  int64_t i64_value;
  int128_t i128_value;

  float f32_value;
  double f64_value;
  long double f128_value;

  char char_value;
  std::string string_value;
  bool bool_value;
  std::string identifier_value;

  ASTExpr* variable_name;
  // function call
  ASTExpr *function_call_name;
  std::vector<ASTExpr*> function_call_arguments;

  ASTExpr* class_ptr;
  ASTExpr *function_definition_name;
  std::vector<ASTExpr*> function_definition_arguments;
  ASTExpr *function_definition_body;

  std::vector<ASTExpr*> array_children;

  ASTExpr* binop_left;
  TokenType binop_op;
  ASTExpr* binop_right;

  ASTExpr* unop_right;
  TokenType unop_operator;

  ASTExpr *variable_definition_name;
  ASTExpr *variable_definition_value;

  ASTExpr *class_name;
  std::vector<ASTExpr*> class_variables;
  std::vector<ASTExpr*> class_functions;
  std::vector<ASTExpr*> class_constructors;
 
  ASTExpr *class_body;
  std::vector<ASTExpr*> class_types;
 

  ASTExpr* if_condition;
  ASTExpr* if_body;
  ASTExpr* else_body;

  ASTExpr* for_variable;
  ASTExpr* for_iterator;
  ASTExpr* for_body;

  ASTExpr* while_condition;
  ASTExpr* while_body;

  ASTExpr* return_value;

  ASTExpr* new_value;

  ASTExpr* list_access_pointer;
  ASTExpr* list_value;

  ASTExpr* type_conversion_to;
  
  char *pointer_address_str;
  ASTExpr* point_to;
  
  class Scope* scope;

  ASTExpr(int type);
  static std::string ASTType_String(AST_Type kind);
};
#endif