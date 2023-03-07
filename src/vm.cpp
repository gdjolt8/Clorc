#include "../include/vm.hpp"
#define MAX 1048576
ClorVM::ClorVM() {
  this->memory = new unsigned char[MAX];
  this->stack = new unsigned char[MAX];
}
void ClorVM::ClorVm_RunBytecode(std::vector<unsigned char> bytecode) {
  for (std::size_t i = 0; i < bytecode.size(); i++)
  {
    int opcode = bytecode[i];    

    switch (opcode) {
      case PUSHL: {
        // alloc 8 bytes
        for(int j = 0; j < 8; i++) {
          this->memory[j] = bytecode[++i];
        }
      }
      case PUSHS: {
        // alloc {len} bytes
        long len = bytecode[++i];
        for(int j = 0; j < len; i++) {
          this->memory[i] = bytecode[++i];
        }
      }
      case PUSHB: {
        this->memory[i] = bytecode[++i];
      }
      case PUSHC: {
        // alloc 1 bytes
        this->memory[i] = bytecode[++i];      
      }
      case CALL: {
        long call_loc = bytecode[++i];
        this->memory[i] = bytecode[++i];      
      }
    }
  }
}