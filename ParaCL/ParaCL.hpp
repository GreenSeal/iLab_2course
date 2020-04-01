#pragma once

#include <FlexLexer.h>
#include "ParaCL_gram.tab.hh"

namespace yy {
  
class NumDriver {
  FlexLexer *plex_;
public:
  std::unordered_map<std::string, int> vars;
  
  NumDriver (FlexLexer *plex) : plex_(plex), vars() {}

  parser::token_type yylex(parser::semantic_type* yylval) {
    parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());
    if (tt == yy::parser::token_type::NUMBER)
      yylval->as<int>() = std::stoi(plex_->YYText());
    if (tt == yy::parser::token_type::VAR) {
      yylval->build<std::string>() = plex_->YYText();
    }
    return tt;
  }
  
  bool parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
  }

  auto find(std::string& str) {
    return vars.find(str);
  }

  auto end() {
    return vars.end();
  }
};  

}
