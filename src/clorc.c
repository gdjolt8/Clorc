#include "../include/clorc.h"
#include "../include/clor_lexer.h"
#include "../include/clor_parser.h"
#include "../include/clor_runtime.h"
#include "../include/clor_scope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clorc_compile_file(const char* directory, char *contents) {
  ClorLexer_T* lexer = init_clor_lexer(contents);
  lexer->directory = directory;
  ClorParser_T* parser = init_clor_parser(lexer);
  ClorASTExpr* statements = clor_parser_parse_source_code(parser);
  ClorRuntime* runtime = init_clor_runtime(parser->global_scope);
  clor_runtime_visit(runtime, statements);
  free_clor_scope(parser->global_scope);
  free(lexer);
  free(parser);
  free_ast_expr(statements);
  free(runtime);
  free(contents);
}