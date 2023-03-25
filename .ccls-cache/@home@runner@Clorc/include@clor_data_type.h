#ifndef CLORC_CLORC_DATA_TYPE_H
#define CLORC_CLORC_DATA_TYPE_H

enum DataTypeEnum {
      DT_I8,DT_I16,DT_I32,DT_I64,DT_I128,DT_F32,DT_F64,DT_F128,DT_U8, DT_U16,DT_U32,DT_U64,DT_U128,DT_CHAR,DT_STRING,DT_BOOL,DT_ARRAYTYPE,DT_VOID,DT_CLASS, DT_POINTER
};

typedef struct CLOR_DATA_TYPE_STRUCT
{
  int data_type;
  struct CLOR_DATA_TYPE_STRUCT* array_ptr;
  struct CLOR_DATA_TYPE_STRUCT* ptr;
  struct CLOR_ASTEXPR_STRUCT* node;
} ClorDataType;

ClorDataType* init_clor_data_type(int type);
const char* clor_data_type_to_string(ClorDataType* type);
const char* clor_dt_integer_to_string(int type);
ClorDataType* init_clor_data_type_with_dt(ClorDataType* type);
void free_clor_data_type(ClorDataType* type);
#endif