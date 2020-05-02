#include <iostream>
#include <fstream>

#include "ParaCL.hpp"
#include "Interpreter.hh"
// here we can return non-zero if lexing is not done inspite of EOF detected
int yyFlexLexer::yywrap() {
  return 1;
}

void PrintVars(yy::Driver * driver) {
  for(int i = 0; i < driver -> vars.size(); ++i) {
    std::cout << "i = " << i << std::endl;
    for(auto it = driver -> vars[i].begin(); it != driver -> vars[i].end(); ++it) {
      std::cout << "map = " << it -> first << " " << it -> second << std::endl;
    }
  }
}

int main() { 
  std::ifstream prog;
  std::string progname;
  std::cin >> progname;             
  auto backup = std::cin.rdbuf();
  prog.open(progname);
  if(!prog.is_open()) {
    std::cout << "File doesn't exist\n";
    std::abort();
  }
  std::cin.rdbuf(prog.rdbuf());
  FlexLexer* lexer = new yyFlexLexer;
  yy::Driver driver(lexer);
  driver.parse();
  std::cin.rdbuf(backup);
  std::unordered_map<std::string, int> map;
  driver.vars.push_back(map);
  assert(driver.tree != NULL);
  Interpreter(&driver, driver.tree);
  delete lexer;
  driver.DeleteTree(driver.tree);
}
