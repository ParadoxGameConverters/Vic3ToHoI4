#include "external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
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

   std::vector<std::pair<std::string, std::string>> dbElements;
   commonItems::parser dbElementParser;
   dbElementParser.registerKeyword("key1", [&](std::istream& stream) {
      dbElements.emplace_back("key1", commonItems::getString(stream));
   });
   dbElementParser.registerKeyword("key2", [&](std::istream& stream) {
      dbElements.emplace_back("key2", commonItems::getString(stream));
   });

   DatabaseParser(dbElementParser).parseStream(input);

   EXPECT_THAT(dbElements,
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

   std::vector<std::pair<std::string, std::string>> dbElements;
   commonItems::parser dbElementParser;
   dbElementParser.registerKeyword("key1", [&](std::istream& stream) {
      dbElements.emplace_back("key1", commonItems::getString(stream));
   });
   dbElementParser.registerKeyword("key2", [&](std::istream& stream) {
      dbElements.emplace_back("key2", commonItems::getString(stream));
   });

   DatabaseParser(dbElementParser).parseStream(input);

   EXPECT_THAT(dbElements,
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

   std::vector<std::pair<std::string, std::string>> dbElements;
   std::string currentKey;
   commonItems::parser dbElementParser;
   dbElementParser.registerKeyword("key1", [&](std::istream& stream) {
      dbElements.emplace_back(currentKey, commonItems::getString(stream));
   });
   dbElementParser.registerKeyword("key2", [&](std::istream& stream) {
      dbElements.emplace_back(currentKey, commonItems::getString(stream));
   });
   const auto parseFunc = [&](const std::string& key, std::istream& stream) {
      currentKey = key;
      dbElementParser.parseStream(stream);
   };

   DatabaseParser parser(parseFunc);

   parser.parseStream(input);

   EXPECT_THAT(dbElements,
       testing::ElementsAre(testing::Pair("key1", "value1"),
           testing::Pair("key1", "value4"),
           testing::Pair("key2", "value2")));
}

}  // namespace vic3