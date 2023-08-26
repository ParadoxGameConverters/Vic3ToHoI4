#pragma once

#include <istream>
#include <vector>

#include "src/vic3_world/database/database_parser.h"
#include "src/vic3_world/institutions/institution.h"


namespace vic3
{

class InstitutionsImporter
{
  public:
   InstitutionsImporter(std::map<int, std::vector<Institution>>& out_institutions_map);
   void operator()(std::istream& input_stream);

  private:
   Institution current_institution_;
   std::map<int, std::vector<Institution>>& institutions_map_;
};
}  // namespace vic3
