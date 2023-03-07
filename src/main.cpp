#include <iostream>
#include <vector>
#include <cstring>

#include "../include/io.hpp"
#include "../include/lexer.hpp"

#include "../include/parser.hpp"
#include "../include/runtime.hpp"
void ClorPrint_Help() {
  fprintf(stderr, "Clorc is a tool for managaing Clor programming language code.\nusage: clorc [option] <file>?\ntools: help: prints help\nby default: compiles .clor file\n");
}

void ClorAssert_True(bool _b, std::string err) {
  if(!_b) {
     
    std::cerr << err << '\n';
    exit(1);
  }
}
int main(int argc, char **argv) {
  if(argc < 2) {
    ClorPrint_Help();
    return 1;
  }

  std::vector<std::string> args;
  
  for(int i = 0; i < argc; i++) {
    args.push_back(std::string(argv[i]));
  }
  if(args[1] == "help") {
    ClorPrint_Help();
  }

  
  std::string file_contents = read_file_contents(args[1]);
  char* ptr = strdup(argv[1]);

  if (std::string(ptr).substr(strlen(ptr) - 4) == "clor") {
      ClorAssert_True(false, "File argument must be a .clor file.");
  }
  
  Lexer* lexer = new Lexer(file_contents);
  lexer->filename = args[1];
  Parser* parser = new Parser(lexer);
  ASTExpr* stmts = parser->parseSource();
  Runtime* rnt = new Runtime();
  rnt->visitAny(stmts);
}