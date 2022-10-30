#include "src/vic3_world/states/state_importer.h"
#include "external/commonItems/ParserHelpers.h"



vic3::StateImporter::StateImporter()
{
	state_parser_.registerKeyword("provinces", [this](std::istream& input_stream)
	{
		provinces_parser_.parseStream(input_stream);
	});

	provinces_parser_.registerKeyword("provinces", [this](std::istream& input_stream)
	{
		const auto provinces_input = commonItems::getInts(input_stream);
		if (provinces_input.size() % 2 != 0)
		{
			throw std::runtime_error("Provinces entry was malformed. Please go to the forum thread and upload log.txt and your save.");
		}
		for (unsigned int i = 0; i < provinces_input.size() / 2; i++)
		{
			int start_province = provinces_input[i];
			int additional_provinces = provinces_input[i + 1];
			for (int j = 0; j <= additional_provinces; j++)
			{
				provinces_.insert(start_province + j);
			}
		}
	});
}


vic3::State vic3::StateImporter::ImportState(std::istream& input_stream)
{
   provinces_.clear();
   state_parser_.parseStream(input_stream);
   return State(provinces_);
}