#include "src/hoi4_world/military/convoy_distributor_builder.h"

#include <external/commonItems/CommonRegexes.h>
#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>

namespace hoi4
{

ConvoyDistributor BuildConvoyDistributor(const std::filesystem::path& filename)
{
   int hoi4_convoys;
   std::map<std::string, float> weights;

   commonItems::parser methods_parser;
   methods_parser.registerRegex(commonItems::catchallRegex,
       [&weights](const std::string& method, std::istream& input_stream) {
          weights[method] = static_cast<float>(commonItems::getDouble(input_stream));
       });
   commonItems::parser file_parser;
   file_parser.registerKeyword("world_convoys", [&hoi4_convoys](std::istream& input_stream) {
      hoi4_convoys = commonItems::getInt(input_stream);
   });
   file_parser.registerKeyword("method_weights", [&weights, &methods_parser](std::istream& input_stream) {
      methods_parser.parseStream(input_stream);
   });
   file_parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
   file_parser.parseFile(filename);

   ConvoyDistributor distributor(hoi4_convoys, std::move(weights));
   return distributor;
}

}  // namespace hoi4
