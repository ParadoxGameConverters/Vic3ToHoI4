#include <external/commonItems/external/googletest/googlemock/include/gmock/gmock-matchers.h>
#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include <map>
#include <sstream>
#include <vector>

#include "src/hoi4_world/military/division_template.h"
#include "src/hoi4_world/military/division_templates_importer.h"



namespace hoi4
{

TEST(Hoi4worldMilitaryDivisiontemplateimporterTests, DivisionTemplatesCanBeImported)  // NOLINT(cert-err58-cpp)
{
   const std::vector<DivisionTemplate> templates =
       ImportDivisionTemplates("test_files/hoi4_world/military/DivisionTemplatesCanBeImported.txt");

   UnitCount battalions = {{"test", 2}};
   UnitCount supports = {{"other", 1}};
   DivisionTemplate expect("Test Division", battalions, supports);
   EXPECT_THAT(templates, testing::UnorderedElementsAre(expect));
}

}  // namespace hoi4
