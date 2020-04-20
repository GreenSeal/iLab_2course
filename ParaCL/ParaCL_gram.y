%language "c++"

%skeleton "lalr1.cc"

%code requires
{
#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "SyntaxTree.hh"

namespace yy {class Driver; }
}

%code
{

#include "ParaCL.hpp"
namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,
      			 Driver* driver);

}


}

%defines
%define api.value.type variant
%param {yy::Driver* driver}

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
%nterm <ISyntaxTreeNode *> expr
%nterm <ISyntaxTreeNode *> statement
%nterm <ISyntaxTreeNode *> statelist
%nterm <ISyntaxTreeNode *> mult
%nterm <ISyntaxTreeNode *> term

%left '+' '-' '*' '/'



%start program

%%
program : statelist { 
		      //std::cout << "STATELIST\n";
		      driver -> tree.head = $1;
		    }
;

statelist : statement SCOLON statelist { 
	  				 //std::cout << "STATEMENT SCOLON STATELIST\n";
					 $$ = new SyntaxNodeBin;
	  				 $$ -> SetLeft($1);
					 $$ -> SetRight($3);
	                               }
| statement SCOLON		       {
				         //std::cout << "STATEMENT\n";	 
					 $$ = $1;
				       }
| construction statelist
| construction
;

construction: cycle
| if
;

statement: VAR      		 {
				  $$ = new SyntaxNodeBin;
				  $$ -> type = NodeTreeTypes::Types::EQUAL;
				  $$ -> SetLeft(new SyntaxNodeStr);
				  $$ -> GetLeft() -> type = NodeTreeTypes::Types::VAR;
				  $$ -> GetLeft() -> SetName($1);
				  $$ -> SetRight(new SyntaxNodeNum);
				  $$ -> GetRight() -> type = NodeTreeTypes::Types::NUM;
				  $$ -> GetRight() -> SetData(0);
	                         }
| VAR EQUAL expr        	 {
				  //std::cout << "VAR = EXPR\n"; 
				  $$ = new SyntaxNodeBin;
				  $$ -> type = NodeTreeTypes::Types::EQUAL;
				  $$ -> SetLeft(new SyntaxNodeStr);
				  $$ -> GetLeft() -> type = NodeTreeTypes::Types::VAR;
				  $$ -> GetLeft() -> SetName($1);
				  $$ -> SetRight($3); 
 				 }
/*| VAR EQUAL EQUAL expr           {
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
				 }*/
| VAR EQUAL SCAN                 {
				  //std::cout << "VAR = SCAN\n";
				  $$ = new SyntaxNodeBin;
                                  $$ -> type = NodeTreeTypes::Types::EQUAL;
                                  $$ -> SetLeft(new SyntaxNodeStr);
                                  $$ -> GetLeft() -> type = NodeTreeTypes::Types::VAR;
                                  $$ -> GetLeft() -> SetName($1);
                                  $$ -> SetRight(new SyntaxNodeStream);
                                  $$ -> GetRight() -> type = NodeTreeTypes::Types::SCAN;
				  //std::cout << "Stm create: " << $$ << std::endl;
				 }
| PRINT VAR                      {
				  //std::cout << "PRINT VAR\n";
				  $$ = new SyntaxNodeBin;
                                  $$ -> type = NodeTreeTypes::Types::EQUAL;
                                  $$ -> SetLeft(new SyntaxNodeStream);
                                  $$ -> GetLeft() -> type = NodeTreeTypes::Types::PRINT;
                                  $$ -> SetRight(new SyntaxNodeStr);
                                  $$ -> GetRight() -> type = NodeTreeTypes::Types::VAR;
                                  $$ -> GetRight() -> SetName($2);
				  //std::cout << "Stm create: " << $$ << std::endl;
				 }
| PRINT NUMBER                   {std::cout << $2 << std::endl;}
;

expr: mult PLUS expr               { //std::cout <<" expr:PLUS\n";
    				     $$ = new SyntaxNodeBin;
				     $$ -> type = NodeTreeTypes::Types::PLUS;
                                     $$ -> SetLeft($1);
				     $$ -> SetRight($3);
				   } 
| mult MINUS expr                  { //std::cout << "expr:MINUS\n";
                                     $$ = new SyntaxNodeBin;
                                     $$ -> type = NodeTreeTypes::Types::MINUS;
                                     $$ -> SetLeft($1);
				     $$ -> SetRight($3);
                                   }
| mult                             { //std::cout << "expr:mult\n";
                                     $$ = $1;
		                   }
; 

mult: term MULT mult               { //std::cout << "mult::MULT\n";
    				     $$ = new SyntaxNodeBin;
                                     $$ -> type = NodeTreeTypes::Types::MULT;
                                     $$ -> SetLeft($1);
				     $$ -> SetRight($3);
			           }
| term DIV mult			   { //std::cout << "mult:DIV\n";
                                     $$ = new SyntaxNodeBin;
	                             $$ -> type = NodeTreeTypes::Types::DIV;
                                     $$ -> SetLeft($1);
                                     $$ -> SetRight($3);
 	                           }
| term                             { //std::cout << "mult:term\n";
                                     $$ = $1;
                                   }
;

term: L_BRACE expr R_BRACE         { //std::cout << "term:expr\n";
                                     $$ = $2;
                                   }
| NUMBER                           { //std::cout << "term:NUM\n";
                                     $$ = new SyntaxNodeNum;
                                     $$ -> type = NodeTreeTypes::Types::NUM;
                                     $$ -> SetData($1);
                                   }
| VAR                              { //std::cout << "term:VAR\n";
                                     $$ = new SyntaxNodeStr;
                                     $$ -> type = NodeTreeTypes::Types::VAR;
                                     $$ -> SetName($1);
                                   }
;

cycle: WHILE L_BRACE statement R_BRACE L_FGR statelist R_FGR  
;

if: IF L_BRACE statement R_BRACE L_FGR statelist R_FGR        
;

%%


namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,
                         Driver* driver)
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&){}

}
