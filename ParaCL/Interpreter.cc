#pragma once

#include "Interpreter.hh"

using namespace NodeTreeTypes;

int curr_scope = 0;

int Interpreter(yy::Driver* driver, ISyntaxTreeNode* node) {
  //std::cout << "Type: " << node -> type << std::endl;
  Types type = node -> type;
  if(type == Types::STLST) {
    Interpreter(driver, node -> GetLeft());
    Interpreter(driver, node -> GetRight());
    return -1;
  }

  if(type == Types::STM) {
   Interpreter(driver, node -> GetLeft());
   return -1;
  }  

  if(type == Types::EQUAL) {
    if(node -> GetLeft() -> type == Types::PRINT) {
      //std::cout << "PRINT\n";
      std::cout << CalculateRes(driver, node -> GetRight()) << std::endl;
      return -1;
      /*if(node -> GetRight() -> type == Types::VAR) {
	for(int i = 0; i <= curr_scope; ++i){
          if(driver -> vars[i].find(node -> GetRight() -> GetName()) != driver -> vars[i].end()) {
            std::cout << driver -> vars[i][node -> GetRight() -> GetName()] << std::endl;
            return -1;
          }
	}
      }*/

      /*else {
        std::cout << "Wrong using function print" << std::endl;
        std::abort();
      }*/
    }

    if(node -> GetLeft() -> type == Types::VAR) {

      if(node -> GetRight() -> type == Types::SCAN) {
        //std::cout << "SCAN\n";
        int a = 10;
        std::cin >> a;
	for(int i = 0; i <= curr_scope; ++i){
	  if(driver -> vars[i].find(node -> GetLeft() -> GetName()) != driver -> vars[i].end()) {
		  driver -> vars[i][node -> GetLeft() -> GetName()] = a;
		  return -1;
          }
	}
        driver -> vars[curr_scope][node -> GetLeft() -> GetName()] = a;
        return -1;
      }

      else {
        //std::cout << "EXPR\n";
	for(int i = 0; i <= curr_scope; ++i){
          if(driver -> vars[i].find(node -> GetLeft() -> GetName()) != driver -> vars[i].end()) {
                  driver -> vars[i][node -> GetLeft() -> GetName()] = CalculateRes(driver, node -> GetRight());
                  return -1;
          }
        }

        driver -> vars[curr_scope][node -> GetLeft() -> GetName()] = CalculateRes(driver, node -> GetRight());
        return -1;
      }
    }

    else {
      std::cout << "Wrong statement" << std::endl; 
      std::abort();
    }
  }

  if(type == Types::IF) {
    //std::cout << "IF\n";
    if(CalculateRes(driver, node -> GetLeft()) != 0) {
      //std::cout << "IF_TRUE\n";
      ++curr_scope;
      std::unordered_map<std::string, int> map;
      driver->vars.push_back(map);
      Interpreter(driver, node -> GetRight());
      driver -> vars.pop_back();
      --curr_scope;
    } 
    return -1;
  }

  if(type == Types::WHILE) {
    //std::cout << "WHILE\n";
    if(CalculateRes(driver, node -> GetLeft()) != 0) {
      //std::cout << "WHILE_TRUE\n";
      ++curr_scope;
      std::unordered_map<std::string, int> map;
      driver -> vars.push_back(map);
      Interpreter(driver, node -> GetRight());
      driver -> vars.pop_back();
      --curr_scope;
      Interpreter(driver, node);
    }
    return -1;
  }

  else {
    std::cout << "Something goes wrong. Wrote to Den Tomashev\n";
    std::abort();
  }
}

double CalculateRes(yy::Driver * driver, ISyntaxTreeNode * node) {
  switch(node -> type) {
    case Types::NUM: return node -> GetData();
    
    case Types::VAR: {
		       for(int i = 0; i <= curr_scope; ++i) {
			 if((driver -> vars[i].find(node -> GetName())) != driver -> vars[i].end()) return driver -> vars[i][node -> GetName()];
		       }

		       std::cout << "No var\n";
		       std::abort();
		     }

    case Types::PLUS: return CalculateRes(driver, node -> GetLeft()) + CalculateRes(driver, node -> GetRight());

    case Types::MINUS: return CalculateRes(driver, node -> GetLeft()) - CalculateRes(driver, node -> GetRight());

    case Types::MULT: return CalculateRes(driver, node -> GetLeft()) * CalculateRes(driver, node -> GetRight());
    
    case Types::DIV: return CalculateRes(driver, node -> GetLeft()) / CalculateRes(driver, node -> GetRight());

    case Types::UMINUS: return -CalculateRes(driver, node -> GetLeft());

    case Types::DEQUAL: {
			  if (CalculateRes(driver, node -> GetLeft()) == CalculateRes(driver, node ->GetRight())) return 1;

			  else return 0;
			}

    case Types::MORE: {
                        if (CalculateRes(driver, node -> GetLeft()) > CalculateRes(driver, node ->GetRight())) return 1;

                        else return 0;
                      }

    case Types::LESS: {
                        if (CalculateRes(driver, node -> GetLeft()) < CalculateRes(driver, node ->GetRight())) return 1;

                        else return 0;
                      }

    case Types::AND: return (CalculateRes(driver, node -> GetLeft()) && CalculateRes(driver, node -> GetRight()));

    case Types::OR: return(CalculateRes(driver, node -> GetLeft()) || CalculateRes(driver, node -> GetLeft()));

    default: {
	       std::cout << "Wrong type on expr tree: " << node -> type << std::endl;
	       std::abort();
	     }
  }
}
