#pragma once

#include <FlexLexer.h>
#include "ParaCL_gram.tab.hh"

namespace yy {
  
class Driver {
  FlexLexer *plex_;

public:

  ISyntaxTreeNode * tree;
  std::vector<std::unordered_map<std::string, int>> vars;
  
  Driver (FlexLexer *plex) : plex_(plex), vars() {}

  parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* yylloc) {
    parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());
    if (tt == yy::parser::token_type::NUMBER)
      yylval->as<int>() = std::stoi(plex_->YYText());
    if (tt == yy::parser::token_type::VAR) 
      yylval->build<std::string>() = plex_->YYText();
    if(tt == yy::parser::token_type::ERR);
      
    return tt;
  }

  void DeleteTree(ISyntaxTreeNode * node) {
    if((node -> GetLeft()) != NULL) {
      DeleteTree(node -> GetLeft());
    }

    if((node -> GetRight()) != NULL) {
      DeleteTree(node -> GetRight());
    } 

    delete node;
  }
  
  bool parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
  }
};  

}
