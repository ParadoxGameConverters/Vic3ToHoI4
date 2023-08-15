#pragma once

#include <istream>

#include "external/commonItems/Parser.h"

namespace vic3
{

// Many vic3 save structures use a generic 'database' element in their parsing.
// usage: in the main parser, registerKeyword("institutions",DatabaseParser(single_institution_func))
// OR you can use it like a normal parser, and call a lambda on [](stream){databaseParser.parse(stream);}
class DatabaseParser: public commonItems::parser
{
  public:
      // single_element_func is the function to run on each entry in the db
   DatabaseParser(commonItems::parsingFunctionStreamOnly single_element_func);
   DatabaseParser();
   void operator()(std::istream& stream);

  private:
   commonItems::parser db_entry_parser_;
};
}  // namespace vic3