%option c++

%{

#include"ParaCL_gram.tab.hh"

%}

WS          [ \t\n\v]+
VARIABLE    [a-z_]+
DIGIT       [0-9]
DIGIT1      [1-9]

%%
{WS}         
"="               return yy::parser::token_type::EQUAL;
"-"               return yy::parser::token_type::MINUS;
"+"               return yy::parser::token_type::PLUS;
";"               return yy::parser::token_type::SCOLON;
"*"               return yy::parser::token_type::MULT;
"/"               return yy::parser::token_type::DIV;
"("               return yy::parser::token_type::L_BRACE;
")"               return yy::parser::token_type::R_BRACE;
">"               return yy::parser::token_type::MORE;
"<"               return yy::parser::token_type::LESS;
"{"               return yy::parser::token_type::L_FGR;
"}"               return yy::parser::token_type::R_FGR;
"while"           return yy::parser::token_type::WHILE;
"if"              return yy::parser::token_type::IF;
"?"               return yy::parser::token_type::SCAN;
"print"           return yy::parser::token_type::PRINT;
{DIGIT1}{DIGIT}*  return yy::parser::token_type::NUMBER;
{VARIABLE}        return yy::parser::token_type::VAR;
.                 return yy::parser::token_type::ERR;

%%
