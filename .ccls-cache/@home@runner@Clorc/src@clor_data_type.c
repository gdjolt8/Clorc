#include "../include/clor_data_type.h"
#include "../include/clor_AST.h"
#include <stdlib.h>
ClorDataType* init_clor_data_type(int type) {
  ClorDataType* data_type = (ClorDataType*)calloc(1, sizeof(ClorDataType));
  data_type->data_type = type;
  data_type->node = NULL;
  data_type->ptr = NULL;
  data_type->array_ptr = NULL;
  return data_type;
}

ClorDataType* init_clor_data_type_with_dt(ClorDataType* type) {
  ClorDataType* data_type = (ClorDataType*)calloc(1, sizeof(ClorDataType));
  data_type->data_type = type->data_type;
  data_type->node = type->node;
  data_type->ptr = type->ptr;
  data_type->array_ptr = type->array_ptr;
  return data_type;
}

const char* clor_data_type_to_string(ClorDataType* type) {
  switch (type->data_type) {
    case DT_I8: return "i8";
    case DT_I16: return "i16";
    case DT_I32: return "i32";
    case DT_I64: return "i64";
    case DT_I128: return "i128";
    case DT_F32: return "f32";
    case DT_F64: return "f64";
    case DT_F128: return "f128";
    case DT_U8: return "u8";
    case DT_U16: return "u16";
    case DT_U32: return "u32";
    case DT_U64: return "u64";
    case DT_U128: return "u128";
    case DT_CHAR: return "char";
    case DT_STRING: return "string";
    case DT_BOOL: return "bool";
    case DT_ARRAYTYPE: {
      int arrays = 0;
      ClorDataType *editable_dt = init_clor_data_type_with_dt(type);
      while (editable_dt->data_type == DT_ARRAYTYPE) {
        editable_dt = init_clor_data_type_with_dt(editable_dt->array_ptr);
        
        arrays++;
        if (editable_dt == NULL) break;
      }
      char *ptr = calloc(2*arrays+1000, sizeof(char));
      sprintf(ptr, "%s", clor_data_type_to_string(editable_dt));
      for(int i = 0; i < arrays; i++) {
        sprintf(ptr + strlen(ptr), "[]");
      }
      return ptr;
    }
    case DT_POINTER: return "{pointer}";
  }
  return "unk_type";
}

const char* clor_dt_integer_to_string(int type) {
    switch (type) {
    case DT_I8: return "i8";
    case DT_I16: return "i16";
    case DT_I32: return "i32";
    case DT_I64: return "i64";
    case DT_I128: return "i128";
    case DT_F32: return "f32";
    case DT_F64: return "f64";
    case DT_F128: return "f128";
    case DT_U8: return "u8";
    case DT_U16: return "u16";
    case DT_U32: return "u32";
    case DT_U64: return "u64";
    case DT_U128: return "u128";
    case DT_CHAR: return "char";
    case DT_STRING: return "string";
    case DT_BOOL: return "bool";
    case DT_ARRAYTYPE: {
      return "{array}";
    }
    case DT_POINTER: return "{pointer}";
  }
  return "unk_type";
}

void free_clor_data_type(ClorDataType* type) {
  if (type->array_ptr) free_clor_data_type(type->array_ptr);
  if (type->node) free_ast_expr(type->node);
  if (type->ptr) free_clor_data_type(type->ptr);
  free(type);
}