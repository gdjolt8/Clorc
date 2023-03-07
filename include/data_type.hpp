#ifndef CLORC_DATA_TYPE_HPP
#define CLORC_DATA_TYPE_HPP

enum DataTypeEnum {
      DT_I8,DT_I16,DT_I32,DT_I64,DT_I128,DT_F32,DT_F64,DT_F128,DT_U8, DT_U16,DT_U32,DT_U64,DT_U128,DT_CHAR,DT_STRING,DT_BOOL,DT_ARRAYTYPE,DT_VOID,DT_CLASS, DT_POINTER
};
class DataType {
  public:
    
    DataTypeEnum data_type;
    DataType* array_ptr;
    DataType *pointing_to;
    class ASTExpr* node;
    DataType(int type);
};
#endif