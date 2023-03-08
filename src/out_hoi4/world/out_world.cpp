#include "out_world.h"

#include "src/out_hoi4/countries/out_countries.h"
#include "src/out_hoi4/map/out_buildings.h"
#include "src/out_hoi4/map/out_rocket_sites.h"
#include "src/out_hoi4/map/out_strategic_regions.h"
#include "src/out_hoi4/states/out_states.h"
#include "external/fmt/include/fmt/format.h"
#include <fstream>
#include <ranges>



namespace
{

void OutputLocalisations(std::string_view output_name, std::string_view localization_file, const commonItems::LocalizationDatabase& localization_database)
{
    if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation", output_name)))
    {
        throw std::runtime_error(fmt::format("Could not create output/{}/localisation", output_name));
    }
    if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation/braz_por", output_name)))
    {
        throw std::runtime_error(fmt::format("Could not create output/{}/localisation/braz_por", output_name));
    }
    if (!commonItems::TryCreateFolder(fmt::format("output/{}/localisation/english", output_name)))
    {
        throw std::runtime_error(fmt::format("Could not create output/{}/localisation/english", output_name));
    }

    std::ofstream braz_por_file(fmt::format("output/{}/localisation/braz_por/{}", output_name, localization_file));
    if (!braz_por_file.is_open())
    {
        throw std::runtime_error(fmt::format("Could not create output/{}/localisation/braz_por/{}", output_name, localization_file));
    }
    braz_por_file << "l_braz_por:\n";

    std::ofstream english_file(fmt::format("output/{}/localisation/english/{}", output_name, localization_file));
    if (!english_file.is_open())
    {
        throw std::runtime_error(fmt::format("Could not create output/{}/localisation/english/{}", output_name, localization_file));
    }
    english_file << "l_english:\n";

    for (const auto& localization_block: localization_database | std::views::values)
    {
        const auto& key = localization_block.GetKey();
        english_file << fmt::format(" {}:0 \"{}\"", key, localization_block.GetLocalization("english"));
        braz_por_file << fmt::format(" {}:0 \"{}\"", key, localization_block.GetLocalization("braz_por"));
    }

    english_file.close();
    braz_por_file.close();
}

}



void out::OutputWorld(std::string_view output_name, const hoi4::World& world)
{
   OutputCountries(output_name, world.GetCountries());
   OutputStates(output_name, world.GetStates().states);
   OutputStrategicRegions(output_name, world.GetStrategicRegions());
   OutputBuildings(output_name, world.GetBuildings());
   OutputRocketSites(output_name, world.GetStates().states);
   OutputLocalisations(output_name, "test_localisations.yml", world.GetLocalizations());
}