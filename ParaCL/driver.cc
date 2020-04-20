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
  prog.open("ParaCL.txt");
  std::cin.rdbuf(prog.rdbuf()); 
  FlexLexer* lexer = new yyFlexLexer;
  yy::Driver driver(lexer);
  driver.parse();
  //std::cout << "\n\n\n";
  fflush(stdin);
  Interpreter(&driver, driver.tree.head);
}
