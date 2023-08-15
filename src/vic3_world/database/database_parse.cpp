#include "database_parser.h"

namespace vic3
{
DatabaseParser::DatabaseParser(commonItems::parsingFunctionStreamOnly single_element_func): parser(), db_entry_parser_()
{
   this->db_entry_parser_.registerRegex(commonItems::integerRegex,
       [single_element_func](std::string key, std::istream& stream) {
          single_element_func(stream);
       });
   const auto dbFunc = [this](std::istream& stream) {
      this->db_entry_parser_.parseStream(stream);
   };
   this->db_entry_parser_.IgnoreUnregisteredItems();
   this->registerKeyword("database", dbFunc);
   this->IgnoreAndLogUnregisteredItems();
}

void DatabaseParser::operator()(std::istream& stream)
{
   this->parseStream(stream);
}
}