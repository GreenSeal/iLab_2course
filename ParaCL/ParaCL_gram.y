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
%nterm <ISyntaxTreeNode *> construction
%nterm <ISyntaxTreeNode *> cycle
%nterm <ISyntaxTreeNode *> if

%left '+' '-' '*' '/'



%start program

%%
program : statelist { 
		      //std::cout << "STATELIST\n";
		      driver -> tree = $1;
		    }
;

statelist : statement SCOLON statelist { 
	  				 //std::cout << "STATEMENT SCOLON STATELIST\n";
					 $$ = new SyntaxNodeBin;
                                         $$ -> type = NodeTreeTypes::Types::STLST;
	  				 $$ -> SetLeft($1);
					 $$ -> SetRight($3);
	                               }
| statement SCOLON		       {
				         //std::cout << "STATEMENT\n";	 
					 $$ = $1;
				       }
| construction statelist               { //std::cout << "STATEMENT CONSTR\n";
                                         $$ = new SyntaxNodeBin;
                                         $$ -> type = NodeTreeTypes::Types::STLST;
                                         $$ -> SetLeft($1);
                                         $$ -> SetRight($2);
                                       }
| construction                         {
                                         $$ = $1;
                                       }
;

construction: cycle                    {
	                                 $$ = $1;
                                       }
| if                                   {
                                         $$ = $1;
                                       }
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
;

expr: mult PLUS expr               { 
    				     $$ = new SyntaxNodeBin;
				     $$ -> type = NodeTreeTypes::Types::PLUS;
                                     $$ -> SetLeft($1);
				     $$ -> SetRight($3);
				   } 
| mult MINUS expr                  { 
                                     $$ = new SyntaxNodeBin;
                                     $$ -> type = NodeTreeTypes::Types::MINUS;
                                     $$ -> SetLeft($1);
				     $$ -> SetRight($3);
                                   }
| mult                             { 
				     $$ = $1;
		                   }
/*| MINUS mult PLUS expr               { std::cout <<" expr: " << "-" << $2 << "+" << $4 << "\n";
                                     $$ = new SyntaxNodeBin;
                                     $$ -> type = NodeTreeTypes::Types::PLUS;
                                     ISyntaxTreeNode * minus = new SyntaxNodeUno;
                                     minus -> type = NodeTreeTypes::Types::UMINUS;
                                     minus -> SetLeft($2);
                                     $$ -> SetLeft(minus);
                                     $$ -> SetRight($4);
                                   }
| MINUS mult MINUS expr            { std::cout <<" expr: " << "-" << $2 << "-" << $4 << "\n";
                                     $$ = new SyntaxNodeBin;
                                     $$ -> type = NodeTreeTypes::Types::MINUS;
                                     ISyntaxTreeNode * minus = new SyntaxNodeUno;
                                     minus -> type = NodeTreeTypes::Types::UMINUS;
                                     minus -> SetLeft($2);
                                     $$ -> SetLeft(minus);
                                     $$ -> SetRight($4);
                                   }
| MINUS mult                       { std::cout << "expr: " << "-" << $2 << "\n";
                                     $$ = new SyntaxNodeUno;
                                     $$ -> type = NodeTreeTypes::Types::UMINUS;
                                     $$ -> SetLeft($2);
                                   }*/
; 

mult: term MULT mult               { 
    				     $$ = new SyntaxNodeBin;
                                     $$ -> type = NodeTreeTypes::Types::MULT;
                                     $$ -> SetLeft($1);
				     $$ -> SetRight($3);
			           }
| term DIV mult			   { 
                                     $$ = new SyntaxNodeBin;
	                             $$ -> type = NodeTreeTypes::Types::DIV;
                                     $$ -> SetLeft($1);
                                     $$ -> SetRight($3);
 	                           }
| term                             { 
                                     $$ = $1;
                                   }
;

term: L_BRACE expr R_BRACE         { 
                                     $$ = $2;
                                   }
| NUMBER                           { 
                                     $$ = new SyntaxNodeNum;
                                     $$ -> type = NodeTreeTypes::Types::NUM;
                                     $$ -> SetData($1);
                                   }
| VAR                              { 
                                     $$ = new SyntaxNodeStr;
                                     $$ -> type = NodeTreeTypes::Types::VAR;
                                     $$ -> SetName($1);
                                   }
| L_BRACE expr EQUAL EQUAL expr R_BRACE { //std::cout << "term:DEQUAL\n";
                                          $$ = new SyntaxNodeBin;
                                          $$ -> type = NodeTreeTypes::Types::DEQUAL;
                                          $$ -> SetLeft($2);
                                          $$ -> SetRight($5);
                                        }
| L_BRACE expr MORE expr R_BRACE        { //std::cout << "term:MORE\n";
                                          $$ = new SyntaxNodeBin;
                                          $$ -> type = NodeTreeTypes::Types::MORE;
                                          $$ -> SetLeft($2);
                                          $$ -> SetRight($4);
                                        }
| L_BRACE expr LESS expr R_BRACE        { //std::cout << "trem:LESS\n";
                                          $$ = new SyntaxNodeBin;
                                          $$ -> type = NodeTreeTypes::Types::LESS;
                                          $$ -> SetLeft($2);
                                          $$ -> SetRight($4);
                                        }
;

cycle: WHILE expr L_FGR statelist R_FGR { //std::cout << "cycle\n"; 
                                          $$ = new SyntaxNodeBin;
                                          $$ -> type = NodeTreeTypes::Types::WHILE;
                                          $$ -> SetLeft($2);
                                          $$ -> SetRight($4);	 
                                        } 
;

if: IF expr L_FGR statelist R_FGR       { //std::cout << "if\n";
                                          $$ = new SyntaxNodeBin;
                                          $$ -> type = NodeTreeTypes::Types::IF;
                                          $$ -> SetLeft($2);
                                          $$ -> SetRight($4);
                                        }
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
