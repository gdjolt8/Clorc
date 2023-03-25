#include "../include/clor_runtime.h"
#include "../include/clor_AST.h"
#include "../include/main.h"
#include "../include/clor_scope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool clor_runtime_boolean_evaluation(ClorASTExpr* node) {
  switch(node->ast_type) {
    case AST_INT:
    return node->i32_value > 0;
    case AST_FLOAT:
    return node->f64_value > 0;
    case AST_STRING:
    return strlen(node->string_value) != 0;
    case AST_BOOLEAN:
    return node->boolean_value;
  }
  return false;
}

bool clor_runtime_arguments_is_equal(list_T* left, list_T* right) {
  list_T* lhs = left;
  list_T* rhs = right;
  if (lhs->used != rhs->used)
    return false;
  if(lhs->used == 0) {
    return true;
  }
  int equality = 0;
  for(int i = 0; i < lhs->used; i++) {
    ClorASTExpr* l = lhs->items[i];
    ClorASTExpr* r = rhs->items[i];
    ClorAssert(!!l->data_type, "Left");
    ClorAssert(!!r->data_type, "Right");
    if(l->data_type->data_type == r->data_type->data_type) {
      equality += 1;
    }
  }

  if(equality == lhs->used)
    return true;
  return false;
}
ClorRuntime* init_clor_runtime(struct CLOR_SCOPE_STRUCT* scope) {
  ClorRuntime* runtime = (ClorRuntime*)calloc(1, sizeof(ClorRuntime));
  runtime->global_scope = scope;
  return runtime;
}

ClorASTExpr* clor_runtime_visit(ClorRuntime* runtime, ClorASTExpr* node) {
  ClorAssert(!!node, "Node [Clor Runtime Visit] null");
  int*a=NULL;
  if(node->ast_type>1000) {
    return &a;
  }
  switch(node->ast_type)
  {
    case AST_INT: return clor_runtime_visit_integer(runtime, node);
    case AST_FLOAT: return clor_runtime_visit_float(runtime, node);
    case AST_CHAR: return clor_runtime_visit_char(runtime, node);
    case AST_STRING: return clor_runtime_visit_string(runtime, node);
    case AST_ARRAY: return clor_runtime_visit_array_value(runtime, node);
    case AST_BOOLEAN: return clor_runtime_visit_boolean(runtime, node);
    case AST_FUNCTION_CALL: return clor_runtime_visit_function_call(runtime, node);
    case AST_FUNCTION_DEFINITION: return clor_runtime_visit_function_definition(runtime, node);
    case AST_CLASS: return clor_runtime_visit_class_definition(runtime, node);
    case AST_VARIABLE: return clor_runtime_visit_variable(runtime, node);
    case AST_VARIABLE_DEFINITION: return clor_runtime_visit_variable_definition(runtime, node);
    case AST_COMPOUND: return clor_runtime_visit_compound(runtime, node);
    case AST_BINOP: return clor_runtime_visit_binary_operation(runtime, node);
    case AST_UNOP: return clor_runtime_visit_unary_operation(runtime, node);
    case AST_LIST_ACCESS: return clor_runtime_visit_list_access(runtime, node);
     case AST_IF: return clor_runtime_visit_if(runtime, node);
    case AST_FOR: return clor_runtime_visit_for(runtime, node);
    case AST_WHILE: return clor_runtime_visit_while(runtime, node);
    case AST_RETURN: return clor_runtime_visit_return(runtime, node);
     case AST_ACCESS: return clor_runtime_visit_access(runtime, node);
     case AST_NULL: return clor_runtime_visit_null(runtime, node);
    case AST_BREAK: return clor_runtime_visit_break(runtime, node);
    case AST_CONTINUE: return clor_runtime_visit_continue(runtime, node);
    default: {
      fprintf(stderr, "Unexpected AST type: `%d`\n", node->ast_type);
      exit(1);
    }
  }
}
ClorASTExpr* clor_runtime_visit_integer(ClorRuntime* runtime, ClorASTExpr* node) {
  return node;
}
ClorASTExpr* clor_runtime_visit_float(ClorRuntime* runtime, ClorASTExpr* node) {
  return node;
}
ClorASTExpr* clor_runtime_visit_char(ClorRuntime* runtime, ClorASTExpr* node) {
  return node;
}
ClorASTExpr* clor_runtime_visit_string(ClorRuntime* runtime, ClorASTExpr* node) {
  return node;
}
ClorASTExpr* clor_runtime_visit_boolean(ClorRuntime* runtime, ClorASTExpr* node) {
  return node;
}
ClorASTExpr* clor_runtime_visit_identifier(ClorRuntime* runtime, ClorASTExpr* node) {
  return node;
}
ClorASTExpr* clor_runtime_visit_array_value(ClorRuntime* runtime, ClorASTExpr* node) {
  ClorASTExpr* array = init_ast_expr(AST_ARRAY);
  array->data_type = node->data_type;
  for(int i = 0; i < node->array_children->used; i++) {
    add_element_list(array->array_children, clor_runtime_visit(runtime, node->array_children->items[i]));
  }
  return array;
}
ClorASTExpr* clor_runtime_visit_function_call(ClorRuntime* runtime, ClorASTExpr* node) {
  for(int i = 0; i < node->function_call_arguments->used; i++) {
    node->function_call_arguments->items[i] = clor_runtime_visit(runtime, node->function_call_arguments->items[i]);
  }

  if (strcmp(node->function_call_name->identifier_value, "__Putchar") == 0) {
    clor_runtime_expect_arguments(runtime, node->function_call_arguments, 1, (int[]){DT_CHAR}, "__Putchar");
    ClorASTExpr* chr = (ClorASTExpr*)node->function_call_arguments->items[0];
    putchar(chr->char_value);
    return init_ast_expr(AST_NOOP);
  }
  list_T* func_names = init_list();
  ClorASTExpr* fdef = NULL;
  int fdef_exists = false;
  for(int i = 0; i < node->scope->scope_function_definitions->used; i++) {
    ClorASTExpr* def = node->scope->scope_function_definitions->items[i];
    if (strcmp(def->function_definition_name->identifier_value, node->function_call_name->identifier_value) == 0) {
      fdef_exists = true;
      if (clor_runtime_arguments_is_equal(def->function_definition_arguments, node->function_call_arguments)) {
        fdef = def;
      }
    }
    
  }

  char *error = calloc(256, sizeof(char));
  sprintf(error, "Function definition `%s` does not exist.", node->function_call_name->identifier_value);
  ClorAssert(fdef_exists, error);
  
  sprintf(error, "No overload for function (%s) exists in current scope", node->function_call_name->identifier_value);
   ClorAssert(fdef != NULL, error);

  int *type_arguments = calloc(fdef->function_definition_arguments->used, sizeof(int));
  for(int i = 0; i < fdef->function_definition_arguments->used; i++) {
    type_arguments[i] = ((ClorASTExpr*)fdef->function_definition_arguments->items[i])->data_type->data_type;
  }
  clor_runtime_expect_arguments(runtime, node->function_call_arguments, fdef->function_definition_arguments->used, type_arguments, fdef->function_definition_name->identifier_value);
  
  ClorASTExpr* ast_variable = NULL;
  ClorASTExpr* ast_value = NULL;
  ClorASTExpr* ast_variable_definition = NULL;
  for(int i = 0; i < node->function_call_arguments->used; i++) {
    ast_variable = fdef->function_definition_arguments->items[i];
    ast_value = clor_runtime_visit(runtime, node->function_call_arguments->items[i]);
    ast_variable_definition = init_ast_expr(AST_VARIABLE_DEFINITION);
    ast_variable_definition->variable_definition_name = ast_variable;
    ast_variable_definition->data_type = ast_variable->data_type;
    ast_variable_definition->variable_definition_value = ast_value;
    add_variable_definitions_scope(fdef->function_definition_body->scope, ast_variable_definition);
  }

  for(int i = 0; i < fdef->function_definition_body->compound_children->used; i++) {
    ClorASTExpr* statement = fdef->function_definition_body->compound_children->items[i];
    if (statement->ast_type == AST_RETURN) {
      return clor_runtime_visit(runtime, statement);
    }
    clor_runtime_visit(runtime, statement);
  }
  return init_ast_expr(AST_NOOP);
}
ClorASTExpr* clor_runtime_visit_function_definition(ClorRuntime* runtime, ClorASTExpr* node) {
  add_function_definitions_scope(node->scope, node);
  return node;
}
ClorASTExpr* clor_runtime_visit_class_definition(ClorRuntime* runtime, ClorASTExpr* node) {
  add_class_definitions_scope(node->scope, node);
  return node;
}
ClorASTExpr* clor_runtime_visit_variable(ClorRuntime* runtime, ClorASTExpr* node) {
  ClorASTExpr* variable = retrieve_variable_definition(node->scope, node->variable_name->identifier_value);
  char *error = calloc(256, sizeof(char));
  sprintf(error, "Variable `%s` does not exist in current scope", node->variable_name->identifier_value);
  ClorAssert(!!variable, error);
  return clor_runtime_visit(runtime, variable->variable_definition_value);
}
ClorASTExpr* clor_runtime_visit_variable_definition(ClorRuntime* runtime, ClorASTExpr* node) {
  if (node->data_type->data_type == DT_CLASS) {
    ClorASTExpr* class = retrieve_class_definition(node->scope, node->data_type->node->class_identifier->identifier_value);

    if (node->data_type->node->class_identifier_types->used != class->class_types->used) {
      char *error = calloc(512, sizeof(char));
      sprintf(error, "Types (%s) for class definition and variable/function argument data type are contrasting", node->data_type->node->class_identifier->identifier_value);
      ClorAssert(false, error);
    }
    for(int i = 0; i < node->scope->scope_class_definitions->used; i++) {
      ClorASTExpr* a = node->scope->scope_class_definitions->items[i];
    }
    char *error = calloc(64 + strlen(node->data_type->node->class_identifier->identifier_value), sizeof(char));
    sprintf(error, "No type named `%s` in current scope.", node->data_type->node->class_identifier->identifier_value);
    ClorAssert (class != NULL, error); 
  }
  node->variable_definition_value = clor_runtime_visit(runtime, node->variable_definition_value);
  add_variable_definitions_scope(node->scope, node);
  return node;
}
ClorASTExpr* clor_runtime_visit_compound(ClorRuntime* runtime, ClorASTExpr* node) {
  for(int i = 0; i < node->compound_children->used; i++) {
    ClorASTExpr* statement = (ClorASTExpr*)node->compound_children->items[i];
    clor_runtime_visit(runtime, statement);
  }
  return node;
}
ClorASTExpr* clor_runtime_visit_binary_operation(ClorRuntime* runtime, ClorASTExpr* node) {
  ClorASTExpr* binop_left = clor_runtime_visit(runtime, node->binop_left);
  ClorASTExpr* binop_right = clor_runtime_visit(runtime, node->binop_right);

  switch(node->binop_op) {
    case Plus: {
      if (binop_left->ast_type == AST_INT && binop_right->ast_type == AST_INT) {
        ClorASTExpr* integer = init_ast_expr(AST_INT);
        integer->i32_value = binop_left->i32_value + binop_right->i32_value;
        integer->scope = node->scope;
        return integer;
      }
      if (binop_left->ast_type == AST_FLOAT && binop_right->ast_type == AST_FLOAT) {
        ClorASTExpr* floatlit = init_ast_expr(AST_FLOAT);
        floatlit->f64_value = binop_left->f64_value + binop_right->f64_value;
        floatlit->scope = node->scope;
        return floatlit;
      }
      if (binop_left->ast_type == AST_STRING && binop_right->ast_type == AST_STRING) {
        ClorASTExpr* string_lit = init_ast_expr(AST_STRING);
        char* string_val = calloc(strlen(binop_left->string_value) + strlen(binop_right->string_value) + 2, sizeof(char));
        sprintf(string_val, "%s%s", binop_left->string_value, binop_right->string_value);
        string_lit->string_value = string_val;
        string_lit->scope = node->scope;
        return string_lit;
      }
    }
    default: {
      char* err = calloc(200, sizeof(char));
      sprintf(err, "cannot use operator '%s' for %s and %s.", token_type_to_string(node->binop_op), ast_type_to_string(binop_left->ast_type), ast_type_to_string(binop_right->ast_type));
      ClorAssert(0, err);
    }
  }
}
ClorASTExpr* clor_runtime_visit_unary_operation(ClorRuntime* runtime, ClorASTExpr* node) {
  ClorASTExpr* right = clor_runtime_visit(runtime, node->unop_right);
  switch (node->unop_op) {
    case Plus: {
      if (right->ast_type == AST_INT) {
        ClorASTExpr* integer = init_ast_expr(AST_INT);
        integer->i32_value = +right->i32_value;
        return integer;
      } else if(right->ast_type == AST_FLOAT) {
        ClorASTExpr* flt = init_ast_expr(AST_FLOAT);
        flt->f64_value = +right->f64_value;
        return flt;
      }
    }
    case Minus: {
      if (right->ast_type == AST_INT) {
        ClorASTExpr* integer = init_ast_expr(AST_INT);
        integer->i32_value = -right->i32_value;
        return integer;
      } else if(right->ast_type == AST_FLOAT) {
        ClorASTExpr* flt = init_ast_expr(AST_FLOAT);
        flt->f64_value = -right->f64_value;
        return flt;
      }
    }

    
  }
}
ClorASTExpr* clor_runtime_visit_list_access(ClorRuntime* runtime, ClorASTExpr* node) {
  ClorASTExpr* value = clor_runtime_visit(runtime, node->list_value);
  switch(value->ast_type) {
    case AST_ARRAY: {
      if (node->list_access_pointer->ast_type != AST_INT) {
        
      }
    }
  }
}
ClorASTExpr* clor_runtime_visit_if(ClorRuntime* runtime, ClorASTExpr* node) {
  if (clor_runtime_boolean_evaluation(clor_runtime_visit(runtime, node->if_condition))) {
    return clor_runtime_visit(runtime, node->if_body);
  } else if (node->else_body) {
    return clor_runtime_visit(runtime, node->else_body);
  }
  return init_ast_expr(AST_NOOP);
}
ClorASTExpr* clor_runtime_visit_for(ClorRuntime* runtime, ClorASTExpr* node) {
  
}
ClorASTExpr* clor_runtime_visit_while(ClorRuntime* runtime, ClorASTExpr* node){
  while (clor_runtime_boolean_evaluation(clor_runtime_visit(runtime, node->while_condition))) {
    for(int i = 0; i < node->while_body->compound_children->used; i++) {
      ClorASTExpr* stmt = node->while_body->compound_children->items[i];
      if(stmt->ast_type == AST_BREAK) break;
      if(stmt->ast_type == AST_CONTINUE) continue;
      clor_runtime_visit(runtime, node);
    }
  }
}
ClorASTExpr* clor_runtime_visit_null(ClorRuntime* runtime, ClorASTExpr* node) {
  return node;
}
ClorASTExpr* clor_runtime_visit_return(ClorRuntime* runtime, ClorASTExpr* node) {
  return clor_runtime_visit(runtime, node->return_value);
}
ClorASTExpr* clor_runtime_visit_access(ClorRuntime* runtime, ClorASTExpr* node) {
  
}
ClorASTExpr* clor_runtime_visit_break(ClorRuntime* runtime, ClorASTExpr* node) {
  return node;
}
ClorASTExpr* clor_runtime_visit_continue(ClorRuntime* runtime, ClorASTExpr* node) {
  return node;
}
ClorASTExpr* clor_runtime_visit_new(ClorRuntime* runtime, ClorASTExpr* node) {
  
}
ClorASTExpr* clor_runtime_visit_pointer(ClorRuntime* runtime, ClorASTExpr* node) {
  
}
ClorASTExpr* clor_runtime_visit_type_conversion(ClorRuntime* runtime, ClorASTExpr* node) {
  
}

void clor_runtime_expect_arguments(ClorRuntime* runtime, list_T* args, int argc, int arguments[], const char* fname) {
  char *Err = calloc(1024, sizeof(char));
  sprintf(Err, "Function %s(%d), takes %d arguments, %zu were given.", fname, argc, argc, args->used);
  ClorAssert(argc == args->used, Err);
  free(Err);
  int errors = 0;
  for(int i = 0; i < args->used; i++) {
    ClorASTExpr* ast = args->items[i];
    if (ast->data_type->data_type != arguments[i]) {
      char* Er = calloc(1024, sizeof(char));
      sprintf(Er, "argument %d, expected type `%s`, but got `%s`.", i+1, clor_dt_integer_to_string(arguments[i]), clor_data_type_to_string(ast->data_type));
      fprintf(stderr, "%s\n", Er);
      errors++;
    }
  }

  if (errors) {
    fprintf(stderr, "Terminating due to previous errors\n");
    exit(1);
  }
  
}