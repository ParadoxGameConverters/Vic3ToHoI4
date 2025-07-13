#include <external/commonItems/ParserHelpers.h>
#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/vic3_world/database/database_parser.h"



namespace vic3
{

TEST(Vic3WorldDatabaseDatabaseParser, DatabaseParserForwardsElements)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tkey1 = value1";
   input << "}\n";
   input << "1 ={ \n";
   input << "\tkey1 = value4";
   input << "}\n";
   input << "2 = {\n";
   input << "\tkey2 = value3";
   input << "}\n";
   input << "3 = {\n";
   input << "\tkey2 = value2";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   std::vector<std::pair<std::string, std::string>> db_elements;
   commonItems::parser db_element_parser;
   db_element_parser.registerKeyword("key1", [&](std::istream& stream) {
      db_elements.emplace_back("key1", commonItems::getString(stream));
   });
   db_element_parser.registerKeyword("key2", [&](std::istream& stream) {
      db_elements.emplace_back("key2", commonItems::getString(stream));
   });

   DatabaseParser(db_element_parser).parseStream(input);

   EXPECT_THAT(db_elements,
       testing::ElementsAre(testing::Pair("key1", "value1"),
           testing::Pair("key1", "value4"),
           testing::Pair("key2", "value3"),
           testing::Pair("key2", "value2")));
}

TEST(Vic3WorldDatabaseDatabaseParser, DatabaseParserSkipsNoneElements)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tkey1 = value1";
   input << "}\n";
   input << "1 ={ \n";
   input << "\tkey1 = value4";
   input << "}\n";
   input << "2 =none\n";
   input << "3 = {\n";
   input << "\tkey2 = value2";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   std::vector<std::pair<std::string, std::string>> db_elements;
   commonItems::parser db_element_parser;
   db_element_parser.registerKeyword("key1", [&](std::istream& stream) {
      db_elements.emplace_back("key1", commonItems::getString(stream));
   });
   db_element_parser.registerKeyword("key2", [&](std::istream& stream) {
      db_elements.emplace_back("key2", commonItems::getString(stream));
   });

   DatabaseParser(db_element_parser).parseStream(input);

   EXPECT_THAT(db_elements,
       testing::ElementsAre(testing::Pair("key1", "value1"),
           testing::Pair("key1", "value4"),
           testing::Pair("key2", "value2")));
}

TEST(Vic3WorldDatabaseDatabaseParser, DatabaseParserCanForwardIds)
{
   std::stringstream input;
   input << "={\n";
   input << "\tdatabase={\n";
   input << "0 = {\n";
   input << "\tkey1 = value1";
   input << "}\n";
   input << "1 ={ \n";
   input << "\tkey1 = value4";
   input << "}\n";
   input << "2 =none\n";
   input << "3 = {\n";
   input << "\tkey2 = value2";
   input << "}\n";
   input << "\t}\n";
   input << "}\n";

   std::vector<std::pair<std::string, std::string>> db_elements;
   std::string current_key;
   commonItems::parser db_element_parser;
   db_element_parser.registerKeyword("key1", [&](std::istream& stream) {
      db_elements.emplace_back(current_key, commonItems::getString(stream));
   });
   db_element_parser.registerKeyword("key2", [&](std::istream& stream) {
      db_elements.emplace_back(current_key, commonItems::getString(stream));
   });
   const auto parse_func = [&](const std::string& key, std::istream& stream) {
      current_key = key;
      db_element_parser.parseStream(stream);
   };

   DatabaseParser parser(parse_func);

   parser.parseStream(input);

   EXPECT_THAT(db_elements,
       testing::ElementsAre(testing::Pair("0", "value1"), testing::Pair("1", "value4"), testing::Pair("3", "value2")));
}

}  // namespace vic3