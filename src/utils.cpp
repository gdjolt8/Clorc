#include "../include/utils.hpp"
#include <cstring>

#define min(a, b) a > b ? b : a
char steq(const char* instr)
{
  if (strlen(instr) <= 1) return 0;
  if (instr[0] != '\\') return 0;
  char in_c = instr[1];

  switch (in_c)
  {
    case 'n': return '\n';
    case 't': return '\t';
    case 'r': return '\r';
    case 'b': return '\b';
    case '0': return 0;
    case '\\': return in_c;
    default: return in_c;
  }

  return in_c;
}

std::string string_formatted(std::string s) {
  std::string newstr = "";
  size_t len = s.length();

  size_t i = 0;

  while(s.c_str()[i] != '\0' && i < len) {
    if(s.c_str()[i] == '\\') {
      char escape = steq(new char[3]{s.c_str()[i], s.c_str()[min(i + 1, len)], '\0'});
      if(s.c_str()[i + 1] == 'x') {
        escape = strtol(new char[3] {s.c_str()[i+2], s.c_str()[i+3], '\0'}, NULL, 16);
        i += 2;
      }
      newstr.append(new char[2]{escape, '\0'});
      i++;
    } else {
      newstr.append(new char[2]{s.c_str()[i], 0});
    }
    i++;
  }
  return newstr;
}

