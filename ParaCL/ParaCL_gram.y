%language "c++"

%skeleton "lalr1.cc"

%code requires
{
#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "SyntaxTree.hh"

namespace yy {
class Driver; 
}

/*#define YYLLOC_DEFAULT(Cur, Rhs, N)			\
  if (N) {							\
    (Cur)=YYRHSLOC(Rhs,1);				\
  } 							\
  else {							\
    (Cur)=YYRHSLOC(Rhs,0);				\
  }*/

}


%code
{

#include "ParaCL.hpp"
namespace yy {

parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* yylloc,
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
  AND     "&&"
  OR      "||"
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
                                  //std::cout << "@1.first_line = " << @1.first_line << std::endl;
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
				 }
| PRINT expr                     {
				  //std::cout << "PRINT VAR\n";
				  $$ = new SyntaxNodeBin;
                                  $$ -> type = NodeTreeTypes::Types::EQUAL;
                                  $$ -> SetLeft(new SyntaxNodeStream);
                                  $$ -> GetLeft() -> type = NodeTreeTypes::Types::PRINT;
                                  $$ -> SetRight($2);
				 }
;

expr: mult PLUS expr               { //std::cout << "PLUS\n";
    				     $$ = new SyntaxNodeBin;
				     $$ -> type = NodeTreeTypes::Types::PLUS;
                                     $$ -> SetLeft($1);
				     $$ -> SetRight($3);
				   } 
| mult MINUS expr                  { //std::cout << "MINUS\n";
                                     $$ = new SyntaxNodeBin;
                                     $$ -> type = NodeTreeTypes::Types::MINUS;
                                     $$ -> SetLeft($1);
				     $$ -> SetRight($3);
                                   }
| mult                             { //std::cout << "mult\n";
				     $$ = $1;
		                   }
; 

mult: term MULT mult               { //std::cout << "MULT\n";
    				     $$ = new SyntaxNodeBin;
                                     $$ -> type = NodeTreeTypes::Types::MULT;
                                     $$ -> SetLeft($1);
				     $$ -> SetRight($3);
			           }
| term DIV mult			   { //std::cout << "DIV\n";
                                     $$ = new SyntaxNodeBin;
	                             $$ -> type = NodeTreeTypes::Types::DIV;
                                     $$ -> SetLeft($1);
                                     $$ -> SetRight($3);
 	                           }
| term                             { //std::cout << "term\n";
                                     $$ = $1;
                                   }
;

term: L_BRACE expr R_BRACE         { //std::cout << "(expr)\n";
                                     $$ = $2;
                                   }
| MINUS L_BRACE expr R_BRACE       {
                                     $$ = new SyntaxNodeUno;
                                     $$ -> type = NodeTreeTypes::Types::UMINUS;
                                     $$ -> SetLeft($3); 
                                   }
| NUMBER                           { //std::cout << "term:NUM\n";
                                     std::cout << "@1 = " << @1 << std::endl;
                                     $$ = new SyntaxNodeNum;
                                     $$ -> type = NodeTreeTypes::Types::NUM;
                                     $$ -> SetData($1);
                                   }
| VAR                              { //std::cout << "term:VAR\n";
                                     std::cout << "@1 = " << @1 << std::endl;
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
| L_BRACE expr AND expr R_BRACE         { //std::cout << "AND\n";
                                          $$ = new SyntaxNodeBin;
                                          $$ -> type = NodeTreeTypes::Types::AND;
                                          $$ -> SetLeft($2);
                                          $$ -> SetRight($4);
                                        }
| L_BRACE expr OR expr R_BRACE          { //std::cout << "OR\n";
                                          $$ = new SyntaxNodeBin;
                                          $$ ->  type = NodeTreeTypes::Types::OR;
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

parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* yylloc, 
                         Driver* driver)
{
  return driver->yylex(yylval, yylloc);
}

void parser::error(const location_type&, const std::string&){}

}
