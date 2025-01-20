#pragma once

#include <external/commonItems/Parser.h>

#include <istream>



namespace vic3
{

// Many vic3 save structures use a generic 'database' element in their parsing.
// usage: in the main parser, registerKeyword("institutions",DatabaseParser(single_institution_func))
// OR you can use it like a normal parser, and call a lambda on [](stream){databaseParser.parse(stream);}
class DatabaseParser: public commonItems::parser
{
  public:
   // single_element_func is the function to run on each entry in the db
   explicit DatabaseParser(commonItems::parsingFunctionStreamOnly single_element_func);
   explicit DatabaseParser(commonItems::parsingFunction multi_element_func);
   DatabaseParser();

  private:
   commonItems::parser db_entry_parser_;
};
}  // namespace vic3