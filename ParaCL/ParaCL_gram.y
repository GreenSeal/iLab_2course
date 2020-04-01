%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::NumDriver* driver}

%code requires
{
#include <iostream>
#include <string>
#include <unordered_map>

namespace yy {class NumDriver; }
}

%code
{
#include "ParaCL.hpp"

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,
      			 NumDriver* driver);

}

}

%token
  EQUAL   "="
  MINUS   "-"
  PLUS    "+"
  SCOLON  ";"
  MULT    "*"
  DIV     "/"
  L_BRACE "("
  R_BRACE ")"
  MORE    ">"
  LESS    "<"
  L_FGR   "{"
  R_FGR   "}"
  WHILE   "while"
  IF      "if"
  SCAN    "?"
  PRINT   "print"
  ERR
;

%token <int> NUMBER
%token <std::string> VAR
%nterm <int> expr
%nterm <int> statement

%left '+' '-' '*' '/'

%start program

%%

program : statelist
;

statelist : statement SCOLON statelist
| statement SCOLON
| construction statelist
| construction
;

construction: cycle
| if
;

statement: VAR      		 {std::cout << $1 << std::endl;
	 			  if (driver -> find($1) == driver -> end()){
				    driver -> vars.insert({$1, 0});
				  }
				  else {
				    std::cout << "Double decl of " << $1 << std::endl;
				    std::abort(); 
				  }
	                         }
| VAR EQUAL expr        	 {
				  if (driver -> vars.find($1) == driver -> vars.end()){
                                    driver -> vars.insert({$1, $3});
                                  }
                                  else {
                                    driver -> vars[$1] = $3;
                                  }
				 }
| VAR EQUAL EQUAL expr           {
				  if (driver -> vars.find($1) == driver -> vars.end()){
                                    std::cout << "Wrong variable" << std::endl;
				    std::abort();
                                  }
                                  else {
                                    if(driver -> vars[$1] == $4) $$ = 1;
				    
				    else $$ = 0;
                                  }
				 }
| VAR MORE expr                  {
				  if (driver -> vars.find($1) == driver -> vars.end()){
                                    std::cout << "Wrong variable" << std::endl;
                                    std::abort();
                                  }
                                  else {
                                    if(driver -> vars[$1] > $3) $$ = 1;

                                    else $$ = 0;
                                  }
 	    			 }
| VAR LESS expr                  {
				  if (driver -> vars.find($1) == driver -> vars.end()){
                                    std::cout << "Wrong variable" << std::endl;
                                    std::abort();
                                  }
                                  else {
                                    if(driver -> vars[$1] == $3) $$ = 1;

                                    else $$ = 0;
                                  }
				 }
| VAR EQUAL SCAN                 {
				  //std::cout << "buf = " << std::cin.rdbuf();
				  int a = 2;
				  //auto input = std::cin.rdbuf();
				  //std::cin.clear();
				  //std::cin.ignore(std::cin.rdbuf() -> in_avail());
				  //std::cout << "buf = " << std::cin.rdbuf();
				  std::cin >> a;
				  //int b = scanf("%d", &a);
				  //std::cout << b;
				  //std::cout << "a = " << a << std::endl;
				  if (driver -> find($1) == driver -> end()){
				    //std::cout << "Hi(6)" << std::endl;
                                    driver -> vars.insert({$1, a});
                                  }
                                  else {
				    std::cout << "a = " << a << std::endl;
                                    driver -> vars[$1] = a;
                                  }
				  
				  //std::cin.rdbuf(input);
				  //std::cout << "buf = " << std::cin.rdbuf() << std::endl;
				 }
| PRINT VAR                      {
				  if(driver -> vars.find($2) == driver -> vars.end()){
				    std::cout << "Wrong variable to print" << std::endl;
				    std::abort();
				  }
				  std::cout << driver -> vars[$2] << std::endl;
				 }
| PRINT NUMBER                   {std::cout << $2 << std::endl;}
;

expr: VAR                          { $$ = driver -> vars[$1];} 
| NUMBER                           { $$ = $1;}
| L_BRACE expr PLUS expr R_BRACE   { $$ = ($2 + $4);}
| L_BRACE expr MINUS expr R_BRACE  { $$ = ($2 - $4);}
| L_BRACE expr MULT expr R_BRACE   { $$ = ($2 * $4);}
| L_BRACE expr DIV expr R_BRACE    { $$ = ($2 / $4);}
;

cycle: WHILE L_BRACE statement R_BRACE L_FGR statelist R_FGR  
;

if: IF L_BRACE statement R_BRACE L_FGR statelist R_FGR        
;

%%


namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,
                         NumDriver* driver)
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&){}

}
