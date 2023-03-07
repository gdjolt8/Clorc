#include "../include/runtime.hpp"
#include "../include/main.hpp"
#include <iostream>
#include <memory>
bool booleanEvaluation(ASTExpr *node) {
  switch (node->type) {
  case AST_INT:
    return node->i128_value > 0;
  case AST_FLOAT:
    return node->f128_value > 0;
  case AST_STRING:
    return node->string_value.length() > 0;
  case AST_BOOLEAN:
    return node->bool_value;
  }
  return false;
}

bool arguments_equality(std::vector<ASTExpr *> left,
                        std::vector<ASTExpr *> right) {
  std::vector<ASTExpr *> lhs = left;
  std::vector<ASTExpr *> rhs = right;
  if (lhs.size() != rhs.size())
    return false;
  if (lhs.size() == 0)
    return true;
  int eq = 0;
  for (int i = 0; i < lhs.size(); i++) {

    ASTExpr *l = lhs[i];
    ASTExpr *r = rhs[i];
    ClorAssert_True(!!l->data_type, "L");
    int* ptr = nullptr;
    if(!!r->data_type) return *ptr;
    if (l->data_type->data_type == r->data_type->data_type) {
      eq++;
    }
  }

  if (eq == lhs.size())
    return true;

  return false;
}

bool isIterable(ASTExpr *node) {
  return (node->type == AST_ARRAY || node->type == AST_STRING);
}
ASTExpr *Runtime::visitAny(ASTExpr *node) {
  switch (node->type) {
  case AST_INT:
    return visitInteger(node);
  case AST_FLOAT:
    return visitFloat(node);
  case AST_STRING:
    return visitString(node);
  case AST_CHAR:
    return visitChar(node);
  case AST_BOOLEAN:
    return visitBoolean(node);
  case AST_ARRAY:
    return visitArray(node);
  case AST_FUNCTION_CALL:
    return visitFunctionCall(node);
  case AST_FUNCTION_DEFINITION:
    return visitFunctionDefinition(node);
  case AST_VARIABLE:
    return visitVariable(node);
  case AST_VARIABLE_DEFINITION:
    return visitVariableDefinition(node);
  case AST_COMPOUND:
    return visitCompound(node);
  case AST_BREAK:
    return node;
  case AST_CONTINUE:
    return node;
  case AST_IF:
    return visitIfStatement(node);
  case AST_FOR:
    return visitForStatement(node);
  case AST_WHILE:
    return visitWhileStatement(node);
  case AST_TYPE_CONVERSION:
    return visitTypeConversion(node);
  case AST_BINOP:
    return visitBinop(node);
  case AST_UNOP:
    return visitUnop(node);
  case AST_CLASS:
    return visitClassStatement(node);
  case AST_RETURN:
    return visitReturn(node);
  case AST_NEW:
    return visitNew(node);
  case AST_ACCESS:
    return visitAccess(node);
  case AST_CLASSPTR:
    return node;
  }
  printf("unimplemented type: %d\n", node->type);
  exit(1);
}
ASTExpr *Runtime::visitInteger(ASTExpr *node) { return node; }
ASTExpr *Runtime::visitFloat(ASTExpr *node) { return node; }
ASTExpr *Runtime::visitString(ASTExpr *node) { return node; }
ASTExpr *Runtime::visitChar(ASTExpr *node) { return node; }
ASTExpr *Runtime::visitBoolean(ASTExpr *node) { return node; }
ASTExpr *Runtime::visitArray(ASTExpr *node) {
  ASTExpr *newarr = new ASTExpr(AST_ARRAY);
  for (auto i : node->array_children) {
    newarr->array_children.push_back(visitAny(i));
  }
  return newarr;
}
ASTExpr *Runtime::visitVariableDefinition(ASTExpr *node) {
  if (node->data_type->node != nullptr) {
    ASTExpr *cls =
        node->scope->retrieve_class(node->data_type->node->identifier_value);
    if (!cls)
      ClorAssert_True(false, "No type named " +
                                 node->data_type->node->identifier_value + ".");
  }

  node->variable_definition_value = visitAny(node->variable_definition_value);
  node->scope->add_variable(node);
  return node;
}

ASTExpr *Runtime::visitVariable(ASTExpr *node) {
node->scope = scope;
  ASTExpr *vdef =
      node->scope->retrieve_variable(node->variable_name->identifier_value);
  std::cout << '\n' << "length: " << node->scope->variable_definitions.size() << '\n';
  bool exists = !!vdef;

  ClorAssert_True(exists, "Variable " + node->variable_name->identifier_value +
                              " doesn't exist in current scope");
  return visitAny(vdef->variable_definition_value);
}
ASTExpr *Runtime::visitFunctionCall(ASTExpr *node) {

  for (auto i : node->function_call_arguments) {
    i->scope = node->scope;
    i = visitAny(i);
  }


  if (node->function_call_name->identifier_value == "__putc") {
    expectArguments(node->function_call_arguments, 1, std::unique_ptr<int*>(new int{AST_CHAR}));
    ASTExpr *st = visitAny(node->function_call_arguments[0]);
    putc(st->char_value, stdout);
    return new ASTExpr(AST_NOOP);
  }

  if (node->function_call_name->identifier_value == "__getc") {
      expectArguments(node->function_call_arguments, 0, std::unique_ptr<int*>(new int{}));
      
      ASTExpr* chr = new ASTExpr(AST_CHAR);
      chr->char_value = getchar();
      return chr;
  }
  std::vector<ASTExpr *> func_names = std::vector<ASTExpr *>();
  ASTExpr *fdef = nullptr;
  bool fdef_exists = false;
  for (auto &i : node->scope->function_definitions) {
    if (i->function_definition_name->identifier_value ==
        node->function_call_name->identifier_value) {
      if (arguments_equality(i->function_definition_arguments,
                             node->function_call_arguments)) {
        fdef = i;
        fdef_exists = true;
      }
    }
  }

  if (!fdef_exists)
    ClorAssert_True(!!fdef, "Function " +
                                node->function_call_name->identifier_value +
                                " doesn't exist in current scope.");
  ClorAssert_True(!!fdef, "No overload for function " +
                              node->function_call_name->identifier_value +
                              " in current scope.");
  // expectArguments(node->function_call_arguments, 1, (int[]){1});
  ASTExpr *ast_variable = nullptr;

  ASTExpr *ast_value = nullptr;

  ASTExpr *ast_variable_definition = nullptr;

  for (int i = 0; i < node->function_call_arguments.size(); i++) {
    ast_variable = fdef->function_definition_arguments[i];
    ast_value = visitAny(node->function_call_arguments[i]);
    ast_variable_definition = new ASTExpr(AST_VARIABLE_DEFINITION);
    ast_variable_definition->data_type = ast_variable->data_type;
    ast_variable_definition->variable_definition_value = ast_value;
    ast_variable_definition->variable_definition_name = ast_variable;
    fdef->function_definition_body->scope->add_variable(
        ast_variable_definition);
  }

  for (auto i : fdef->function_definition_body->compound_children) {
    if (i->type == AST_RETURN) {
      return visitReturn(i);
    }
    visitAny(i);
  }
  return new ASTExpr(AST_NOOP);
}
ASTExpr *Runtime::visitFunctionDefinition(ASTExpr *node) {
  // check for already function

  for (auto &i : node->scope->function_definitions) {

    if (i->function_definition_name->identifier_value ==
        node->function_definition_name->identifier_value) {
      if (arguments_equality(i->function_definition_arguments,
                             node->function_definition_arguments)) {
        ClorAssert_True(false,
                        "Function (" +
                            i->function_definition_name->identifier_value +
                            ") already is declared with same arguments");
      }
    }
  }
  node->scope->add_function(node);
  return node;
}
ASTExpr *Runtime::visitIfStatement(ASTExpr *node) {
  if (booleanEvaluation(visitAny(node->if_condition)))
    return visitAny(node->if_body);
  else if (node->else_body) {
    return visitAny(node->else_body);
  }
  return new ASTExpr(AST_NOOP);
}
ASTExpr *Runtime::visitForStatement(ASTExpr *node) {
  ASTExpr *forstmt = new ASTExpr(AST_FOR);
  forstmt->for_variable = node->for_variable;
  forstmt->for_iterator = visitAny(node->for_iterator);
  forstmt->for_body = node->for_body;
  ASTExpr *vdef = new ASTExpr(AST_VARIABLE_DEFINITION);
  vdef->data_type = forstmt->for_variable->data_type;
  vdef->variable_definition_name = forstmt->for_variable->variable_name;
  ClorAssert_True(isIterable(forstmt->for_iterator),
                  "for statement iterator is not Iterable");

  int id;
  switch (forstmt->for_iterator->type) {
  case AST_ARRAY: {
    for (auto i : forstmt->for_iterator->array_children) {
      vdef->variable_definition_value = i;
      forstmt->for_body->scope->add_variable(vdef);
      visitAny(forstmt->for_body);
    }
    break;
  }
  case AST_STRING: {

    for (auto i : forstmt->for_iterator->string_value) {
      ASTExpr *chr = new ASTExpr(AST_CHAR);
      chr->char_value = i;

      vdef->variable_definition_value = chr;
      forstmt->for_body->scope->add_variable(vdef);
      visitAny(forstmt->for_body);
    }
    break;
  } break;
  }
  forstmt->for_body->scope->variable_definitions.erase(
      forstmt->for_body->scope->variable_definitions.begin() + id);
  return forstmt;
}

ASTExpr *Runtime::visitWhileStatement(ASTExpr *node) {
  while (booleanEvaluation(visitAny(node->while_condition))) {
    for (auto i : node->while_body->compound_children) {
      if (i->type == AST_BREAK)
        break;
      if (i->type == AST_CONTINUE)
        continue;
      visitAny(i);
    }
  }
}
ASTExpr *Runtime::visitClassStatement(ASTExpr *node) {
  ASTExpr *test = node->class_constructors[0];

  if (node->class_constructors.size() != 1) {
    for (auto i : node->class_constructors) {
      if (arguments_equality(test->function_definition_arguments,
                             i->function_definition_arguments)) {
        ClorAssert_True(false,
                        "In class definition: \"" +
                            node->class_name->identifier_value +
                            "\" multiple constructors with same arguments");
      }
      test = i;
    }
  }
  node->scope->add_class(node);
  return node;
}
ASTExpr *Runtime::visitBinop(ASTExpr *node) {
  node->binop_left->scope = node->scope;
  node->binop_right->scope = node->scope;
  ASTExpr *left = visitAny(node->binop_left);
  ASTExpr *right = visitAny(node->binop_right);

  switch (node->binop_op) {
  case Plus: {
    if (left->type == AST_INT && right->type == AST_INT) {
      ASTExpr *integer = new ASTExpr(AST_INT);
      integer->data_type = new DataType(DT_I128);
      integer->i128_value = left->i128_value + right->i128_value;
      return integer;
    }
    if (left->type == AST_FLOAT && right->type == AST_FLOAT) {
      ASTExpr *float_ = new ASTExpr(AST_FLOAT);
      float_->data_type = new DataType(DT_F128);
      float_->f128_value = left->f128_value + right->f128_value;
      return float_;
    }
    if (left->type == AST_FLOAT && right->type == AST_INT) {
        ASTExpr* integer = new ASTExpr(AST_INT);
        integer->data_type = new DataType(DT_F128);
        integer->i128_value = left->f128_value + right->i128_value;
        return integer;
    }
    if (left->type == AST_INT && right->type == AST_FLOAT) {
        ASTExpr* integer = new ASTExpr(AST_INT);
        integer->data_type = new DataType(DT_I128);
        integer->i128_value = left->i128_value + right->f128_value;
        return integer;
    }
    if (left->type == AST_STRING && right->type == AST_STRING) {
      ASTExpr *str = new ASTExpr(AST_STRING);
      str->data_type = new DataType(DT_STRING);
      str->string_value = left->string_value + right->string_value;
      return str;
    }
    break;
  
  }
  case Minus: {
      if (left->type == AST_INT && right->type == AST_INT) {
          ASTExpr* integer = new ASTExpr(AST_INT);
          integer->i128_value = left->i128_value - right->i128_value;
          return integer;
      }
      if (left->type == AST_FLOAT && right->type == AST_FLOAT) {
          ASTExpr* float_ = new ASTExpr(AST_FLOAT);
          float_->f128_value = left->f128_value - right->f128_value;
          return float_;
      }
      if (left->type == AST_STRING && right->type == AST_STRING) {
          ASTExpr* str = new ASTExpr(AST_STRING);
          str->string_value = left->string_value + right->string_value;
          return str;
      }
      break;

  }
  case Modulo: {
    if (left->type == AST_INT && right->type == AST_INT) {
      ASTExpr *integer = new ASTExpr(AST_INT);
      integer->i128_value = left->i128_value % right->i128_value;
      return integer;
    }
    if (left->type == AST_FLOAT && right->type == AST_FLOAT) {
      ASTExpr *float_ = new ASTExpr(AST_FLOAT);
      float_->f128_value =
          (int128_t)left->f128_value % (int128_t)right->f128_value;
      return float_;
    }
    break;
  }
  case Slash: {
    if (left->type == AST_INT && right->type == AST_INT) {
      ASTExpr *integer = new ASTExpr(AST_INT);
      integer->i128_value = left->i128_value / right->i128_value;
      return integer;
    }
    if (left->type == AST_FLOAT && right->type == AST_FLOAT) {
      ASTExpr *float_ = new ASTExpr(AST_FLOAT);
      float_->f128_value = left->f128_value / right->f128_value;
      return float_;
    }
    break;
  }
  case Lt: {
    if (left->type == AST_INT && right->type == AST_INT) {
      ASTExpr *b = new ASTExpr(AST_BOOLEAN);
      b->bool_value = left->i128_value < right->i128_value;
      return b;
    }
    break;
  }
  case Gt: {
    if (left->type == AST_INT && right->type == AST_INT) {
      ASTExpr *b = new ASTExpr(AST_BOOLEAN);
      b->bool_value = left->i128_value > right->i128_value;
      return b;
    }
    break;
  }
  case Eq: {

    if (node->binop_left->type != AST_VARIABLE) {
      exit(1);
    }

    ASTExpr *vdef = scope->retrieve_variable(
        node->binop_left->variable_name->identifier_value);

    if (!vdef)
      exit(2);

    vdef->variable_definition_value = right;

    scope->add_variable(vdef);

    return vdef->variable_definition_value;
  }
  default: {
      ClorAssert_True(false, "Invalid operator '" +  Token::ttString(node->binop_op) + "' for types " + std::to_string(left->type) + ", " + std::to_string(right->type));
  }
  }
}
ASTExpr *Runtime::visitUnop(ASTExpr *node) {
  node->unop_right->scope = node->scope;
  ASTExpr *type = visitAny(node->unop_right);
  ASTExpr *nw = nullptr;
  switch (node->unop_operator) {
  case Plus: {
    switch (type->type) {
      case AST_INT: {
        nw = new ASTExpr(AST_INT);
        nw->i128_value = +type->i128_value;
        return nw;
      }
      case AST_FLOAT: {
        nw = new ASTExpr(AST_FLOAT);
        nw->f128_value = +type->f128_value;
        return nw;
      }
    }
  }
  case Minus: {
    switch (type->type) {
      case AST_INT: {
        nw = new ASTExpr(AST_INT);
        nw->i128_value = -type->i128_value;
        return nw;
      }
      case AST_FLOAT: {
        nw = new ASTExpr(AST_FLOAT);
        nw->f128_value = -type->f128_value;
        return nw;
      }
    }
  }
  case Asterisk: {
    switch (type->type) {
      case AST_STRING: {
        nw = new ASTExpr(AST_CHAR);
        nw->char_value = type->string_value[0];
        return nw;
      }
      case AST_ARRAY: {
        return type->array_children[0];
      }
    }
  }
  }
  return new ASTExpr(AST_NOOP);
}
ASTExpr *Runtime::visitAccess(ASTExpr *node) {
  ASTExpr *binop_left = visitAny(node->binop_left);
  if (binop_left->type == AST_CLASSPTR) {
    ASTExpr *cls = binop_left->class_ptr;
    if (node->binop_right->type == AST_VARIABLE) {
      for (auto i : cls->class_variables) {
        if (i->variable_definition_name->identifier_value ==
            node->binop_right->variable_name->identifier_value) {
          return i->variable_definition_value;
        }
      }
      ClorAssert_True(false, "No variable named " +
                       node->binop_right->variable_name->identifier_value +
                       " in " + cls->class_name->identifier_value + " class."
                );
    }
    else if (node->binop_left->type == AST_FUNCTION_CALL) {
        for (auto i : cls->class_functions){
            if (i->function_definition_name->identifier_value ==
                node->binop_right->function_call_name->identifier_value)
            {
                ASTExpr* ast_variable = nullptr;
                ASTExpr* ast_value = nullptr;
                ASTExpr* ast_variable_definition = nullptr;

                for (int j = 0; j < node->function_call_arguments.size(); j++) {
                    ast_variable = i->function_definition_arguments[j];
                    ast_value = visitAny(node->binop_left->function_call_arguments[j]);
                    ast_variable_definition = new ASTExpr(AST_VARIABLE_DEFINITION);
                    ast_variable_definition->data_type = ast_variable->data_type;
                    ast_variable_definition->variable_definition_value = ast_value;
                    ast_variable_definition->variable_definition_name = ast_variable;
                    i->function_definition_body->scope->add_variable(
                        ast_variable_definition);
                }

                for (auto j: i->function_definition_body->compound_children) {
                    if (j->type == AST_RETURN) {
                        return visitReturn(j);
                    }
                    visitAny(j);
                }
            }
       }
    } else {
      ClorAssert_True(false, "What?");
    }
  }
  return new ASTExpr(AST_NOOP);
}
ASTExpr *Runtime::visitReturn(ASTExpr *node) {
  ASTExpr *new_ret = new ASTExpr(AST_RETURN);
  new_ret->return_value = visitAny(node->return_value);
  return new_ret->return_value;
}
ASTExpr *Runtime::visitListAccess(ASTExpr *node) {
  ASTExpr* value = visitAny(node->list_value);

  switch (value->type) {
    case AST_ARRAY: {
      if ((node->list_access_pointer->type != AST_INT)) ClorAssert_True(false, "List pointer is not integer.");
      return node->list_value->array_children[node->list_access_pointer->i128_value];
    }
    case AST_STRING: {
      if ((node->list_access_pointer->type != AST_INT)) ClorAssert_True(false, "List pointer is not integer.");
      ASTExpr* chr = new ASTExpr(AST_CHAR);
      chr->data_type = new DataType(DT_CHAR);
      chr->char_value = node->list_value->string_value[node->list_access_pointer->i128_value];
      return chr;
    }
  }
  return new ASTExpr(AST_NOOP);
}
ASTExpr *Runtime::visitCompound(ASTExpr *node) {
  if (node->is_global)
    this->scope = node->scope;
  for (auto i : node->compound_children) {
    i->scope = node->scope;
    visitAny(i);
  }
  return new ASTExpr(AST_NOOP);
}
ASTExpr *Runtime::visitNew(ASTExpr *node) {
  if (node->new_value->type == AST_FUNCTION_CALL) {
    ASTExpr *val = node->new_value;

    ASTExpr *cls =
        node->scope->retrieve_class(val->function_call_name->identifier_value);
    if (!cls) {
      ClorAssert_True(false, "Class " +
                                 val->function_call_name->identifier_value +
                                 " does NOT exist in this current scope.");
    }
    ASTExpr *newcsl = new ASTExpr(AST_CLASSPTR);
    newcsl->class_ptr = cls;

    return newcsl;
  }
  exit(1);
  return visitAny(node->new_value);
}

void Runtime::expectArguments(std::vector<ASTExpr *> arguments, int argc,
                              std::unique_ptr<int*> args) {
  ClorAssert_True(arguments.size() == argc,
                  "expected " + std::to_string(argc) + " arguments, but got " +
                      std::to_string(arguments.size()) + ".");

  size_t errors = 0;

  for (int i = 0; i < argc; i++) {
    ASTExpr *ast = visitAny(arguments[i]);

    if (ast->type != args[i]) {
      std::cerr << "For argument " << i + 1 << ", expected type "
                << ASTExpr::ASTType_String(static_cast<AST_Type>(args[i]))
                << ", but got "
                << ASTExpr::ASTType_String(static_cast<AST_Type>(ast->type))
                << '\n';
      errors++;
    }
  }

  if (errors) {
    std::cerr << "Terminating due to previous errors.\n";

    exit(1);
  }
}

ASTExpr *Runtime::visitTypeConversion(ASTExpr *node) {
  ASTExpr *tc = new ASTExpr(AST_TYPE_CONVERSION);

  node->type_conversion_to->scope = node->scope;
  tc->data_type = node->data_type;
  tc->type_conversion_to = visitAny(node->type_conversion_to);

  if (tc->data_type->data_type == DT_CHAR) {
    if (tc->type_conversion_to->type == AST_INT) {
      ASTExpr *chr = new ASTExpr(AST_CHAR);
      chr->char_value = (char)tc->type_conversion_to->i128_value;
      return chr;
    }
  }

  if (tc->data_type->data_type == DT_U8) {
    if (tc->type_conversion_to->type == AST_CHAR) {
      ASTExpr *chr = new ASTExpr(AST_INT);
      chr->u8_value = (uint8_t)tc->type_conversion_to->char_value;
      return chr;
    }
  }

  return tc->type_conversion_to;
}