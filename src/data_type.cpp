#include "../include/data_type.hpp"
#include "../include/AST.hpp"
DataType::DataType(int type) {
  this->data_type = static_cast<DataTypeEnum>(type);
  this->array_ptr = nullptr;
}