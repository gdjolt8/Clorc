#include "../include/io.hpp"
#include <fstream>

#include "../include/main.hpp"
std::string read_file_contents(std::string path) {
  std::ifstream file(path);

  ClorAssert_True(file.is_open(), "File " + path + " doesn't exist");

  std::string result, line;

  while(getline(file, line)) {
    result += line + '\n';
  }

  result.erase(result.length()-1);

  return result;
}