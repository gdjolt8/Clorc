#ifndef CLORC_VM_HPP
#define CLORC_VM_HPP
#include <vector>

enum OpCode {
  PUSHL,
  PUSHS,
  PUSHC,
  PUSHB,
  CALL,
  LABEL,
  SAVEATMEM,
  LOADFROM,
  RET
};
class ClorVM
{
public:
  unsigned char *memory;
  unsigned char *stack;
  
  ClorVM();
  void ClorVm_RunBytecode(std::vector<unsigned char> bytecode);
};
#endif