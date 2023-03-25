#ifndef CLORC_CLORC_PARSER_H
#define CLORC_CLORC_PARSER_H

#include "clor_lexer.h"
#include "clor_AST.h"

typedef struct CLOR_PARSER_STRUCT {
  ClorLexer_T* lexer;
  ClorToken_T* current_token;
  ClorToken_T* peek_token;
  list_T* parsing_errors;
  list_T* parsing_warnings;
  struct CLOR_SCOPE_STRUCT* global_scope;
} ClorParser_T;

ClorParser_T* init_clor_parser(ClorLexer_T* lexer);
void clor_parser_next_token(ClorParser_T* parser);
void clor_parser_add_error(ClorParser_T* parser, char const* error);
void clor_parser_add_warning(ClorParser_T* parser, char const* warning);
void clor_parser_match(ClorParser_T* parser, enum TokenType kind);
int clor_parser_current_token_is(ClorParser_T* parser, enum TokenType kind);
int clor_parser_peek_token_is(ClorParser_T* parser, enum TokenType kind);
int clor_parser_is_current_token_a_datatype(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_integer(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_float(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_character(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_string_literal(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_array(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_boolean(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_identifier(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_variable(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_if_statement(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_for_statement(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_while_statement(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_class_statement(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_block_statement(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_variable_definition(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_source_code(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_return_statement(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_new_statement(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_null_expr(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_function_call(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_function_definition(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_statement(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_factor(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_term(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_expression(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_expr(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_break(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_continue(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_this(ClorParser_T* parser);
list_T* clor_parser_parse_argument_list(ClorParser_T* parser);
ClorDataType* clor_parser_parse_datatype(ClorParser_T* parser);
ClorASTExpr* clor_parser_parse_class_identifier(ClorParser_T* parser);
#endif