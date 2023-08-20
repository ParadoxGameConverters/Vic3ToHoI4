#pragma once

#include <istream>

#include "external/commonItems/Parser.h"

namespace vic3
{
// wrapper class until I can get this in to parser itself
class CallableParser: public commonItems::parser
{
  public:
   using commonItems::parser::parser;
   void operator()(std::istream& stream) { this->parseStream(stream); }
};

// Many vic3 save structures use a generic 'database' element in their parsing.
// usage: in the main parser, registerKeyword("institutions",DatabaseParser(single_institution_func))
// OR you can use it like a normal parser, and call a lambda on [](stream){databaseParser.parse(stream);}
class DatabaseParser: public CallableParser
{
  public:
   // single_element_func is the function to run on each entry in the db
   DatabaseParser(commonItems::parsingFunctionStreamOnly single_element_func);
   DatabaseParser();

  private:
   CallableParser db_entry_parser_;
};
}  // namespace vic3