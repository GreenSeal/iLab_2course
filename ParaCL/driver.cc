#include <iostream>
#include <fstream>

#include "ParaCL.hpp"
#include "Interpreter.hh"
// here we can return non-zero if lexing is not done inspite of EOF detected
int yyFlexLexer::yywrap() {
  return 1;
}

int main() { 
  std::ifstream prog;
  auto backup = std::cin.rdbuf();
  prog.open("Programm3.txt");
  std::cin.rdbuf(prog.rdbuf());
  FlexLexer* lexer = new yyFlexLexer;
  yy::Driver driver(lexer);
  driver.parse();
  std::cin.rdbuf(backup);
  Interpreter(&driver, driver.tree);
  delete lexer;
  driver.DeleteTree(driver.tree);
}
