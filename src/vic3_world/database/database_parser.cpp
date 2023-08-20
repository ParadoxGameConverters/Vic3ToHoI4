#include "database_parser.h"

namespace vic3
{

DatabaseParser::DatabaseParser(): CallableParser(), db_entry_parser_()
{
   this->db_entry_parser_.registerRegex(commonItems::integerRegex, [](std::string key, std::istream& stream) {
      return;
   });
   this->db_entry_parser_.IgnoreUnregisteredItems();
   this->registerKeyword("database", db_entry_parser_);
   this->IgnoreAndLogUnregisteredItems();
}

DatabaseParser::DatabaseParser(commonItems::parsingFunctionStreamOnly single_element_func):
    CallableParser(),
    db_entry_parser_()
{
   this->db_entry_parser_.registerRegex(commonItems::integerRegex,
       [single_element_func](std::string key, std::istream& stream) {
          // i have a mouth and i must scream
          char m = stream.peek();
          char n = (stream.get(), stream.peek());
          if (stream.peek() == '{')
          {
             stream.putback(m);
             single_element_func(stream);
          }
          else
          {
             commonItems::ignoreItem(key, stream);
          }
       });
   this->db_entry_parser_.IgnoreAndLogUnregisteredItems();

   this->registerKeyword("database", this->db_entry_parser_);
   this->IgnoreAndLogUnregisteredItems();
}
}  // namespace vic3