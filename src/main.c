#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "../include/clorc.h"
#include "../include/file_sys.h"
#define DEBUG 1
void ClorHelp() {
  fprintf (stderr, "Clorc is the compiler for the programming language Clor. \n commands: \nhelp:\tdisplay this message.\nusage: clorc <option> [arguments]\n");
}

void ClorAssert(int b, char *error) {
  if (!b) {
    fprintf(stderr, "%s\n", error);
    exit (1);
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1 && !DEBUG)
  {
    ClorHelp();
    return 0;
  }
  
  if(DEBUG) {
    char *file_path = "./examples/main.clor";
    char *file = read_file_contents(file_path);
    clorc_compile_file(file_path, file);
    return 0;
  }
  if (!strcmp(argv[1], "help")) {
    ClorHelp();
    return 0;
  }

  if (!strcmp(argv[1], "compile")  ) {
    char *file_path = argv[2];
    char *file = read_file_contents((!DEBUG ? file_path : "./examples/main.clor"));
    clorc_compile_file(file_path, file);
  }
  return 0;
}