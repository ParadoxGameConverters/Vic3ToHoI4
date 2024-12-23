#include "src/vic3_world/country_rankings/country_rankings_importer.h"

#include <external/commonItems/Parser.h>
#include <external/commonItems/ParserHelpers.h>

#include <istream>
#include <sstream>



vic3::CountryRankings vic3::ImportCountryRankings(std::istream& input_stream)
{
   std::set<int> great_powers;
   std::set<int> major_powers;
   std::map<int, int> scored_countries;

   std::string rank;
   std::optional<int> country_number;
   int score = 0;

   commonItems::parser ranking_parser;
   ranking_parser.registerKeyword("rank", [&rank](std::istream& input_stream) {
      rank = commonItems::getString(input_stream);
   });
   ranking_parser.registerKeyword("country", [&country_number](std::istream& input_stream) {
      country_number = commonItems::getInt(input_stream);
   });
   ranking_parser.registerKeyword("score", [&score](std::istream& input_stream) {
      score = commonItems::getInt(input_stream);
   });
   ranking_parser.IgnoreUnregisteredItems();

   commonItems::parser overall_parser;
   overall_parser.registerKeyword("country_rankings",
       [&rank, &country_number, &score, &ranking_parser, &great_powers, &major_powers, &scored_countries](
           std::istream& input_stream) {
          for (const std::string& ranking_blob: commonItems::blobList(input_stream).getBlobs())
          {
             rank.clear();
             country_number.reset();
             score = 0;

             std::stringstream blob_stream;
             blob_stream << ranking_blob;
             ranking_parser.parseStream(blob_stream);

             if (!country_number)
             {
                continue;
             }

             if (rank == "great_power")
             {
                great_powers.insert(*country_number);
             }
             if (rank == "major_power")
             {
                major_powers.insert(*country_number);
             }
             scored_countries.emplace(score, *country_number);
          }
       });
   overall_parser.IgnoreUnregisteredItems();

   overall_parser.parseStream(input_stream);

   return {great_powers, major_powers, scored_countries};
}