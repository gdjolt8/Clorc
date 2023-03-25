#include "../include/clor_parser.h"
#include "../include/main.h"
#include "../include/clor_scope.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
ClorParser_T* init_clor_parser(ClorLexer_T* lexer) {
  ClorParser_T* parser = (ClorParser_T*)calloc(1, sizeof(ClorParser_T));
  parser->lexer = lexer;
  parser->current_token = NULL;
  parser->peek_token = NULL;
  parser->parsing_errors = init_list();
  parser->parsing_warnings = init_list();
  parser->global_scope = init_clor_scope();
  clor_parser_next_token(parser);
  clor_parser_next_token(parser);
  return parser;
}
void clor_parser_next_token(ClorParser_T* parser) {
  parser->current_token = parser->peek_token;
  parser->peek_token = clor_lexer_read_next_token(parser->lexer);
  if (parser->lexer->error_list->used > 0) {
    ClorAssert(false, read_clor_error((ClorError_T*)parser->lexer->error_list->items[0]));
  }
}
void clor_parser_add_error(ClorParser_T* parser, char const* error) {
  clor_lexer_add_error_msg(parser->lexer, (void*)error);
}
void clor_parser_add_warning(ClorParser_T* parser, char const* warning) {
  add_element_list(parser->parsing_warnings, (void*)warning);
}
void clor_parser_match(ClorParser_T* parser, enum TokenType kind) {
  if (kind != parser->current_token->type) {
    char *buffer = calloc(72, sizeof(char));
    sprintf(buffer, "Expected '%s', but got '%s'.", token_type_to_string(kind), token_type_to_string(parser->current_token->type));
    clor_parser_add_error(parser, buffer);
  }
  clor_parser_next_token(parser);
}

int clor_parser_current_token_is(ClorParser_T* parser, enum TokenType kind) {
  return parser->current_token->type == kind;  
}
int clor_parser_peek_token_is(ClorParser_T* parser, enum TokenType kind) {
  return parser->peek_token->type == kind;  
}
int clor_parser_is_current_token_a_datatype(ClorParser_T* parser) {
  return (parser->current_token->type > DT_start && parser->current_token->type < DT_end);
}
ClorASTExpr* clor_parser_parse_integer(ClorParser_T* parser) {
  ClorASTExpr* integer = init_ast_expr(AST_INT);
  integer->data_type = init_clor_data_type(DT_I32);
  integer->i32_value = strtoll(parser->current_token->value, NULL, 10);
  integer->scope = init_clor_scope();
  return integer;
}
ClorASTExpr* clor_parser_parse_float(ClorParser_T* parser) {
  ClorASTExpr* floatlit = init_ast_expr(AST_FLOAT);
  floatlit->data_type = init_clor_data_type(DT_F64);
  floatlit->f64_value = strtold(parser->current_token->value, NULL);
  floatlit->scope = init_clor_scope();
  return floatlit;
}
ClorASTExpr* clor_parser_parse_character(ClorParser_T* parser){
  ClorASTExpr* character = init_ast_expr(AST_CHAR);
  character->data_type = init_clor_data_type(DT_CHAR);
  character->char_value = parser->current_token->value[0];
  character->scope = init_clor_scope();
  return character;
}
ClorASTExpr* clor_parser_parse_string_literal(ClorParser_T* parser){
  ClorASTExpr* string = init_ast_expr(AST_STRING);
  string->data_type = init_clor_data_type(DT_STRING);
  string->string_value = parser->current_token->value;
  string->scope = init_clor_scope();
  return string;
}
ClorASTExpr* clor_parser_parse_array(ClorParser_T* parser) {
  clor_parser_match(parser, Lbrack);
  ClorASTExpr* array = init_ast_expr(AST_ARRAY);
  array->scope = parser->global_scope;
  if (!clor_parser_current_token_is(parser, Rbrack)) {
    ClorASTExpr* expr = clor_parser_parse_expr(parser);
    add_element_list(array->array_children, expr);
  }

  while (clor_parser_current_token_is(parser, Comma)) {
    clor_parser_match(parser, Comma);
    add_element_list(array->array_children, clor_parser_parse_expr(parser));
  }
  clor_parser_match(parser, Rbrack);
  return array;
}
ClorASTExpr* clor_parser_parse_boolean(ClorParser_T* parser) {
  ClorASTExpr* boolean = init_ast_expr(AST_BOOLEAN);
  boolean->boolean_value = clor_parser_current_token_is(parser, KW_true);
  boolean->data_type = init_clor_data_type(DT_BOOL);
  boolean->scope = parser->global_scope;
  return boolean;
}
ClorASTExpr* clor_parser_parse_identifier(ClorParser_T* parser){
  ClorASTExpr* identifier = init_ast_expr(AST_IDENT);
  identifier->identifier_value = parser->current_token->value;
  identifier->scope = parser->global_scope;
  return identifier;
}
ClorASTExpr* clor_parser_parse_variable(ClorParser_T* parser) {
  ClorASTExpr* variable = init_ast_expr(AST_VARIABLE);
  variable->variable_name = clor_parser_parse_identifier(parser);
  variable->scope = parser->global_scope;

  if (clor_parser_peek_token_is(parser, Lparen)) return clor_parser_parse_function_call(parser);

  clor_parser_next_token(parser);
    while (clor_parser_current_token_is(parser, Eq)) {
    ClorASTExpr* ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    clor_parser_next_token(parser);
    ast->binop_left = variable;
    ast->binop_right = clor_parser_parse_expr(parser);
    variable = ast;
  }
  return variable;
}
ClorASTExpr* clor_parser_parse_if_statement(ClorParser_T* parser) {
  ClorASTExpr* if_statement = init_ast_expr(AST_IF);
  clor_parser_match(parser, KW_if);
  if_statement->scope = parser->global_scope;
  if (clor_parser_current_token_is(parser, Lparen)) {
    clor_parser_add_warning(parser, "Lparen not needed in if statement.");
    clor_parser_next_token(parser);
  }
  if_statement->if_condition = clor_parser_parse_expr(parser);
  if (clor_parser_current_token_is(parser, Rparen)) {
    clor_parser_add_warning(parser, "Rparen not needed in if statement.");
    clor_parser_next_token(parser);
  }
  if (clor_parser_current_token_is(parser, KW_else)) {
    clor_parser_match(parser, KW_else);
    if (clor_parser_current_token_is(parser, KW_if)) {
      if_statement->else_body = clor_parser_parse_if_statement(parser);
    } else {
      if_statement->else_body = clor_parser_parse_block_statement(parser);
    }
  }
  return if_statement;
}
ClorASTExpr* clor_parser_parse_for_statement(ClorParser_T* parser) {
  ClorASTExpr* for_statement = init_ast_expr(AST_FOR);
  clor_parser_match(parser, KW_for);
  for_statement->scope = parser->global_scope;
  if (clor_parser_current_token_is(parser, Lparen)) {
    clor_parser_add_warning(parser, "Lparen not needed in for statement.");
    clor_parser_next_token(parser);
  }
  ClorDataType* dt = clor_parser_parse_datatype(parser);

  for_statement->for_variable = clor_parser_parse_variable(parser);
  for_statement->for_variable->data_type = dt;
  clor_parser_match(parser, Colon);
  for_statement->for_iterator = clor_parser_parse_expr(parser);
  if (clor_parser_current_token_is(parser, Rparen)) {
    clor_parser_add_warning(parser, "Rparen not needed in for statement.");
    clor_parser_next_token(parser);
  }
  for_statement->else_body = clor_parser_parse_block_statement(parser);
  return for_statement;
}
ClorASTExpr* clor_parser_parse_while_statement(ClorParser_T* parser){
  ClorASTExpr* while_statement = init_ast_expr(AST_WHILE);
  clor_parser_match(parser, KW_while);
  while_statement->scope = parser->global_scope;
  if (clor_parser_current_token_is(parser, Lparen)) {
    clor_parser_add_warning(parser, "Lparen not needed in while statement.");
    clor_parser_next_token(parser);
  }
  while_statement->for_iterator = clor_parser_parse_expr(parser);
  if (clor_parser_current_token_is(parser, Rparen)) {
    clor_parser_add_warning(parser, "Rparen not needed in while statement.");
    clor_parser_next_token(parser);
  }
  while_statement->else_body = clor_parser_parse_block_statement(parser);
  return while_statement;
}
ClorASTExpr* clor_parser_parse_class_statement(ClorParser_T* parser) {
  ClorASTExpr* class = init_ast_expr(AST_CLASS);
  clor_parser_match(parser, KW_class);
  class->class_name = clor_parser_parse_identifier(parser);
  clor_parser_match(parser, Identifier);

  if (clor_parser_current_token_is(parser, Lt)) {
    list_T* class_types = init_list();
    clor_parser_match(parser, Lt);
    if (!clor_parser_current_token_is(parser, Gt)) {
      clor_parser_match(parser, Identifier);
      add_element_list(class_types, clor_parser_parse_identifier(parser));
      
    }

    while (clor_parser_current_token_is(parser, Comma)) {
      clor_parser_match(parser, Comma);
      add_element_list(class_types, clor_parser_parse_identifier(parser));
      clor_parser_match(parser, Identifier);
    }
    class->class_types = class_types;
    clor_parser_match(parser, Gt);
  }
  class->class_body = clor_parser_parse_block_statement(parser);
  for (int i = 0; i < class->class_body->compound_children->used; i++) {
    ClorASTExpr* expr = class->class_body->compound_children->items[i];
    
    if (expr->ast_type == AST_VARIABLE_DEFINITION) add_element_list(class->class_variable_definitions, expr);
    if (expr->ast_type == AST_FUNCTION_DEFINITION) add_element_list(class->class_function_definitions, expr);
    if (expr->ast_type == AST_CONSTRUCTOR) add_element_list(class->class_constructors, expr);
  }
  return class;
}
ClorASTExpr* clor_parser_parse_block_statement(ClorParser_T* parser) {
  ClorASTExpr* compound = init_ast_expr(AST_COMPOUND);
  compound->scope = parser->global_scope;
  if (!clor_parser_current_token_is(parser, Lbrace)) {
    ClorASTExpr *stmt = clor_parser_parse_statement(parser);
    stmt->scope = compound->scope;
    add_element_list(compound->compound_children, stmt);
    if(clor_parser_current_token_is(parser, Semicolon)) clor_parser_match(parser, Semicolon);
    return compound;
  }
  
  clor_parser_match(parser, Lbrace);
  while (!clor_parser_current_token_is(parser, Rbrace)) {
    ClorASTExpr* stmt = clor_parser_parse_statement(parser);
    stmt->scope = compound->scope;
    if (stmt) add_element_list(compound->compound_children, stmt);
    if (clor_parser_current_token_is(parser, Semicolon)) clor_parser_match(parser, Semicolon);
  }
  clor_parser_match(parser, Rbrace);
  return compound;
}
ClorASTExpr* clor_parser_parse_variable_definition(ClorParser_T* parser) {
  ClorASTExpr* variable_definition = init_ast_expr(AST_VARIABLE_DEFINITION);
  variable_definition->scope = parser->global_scope;
  variable_definition->variable_definition_name = clor_parser_parse_identifier(parser);
  clor_parser_match(parser, Identifier);
  clor_parser_match(parser, Colon);
  variable_definition->data_type = clor_parser_parse_datatype(parser);
  clor_parser_match(parser, Eq);
  variable_definition->variable_definition_value = clor_parser_parse_expr(parser);
  variable_definition->variable_definition_value->data_type = variable_definition->data_type;
  return variable_definition;
}
ClorASTExpr* clor_parser_parse_source_code(ClorParser_T* parser) {
  ClorASTExpr* compound = init_ast_expr(AST_COMPOUND);
  compound->is_global = true;
  compound->scope = parser->global_scope;

  while (!clor_parser_current_token_is(parser, END_OF_FILE)) {
    ClorASTExpr* statement = clor_parser_parse_statement(parser);
    statement->scope = compound->scope;
    if (clor_parser_current_token_is(parser, Semicolon)) {
      clor_parser_match(parser, Semicolon);
    }
    add_element_list(compound->compound_children, statement);
  }
  return compound;
}
ClorASTExpr* clor_parser_parse_return_statement(ClorParser_T* parser) {
  ClorASTExpr* ret = init_ast_expr(AST_RETURN);
  ret->scope = parser->global_scope;
  clor_parser_match(parser, KW_return);
  ret->return_value = clor_parser_parse_expr(parser);
  return ret;
}
ClorASTExpr* clor_parser_parse_new_statement(ClorParser_T* parser) {
  ClorASTExpr* new_statement = init_ast_expr(AST_NEW);
  new_statement->scope = parser->global_scope;
  clor_parser_match(parser, KW_new);
  new_statement->new_value = clor_parser_parse_expr(parser);
  return new_statement;
}
ClorASTExpr* clor_parser_parse_null_expr(ClorParser_T* parser) {
  ClorASTExpr* null = init_ast_expr(AST_NULL);
  clor_parser_match(parser, KW_null);
  return null;
}
ClorASTExpr* clor_parser_parse_function_call(ClorParser_T* parser) {
  ClorASTExpr* function_call = init_ast_expr(AST_FUNCTION_CALL);
  function_call->scope = parser->global_scope;
  function_call->function_call_name =clor_parser_parse_identifier(parser);
  clor_parser_match(parser, Identifier);
  clor_parser_match(parser, Lparen);
  if (!clor_parser_current_token_is(parser, Rparen)) {
    ClorASTExpr* expr = clor_parser_parse_expr(parser);
    add_element_list(function_call->function_call_arguments, expr);
  }

  while(clor_parser_current_token_is(parser, Comma)) {
    clor_parser_match(parser, Comma);
    ClorASTExpr* expr = clor_parser_parse_expr(parser);
    add_element_list(function_call->function_call_arguments, expr);
  }
  clor_parser_match(parser, Rparen);
  return function_call;
}
ClorASTExpr* clor_parser_parse_function_definition(ClorParser_T* parser) {
  clor_parser_match(parser, KW_func);
  bool is_void = true;
  ClorDataType* dt = NULL;
  if(clor_parser_current_token_is(parser, Lparen)) {
    clor_parser_match(parser, Lparen);
     dt = clor_parser_parse_datatype(parser);
    clor_parser_match(parser, Rparen);
    is_void = false;
  }

  ClorASTExpr* function_definition = init_ast_expr(AST_FUNCTION_DEFINITION);
  function_definition->scope = parser->global_scope;
  function_definition->data_type = (is_void ? init_clor_data_type(DT_VOID) : dt);
  function_definition->function_definition_name = clor_parser_parse_identifier(parser);
  clor_parser_match(parser, Identifier);
  function_definition->function_definition_arguments = clor_parser_parse_argument_list(parser);
  function_definition->function_definition_body = clor_parser_parse_block_statement(parser);

  for(int i = 0; i < function_definition->function_definition_body->compound_children->used; i++) {
    ClorASTExpr* stmt = function_definition->function_definition_body->compound_children->items[i];
    if (stmt->ast_type == AST_RETURN) function_definition->function_definition_return_value = stmt->return_value;
  }
  return function_definition;
}
ClorASTExpr* clor_parser_parse_statement(ClorParser_T* parser) {
  switch (parser->current_token->type) {
    case KW_if: return clor_parser_parse_if_statement(parser);
    case KW_for: return clor_parser_parse_for_statement(parser);
    case KW_while: return clor_parser_parse_while_statement(parser);
    case KW_func: return clor_parser_parse_function_definition(parser);
    case KW_class: return clor_parser_parse_class_statement(parser);
    case KW_return: return clor_parser_parse_return_statement(parser);
    case KW_break: return clor_parser_parse_break(parser);
    case KW_continue: return clor_parser_parse_continue(parser);
    case Identifier: { 
      if(clor_parser_peek_token_is(parser, Colon)) return clor_parser_parse_variable_definition(parser);
      return clor_parser_parse_variable(parser);
    }
    case KW_new: {
      clor_parser_next_token(parser);
      list_T* list = clor_parser_parse_argument_list(parser);
      if (clor_parser_current_token_is(parser, Lbrace)) {
        ClorASTExpr* cons = init_ast_expr(AST_CONSTRUCTOR);
        cons->function_definition_arguments = list;
        cons->function_definition_body = clor_parser_parse_block_statement(parser);
        return cons;
      }
    }
    
    default: {
      return clor_parser_parse_expr(parser);
    }
  }
}
ClorASTExpr* clor_parser_parse_factor(ClorParser_T* parser) {
  while (clor_parser_current_token_is(parser, Plus) || clor_parser_current_token_is(parser, Minus) || clor_parser_current_token_is(parser, Asterisk) || clor_parser_current_token_is(parser, BitAnd) || clor_parser_current_token_is(parser, Not)) {
    ClorASTExpr* unop = init_ast_expr(AST_UNOP);
    unop->unop_op = parser->current_token->type;
    unop->scope = parser->global_scope;
    clor_parser_next_token(parser);
    unop->unop_right = clor_parser_parse_term(parser);
    return unop;
  }

  ClorASTExpr* factor = NULL;
  bool move = true;
  if (clor_parser_current_token_is(parser, Identifier)) {

    factor = clor_parser_parse_variable(parser);
    move = false;
  }

  if (clor_parser_current_token_is(parser, Lparen)) {
    clor_parser_match(parser, Lparen);
    factor = clor_parser_parse_expr(parser);
    clor_parser_match(parser, Rparen);
    return factor;
  }

  if (clor_parser_is_current_token_a_datatype(parser)) {
    ClorDataType* data_type = clor_parser_parse_datatype(parser);
    clor_parser_next_token(parser);
    clor_parser_match(parser, Colon);

    ClorASTExpr* type_conversion = init_ast_expr(AST_TYPE_CONVERSION);
    type_conversion->scope = parser->global_scope;
    type_conversion->data_type = data_type;
    type_conversion->type_conversion_to = clor_parser_parse_expr(parser);

    return type_conversion;
  }

  if (clor_parser_current_token_is(parser, KW_true) || clor_parser_current_token_is(parser, KW_false)) {
    factor = clor_parser_parse_boolean(parser);
  } else if(clor_parser_current_token_is(parser, Integer)) {
    factor = clor_parser_parse_integer(parser);
  } else if(clor_parser_current_token_is(parser, FloatLit)) {
    factor = clor_parser_parse_float(parser);
  } else if(clor_parser_current_token_is(parser, Char)) {
    factor = clor_parser_parse_character(parser);
  } else if(clor_parser_current_token_is(parser, String)) {
    factor = clor_parser_parse_string_literal(parser);
  } else if(clor_parser_current_token_is(parser, Lbrack)) {
    factor = clor_parser_parse_array(parser);
  } else if(clor_parser_current_token_is(parser, Lbrack)) {
    factor = clor_parser_parse_this(parser);
  } else if(clor_parser_current_token_is(parser, KW_new)) {
    factor = clor_parser_parse_new_statement(parser);
  }
  int last = parser->current_token->type;
  if(move) clor_parser_next_token(parser);
  char *string = (char*)calloc(36 + strlen(token_type_to_string(last)), sizeof(char));
  sprintf(string, "Unexpected expression: '%s'\n", token_type_to_string(last));
  if (factor == NULL) {
    ClorAssert(!!factor, string);
  }
  return factor;
}
ClorASTExpr* clor_parser_parse_term(ClorParser_T* parser) {
  ClorASTExpr* node = clor_parser_parse_factor(parser);
  while (clor_parser_current_token_is(parser, Asterisk) || clor_parser_current_token_is(parser, Slash)) {
    ClorASTExpr* ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    clor_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = clor_parser_parse_factor(parser);
    node = ast;
  }

  while (clor_parser_current_token_is(parser, Dot)) {
    ClorASTExpr* ast = init_ast_expr(AST_ACCESS);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    clor_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = clor_parser_parse_factor(parser);
    node = ast;
  }

  while (clor_parser_current_token_is(parser, Shl) || clor_parser_current_token_is(parser, Shr) || clor_parser_current_token_is(parser, Modulo)) {
    ClorASTExpr* ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    clor_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = clor_parser_parse_factor(parser);
    node = ast;
  }

  while (clor_parser_current_token_is(parser, Lbrack)) {
    ClorASTExpr* ast = init_ast_expr(AST_LIST_ACCESS);
    ast->scope = parser->global_scope;
    clor_parser_match(parser, Lbrack);
    ast->list_value = node;
    ast->list_access_pointer = clor_parser_parse_expr(parser);
    clor_parser_match(parser, Rbrack);
    node = ast;
  }
  return node;
}
ClorASTExpr* clor_parser_parse_expression(ClorParser_T* parser) {
    ClorASTExpr* node = clor_parser_parse_term(parser);
  
  while (clor_parser_current_token_is(parser, Plus) || clor_parser_current_token_is(parser, Minus)) {
    ClorASTExpr* ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    clor_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = clor_parser_parse_term(parser);
    node = ast;
    
  }


  while (clor_parser_current_token_is(parser, Eq)) {
    ClorASTExpr* ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    clor_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = clor_parser_parse_expr(parser);
    node = ast;
  }
 
  return node;
}
ClorASTExpr* clor_parser_parse_expr(ClorParser_T* parser) {
    ClorASTExpr* node = clor_parser_parse_expression(parser);
  
  while (clor_parser_current_token_is(parser, And) || clor_parser_current_token_is(parser, Neq) || clor_parser_current_token_is(parser, Or)) {
    ClorASTExpr* ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    clor_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = clor_parser_parse_expression(parser);
    node = ast;
  }

  return node;
}
ClorASTExpr* clor_parser_parse_break(ClorParser_T* parser) {
  clor_parser_match(parser, KW_break);
  ClorASTExpr* brk = init_ast_expr(AST_BREAK);
  return brk;
}
ClorASTExpr* clor_parser_parse_continue(ClorParser_T* parser) {
  clor_parser_match(parser, KW_continue);
  ClorASTExpr* cnt = init_ast_expr(AST_CONTINUE);
  return cnt;
}
ClorASTExpr* clor_parser_parse_this(ClorParser_T* parser);
list_T* clor_parser_parse_argument_list(ClorParser_T* parser) {
  list_T* list = init_list();
  clor_parser_match(parser, Lparen);
  if (!clor_parser_current_token_is(parser, Rparen)) {
    ClorASTExpr* identifier = clor_parser_parse_identifier(parser);
    clor_parser_match(parser, Identifier);
    clor_parser_match(parser, Colon);
    identifier->data_type = clor_parser_parse_datatype(parser);
    add_element_list(list, identifier);
  }

  while (clor_parser_current_token_is(parser, Comma)) {
    clor_parser_match(parser, Comma);
    ClorASTExpr* identifier = clor_parser_parse_identifier(parser);
    clor_parser_match(parser, Identifier);
    clor_parser_match(parser, Colon);
    identifier->data_type = clor_parser_parse_datatype(parser);
    add_element_list(list, identifier);
  }
  clor_parser_match(parser, Rparen);
  return list;
}
ClorDataType* clor_parser_parse_datatype(ClorParser_T* parser) {
  ClorDataType* data_type = init_clor_data_type(-1);
  if (clor_parser_current_token_is(parser, KW_i8))
    data_type->data_type = DT_I8;
  if (clor_parser_current_token_is(parser, KW_i16))
    data_type->data_type = DT_I16;
  else if (clor_parser_current_token_is(parser, KW_i32))
    data_type->data_type = DT_I32;
  else if (clor_parser_current_token_is(parser, KW_i64))
    data_type->data_type = DT_I64;
  else if (clor_parser_current_token_is(parser, KW_i128))
    data_type->data_type = DT_I128;
  else if (clor_parser_current_token_is(parser, KW_u8))
    data_type->data_type = DT_U8;
  else if (clor_parser_current_token_is(parser, KW_u16))
    data_type->data_type = DT_U16;
  else if (clor_parser_current_token_is(parser, KW_u32))
    data_type->data_type = DT_U32;
  else if (clor_parser_current_token_is(parser, KW_u64))
    data_type->data_type = DT_U64;
  else if (clor_parser_current_token_is(parser, KW_u128))
    data_type->data_type = DT_U128;
  else if (clor_parser_current_token_is(parser, KW_f32))
    data_type->data_type = DT_F32;
  else if (clor_parser_current_token_is(parser, KW_f64))
    data_type->data_type = DT_F64;
  else if (clor_parser_current_token_is(parser, KW_f128))
    data_type->data_type = DT_F128;
  else if (clor_parser_current_token_is(parser, KW_char))
    data_type->data_type = DT_CHAR;
  else if (clor_parser_current_token_is(parser, KW_string))
    data_type->data_type = DT_STRING;
  else if (clor_parser_current_token_is(parser, KW_boolean))
    data_type->data_type = DT_BOOL;
  else {
    data_type->data_type = DT_CLASS;
    data_type->node = clor_parser_parse_class_identifier(parser);
 
  }
  
  if (clor_parser_peek_token_is(parser, Asterisk)) {
    clor_parser_next_token(parser);
    while (clor_parser_current_token_is(parser, Asterisk)) {
      data_type->ptr = data_type;
      data_type->data_type = DT_POINTER;
      clor_parser_match(parser, Asterisk);
    }
  }
  
  if (clor_parser_peek_token_is(parser, Lbrack)) {
    clor_parser_next_token(parser);
    while (clor_parser_current_token_is(parser, Lbrack)) {
      clor_parser_match(parser, Lbrack);
      data_type->array_ptr = init_clor_data_type_with_dt(data_type);
      data_type->data_type = DT_ARRAYTYPE;
      clor_parser_match(parser, Rbrack);
    }
    return data_type;
  }
  clor_parser_next_token(parser);
  return data_type;
}

ClorASTExpr* clor_parser_parse_class_identifier(ClorParser_T* parser) {
  ClorASTExpr* class_ident = init_ast_expr(AST_CLASS_IDENT);
  class_ident->class_identifier = clor_parser_parse_identifier(parser);
  if (clor_parser_peek_token_is(parser, Lt)) {
    clor_parser_match(parser, Identifier);
    clor_parser_match(parser, Lt);
    if (!clor_parser_current_token_is(parser, Gt)) {
      ClorDataType* data_type = clor_parser_parse_datatype(parser);
      add_element_list(class_ident->class_identifier_types, data_type);
    }

    while (clor_parser_current_token_is(parser, Comma)) {
      clor_parser_match(parser, Comma);
      ClorDataType* data_type = clor_parser_parse_datatype(parser);
      add_element_list(class_ident->class_identifier_types, data_type);
    }

  }
  
  return class_ident;
}