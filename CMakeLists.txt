cmake_minimum_required(VERSION 3.25)
set(VERBOSE true)

project(Vic3ToHoi4)
set(FRONTER_SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/Fronter/Fronter/Source)
set(FRONTER_COMMONS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/Fronter/commonItems)
set(FRONTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}-${PLATFORM})
set(CONVERTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}-${PLATFORM}/Vic3ToHoi4)
set(TEST_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/test/${CMAKE_BUILD_TYPE}-${PLATFORM})

set(CMAKE_CXX_STANDARD 23)
set (UNICODE_DEFAULT OFF)

if (PLATFORM STREQUAL "Windows")
	ADD_DEFINITIONS(-DUNICODE -D_UNICODE -DWINDOWS)
elseif (PLATFORM STREQUAL "Linux")
	if (COVERAGE STREQUAL true)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
		set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
	else (COVERAGE STREQUAL true)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall")
	endif (COVERAGE STREQUAL true)
endif (PLATFORM STREQUAL "Windows")


#### Converter ####

include_directories(.)
include_directories("./external/fmt/include")
include_directories("./external/commonItems")

set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/bitmap/bitmap_image.hpp")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/BulkParser.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/Color.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/CommonFunctions.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/CommonRegexes.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/ConvenientParser.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/ConverterVersion.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/Date.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/GameVersion.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/Localization/LocalizationBlock.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/Localization/LocalizationDatabase.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/Localization/LocalizationLambdas.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/Log.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/ModLoader/Mod.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/ModLoader/ModFilesystem.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/ModLoader/ModLoader.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/ModLoader/ModParser.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/OSCompatibilityLayer.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/Parser.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/ParserHelpers.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/StringUtils.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/external/commonItems/targa.h")

set(MAIN_SOURCE "src/main.cpp")

set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/configuration/configuration_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/characters/hoi4_character_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/characters/hoi4_characters_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/countries/hoi4_countries_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/countries/hoi4_country.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/countries/hoi4_country_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/decisions/decision.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/decisions/decisions_category_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/decisions/decision_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/diplomacy/hoi4_war_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/events/event_option_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/focus_trees/focus.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/focus_trees/focus_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/focus_trees/focus_tree_assembler.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/localizations/localizations_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/map/buildings_creator.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/map/coastal_provinces_creator.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/map/hoi4_building.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/map/hoi4_province_definition_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/map/railways_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/map/resources_map_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/map/strategic_region_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/map/strategic_regions.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/map/strategic_regions_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/military/convoy_distributor.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/military/convoy_distributor_builder.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/military/division_templates_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/military/equipment_variants_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/military/task_force.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/military/task_force_template.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/military/task_force_templates_builder.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/roles/repeat_focus_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/roles/role_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/roles/roles_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/roles/stories_creator.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/states/default_states_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/states/hoi4_state.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/states/hoi4_states_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/states/state_categories.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/technology/technologies.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/technology/technologies_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/world/hoi4_world_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/world/hoi4_world_framework_builder.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/character/character_trait_mapper.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/character/character_trait_mapper_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/character/leader_type_mapper.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/character/leader_type_mapper_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/country/country_mapper.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/country/country_mapper_creator.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/culture/culture_graphics_mapper.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/culture/culture_graphics_mapper_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/culture/culture_graphics_mapping.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/flags/flag_mapper.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/flags/flag_mapper_builder.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/ideology/ideology_mapper.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/ideology/ideology_mapper_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/infrastructure/infrastructure_mapper.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/provinces/province_mapper.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/provinces/province_mapper_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/provinces/province_mapping_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/technology/tech_mapping_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/technology/tech_mappings_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/resources/resource_mapper.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/resources/resource_mapper_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/unit/unit_mapper.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/unit/unit_mapper_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/world/world_mapper_builder.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/maps/map_data.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/maps/map_data_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/maps/province_definitions.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/maps/province_points.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/maps/utility.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/characters/out_character.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/countries/out_countries.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/countries/out_country.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/decisions/out_decision.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/decisions/out_decisions.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/decisions/out_decisions_categories.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/decisions/out_decisions_category.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/diplomacy/out_war.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/focus_trees/out_focus.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/focus_trees/out_focus_tree.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/flags/out_flags.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/ideas/out_ideas.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/localizations/out_localizations.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/map/out_building.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/map/out_buildings.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/map/out_railway.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/map/out_railways.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/map/out_strategic_region.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/map/out_strategic_regions.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/map/out_supply_nodes.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/military/out_equipment_variant.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/names/out_name_list.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/names/out_names.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/out_mod.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/portraits/out_portrait_paths.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/portraits/out_portraits.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/states/out_state.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/states/out_states.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/technology/out_technologies.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/world/out_world.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/support/converter_utils.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/support/progress_manager.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_to_hoi4_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/buildings/building_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/buildings/buildings.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/buildings/buildings_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/characters/vic3_character_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/characters/vic3_character_manager.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/characters/vic3_characters_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/characters/vic3_country_character_map_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/countries/country_definitions_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/countries/vic3_countries_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/countries/vic3_country.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/countries/vic3_country_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/country_rankings/country_rankings_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/cultures/culture_definitions_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/cultures/cultures_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/database/database_parser.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/elections/elections_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/ideologies/ideologies.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/ideologies/ideologies_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/ideologies/ideology_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/institutions/institutions_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/interest_groups/interest_group_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/interest_groups/interest_groups_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/laws/laws_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/military/military_formations_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/military/military_formation_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/pacts/pact_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/pacts/pacts_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/provinces/vic3_province_definitions.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/provinces/vic3_province_definitions_loader.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/states/state_regions_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/states/vic3_state_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/states/vic3_states_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/technology/vic3_technology_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/wars/wars_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/wars/war_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/world/vic3_world_builder.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/world/vic3_world_importer.cpp")

set(FORMAT_SOURCES ${FORMAT_SOURCES} "external/fmt/src/format.cc")
set(FORMAT_SOURCES ${FORMAT_SOURCES} "external/fmt/src/os.cc")
add_library(fmtLib ${FORMAT_SOURCES})

link_directories("./external/rakaly")

add_subdirectory(external/commonItems EXCLUDE_FROM_ALL)

add_library(Vic3ToHoi4lib
	${CONVERTER_SOURCES}
)
target_precompile_headers(Vic3ToHoi4lib PUBLIC ${PCH_SOURCES})
if (PLATFORM STREQUAL "Windows")
	target_compile_options(Vic3ToHoi4lib PRIVATE /permissive /W4 /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 
        /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928 
        /external:anglebrackets /external:W0 /WX)
	target_link_libraries(Vic3ToHoi4lib LINK_PUBLIC commonLib fmtLib rakaly.dll.lib)
elseif (PLATFORM STREQUAL "Linux")
	target_link_libraries(Vic3ToHoi4lib LINK_PUBLIC commonLib fmtLib rakaly)
endif (PLATFORM  STREQUAL "Windows")

add_executable(
	Vic3ToHoi4
	${MAIN_SOURCE}
 )
set_target_properties( Vic3ToHoi4
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CONVERTER_OUTPUT_DIRECTORY}
)
target_precompile_headers(Vic3ToHoi4 REUSE_FROM Vic3ToHoi4lib)
target_link_libraries(Vic3ToHoi4 LINK_PUBLIC Vic3ToHoi4lib)
if (PLATFORM STREQUAL "Windows")
	target_compile_options(Vic3ToHoi4 PRIVATE /permissive /W4 /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 
        /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928 
        /external:anglebrackets /external:W0 /WX)
	target_link_libraries(Vic3ToHoi4 LINK_PUBLIC commonLib rakaly.dll.lib)
elseif (PLATFORM STREQUAL "Linux")
	target_link_libraries(Vic3ToHoi4 LINK_PUBLIC commonLib rakaly)
endif (PLATFORM  STREQUAL "Windows")

configure_file("data/version.txt" "${FRONTER_OUTPUT_DIRECTORY}/version.txt" COPYONLY)
configure_file("data/readme.txt" "${FRONTER_OUTPUT_DIRECTORY}/readme.txt" COPYONLY)
file(COPY "data/blank_mod" DESTINATION "${CONVERTER_OUTPUT_DIRECTORY}")
file(COPY "data/configurables" DESTINATION "${CONVERTER_OUTPUT_DIRECTORY}")
if (PLATFORM STREQUAL "Windows")
	configure_file("external/rakaly/rakaly.dll" "${FRONTER_OUTPUT_DIRECTORY}/Vic3toHoi4/rakaly.dll" COPYONLY)
endif (PLATFORM STREQUAL "Windows")
add_custom_command(TARGET Vic3ToHoi4 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND git rev-parse HEAD > ${CONVERTER_OUTPUT_DIRECTORY}/../commit_id.txt)

#### Fronter ####

if (PLATFORM STREQUAL "Windows")
	link_directories("./external/Fronter/external/wxWidgets/3.2.1/lib/vc14x_x64_dll")
	link_directories("./external/Fronter/external/curl/lib")
	find_library(LIBCURL_LIB libcurl HINT "./external/Fronter/external/curl/lib" NO_DEFAULT_PATH)
	find_library(LIBCURL_DLL_LIB libcurl.dll HINT "./external/Fronter/external/curl/lib" NO_DEFAULT_PATH)
endif (PLATFORM STREQUAL "Windows")

include_directories("./external/Fronter/external/SQLiteCpp/include")
include_directories("./external/Fronter/external/SQLiteCpp/sqlite3")
include_directories("./external/Fronter/external/commonItems/external/json/single_include")
if (PLATFORM STREQUAL "Windows")
	include_directories(${PROJECT_SOURCE_DIR}/external/Fronter/external/curl/include)
	include_directories(${PROJECT_SOURCE_DIR}/external/Fronter/external/wxWidgets/3.2.1/include)
	include_directories(${PROJECT_SOURCE_DIR}/external/Fronter/external/wxWidgets/3.2.1/include/msvc)
	ADD_DEFINITIONS(-D_WINDOWS)
	ADD_DEFINITIONS(-D__WXMSW__)
	ADD_DEFINITIONS(-DwxMSVC_VERSION_AUTO)
	ADD_DEFINITIONS(-DWXUSINGDLL)
	ADD_DEFINITIONS(-DwxMSVC_VERSION_ABI_COMPAT)
elseif (PLATFORM STREQUAL "Linux")
	find_package(CURL REQUIRED)
	find_package(wxWidgets REQUIRED COMPONENTS net core base)
	include_directories("/usr/include/wx-3.2-unofficial")
	include_directories("/usr/lib/x86_64-linux-gnu/wx/include/gtk3-unicode-3.2-unofficial")
	include(${wxWidgets_USE_FILE})
	add_compile_options(-D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread)
endif (PLATFORM STREQUAL "Windows")

file(GLOB F_MAIN_SOURCES "${FRONTER_SOURCE_DIR}/*.cpp")
file(GLOB F_CONFIGURATION_SOURCES "${FRONTER_SOURCE_DIR}/Configuration/*.cpp")
file(GLOB F_CONFIGURATION_OPTIONS_SOURCES "${FRONTER_SOURCE_DIR}/Configuration/Options/*.cpp")
file(GLOB F_FRAMES_SOURCES "${FRONTER_SOURCE_DIR}/Frames/*.cpp")
file(GLOB F_FRAMES_TABS_SOURCES "${FRONTER_SOURCE_DIR}/Frames/Tabs/*.cpp")
file(GLOB F_UPDATECHECKER_SOURCES "${FRONTER_SOURCE_DIR}/UpdateChecker/*.cpp")
file(GLOB F_UTILS_SOURCES "${FRONTER_SOURCE_DIR}/Utils/*.cpp")
file(GLOB F_UTILS_LOCALIZATION_SOURCES "${FRONTER_SOURCE_DIR}/Utils/Localization/*.cpp")
file(GLOB F_CONVERTERLAUNCHER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/ConverterLauncher/*.cpp")
if (PLATFORM STREQUAL "Windows")
	list(REMOVE_ITEM F_CONVERTERLAUNCHER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/ConverterLauncher/LinuxConverterLauncher.cpp")
elseif (PLATFORM STREQUAL "Linux")
	list(REMOVE_ITEM F_CONVERTERLAUNCHER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/ConverterLauncher/WinConverterLauncher.cpp")
endif (PLATFORM STREQUAL "Windows")
file(GLOB F_LOGWATCHER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/LogWatcher/*.cpp")
file(GLOB F_MODCOPIER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/ModCopier/*.cpp")
file(GLOB F_SQLITE_SOURCES "${FRONTER_SOURCE_DIR}/../../external/SQLiteCpp/src/*.cpp")
file(GLOB F_SQLITE3_SOURCES "${FRONTER_SOURCE_DIR}/../../external/SQLiteCpp/sqlite3/*.c")

if (PLATFORM STREQUAL "Windows")
	add_executable(ConverterFrontend WIN32
		${F_MAIN_SOURCES}
		${F_CONFIGURATION_SOURCES}
		${F_CONFIGURATION_OPTIONS_SOURCES}
		${F_FRAMES_SOURCES}
		${F_FRAMES_TABS_SOURCES}
		${F_UTILS_SOURCES}
		${F_UTILS_LOCALIZATION_SOURCES}
		${F_CONVERTERLAUNCHER_SOURCES}
		${F_LOGWATCHER_SOURCES}
		${F_MODCOPIER_SOURCES}
		${F_UPDATECHECKER_SOURCES}
		${F_SQLITE_SOURCES}
		${F_SQLITE3_SOURCES}
	 )
elseif (PLATFORM STREQUAL "Linux")
	add_executable(ConverterFrontend
		${F_MAIN_SOURCES}
		${F_CONFIGURATION_SOURCES}
		${F_CONFIGURATION_OPTIONS_SOURCES}
		${F_FRAMES_SOURCES}
		${F_FRAMES_TABS_SOURCES}
		${F_UTILS_SOURCES}
		${F_UTILS_LOCALIZATION_SOURCES}
		${F_CONVERTERLAUNCHER_SOURCES}
		${F_LOGWATCHER_SOURCES}
		${F_MODCOPIER_SOURCES}
		${F_UPDATECHECKER_SOURCES}
		${F_SQLITE_SOURCES}
		${F_SQLITE3_SOURCES}
	)
endif (PLATFORM STREQUAL "Windows")

set_target_properties(ConverterFrontend
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${FRONTER_OUTPUT_DIRECTORY}
)
if (PLATFORM STREQUAL "Windows")
	target_link_libraries(ConverterFrontend LINK_PUBLIC ${LIBCURL_LIB} ${LIBCURL_DLL_LIB} commonLib) 
elseif (PLATFORM STREQUAL "Linux")
	target_link_libraries(ConverterFrontend LINK_PUBLIC ${wxWidgets_LIBRARIES} ${CURL_LIBRARIES} commonLib)
endif (PLATFORM STREQUAL "Windows")

if (PLATFORM STREQUAL "Windows")
	configure_file("external/Fronter/Fronter/Resources/converter.ico" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
	configure_file("external/Fronter/Fronter/Resources/GeneralFAQ-READ.ME.FIRST.txt" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
	configure_file("external/Fronter/Fronter/Resources/libcrypto-1_1-x64.dll" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
	configure_file("external/Fronter/Fronter/Resources/libssl-1_1-x64.dll" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
	configure_file("external/Fronter/external/curl/bin/libcurl-x64.dll" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	configure_file("external/Fronter/external/wxWidgets/3.2.1/lib/vc14x_x64_dll/wxbase32u_vc14x_x64.dll" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
	configure_file("external/Fronter/external/wxWidgets/3.2.1/lib/vc14x_x64_dll/wxmsw32u_core_vc14x_x64.dll" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
	configure_file("external/Fronter/external/wxWidgets/3.2.1/lib/vc14x_x64_dll/wxbase32ud_vc14x_x64.dll" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
	configure_file("external/Fronter/external/wxWidgets/3.2.1/lib/vc14x_x64_dll/wxmsw32ud_core_vc14x_x64.dll" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
endif (CMAKE_BUILD_TYPE STREQUAL "Release")
	file(GLOB FRONTER_CONFIG "data/fronter/*")
	file(COPY ${FRONTER_CONFIG} DESTINATION "${FRONTER_OUTPUT_DIRECTORY}/Configuration/")
	file(GLOB LOCALIZATIONS "external/Fronter/Fronter/Resources/*.yml")
	file(COPY ${LOCALIZATIONS} DESTINATION "${FRONTER_OUTPUT_DIRECTORY}/Configuration/")
elseif (PLATFORM STREQUAL "Linux")
	add_custom_command(TARGET ConverterFrontend
		COMMAND mkdir -p "${FRONTER_OUTPUT_DIRECTORY}/Configuration/" &&
		cp -r "data/fronter/*" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/" &&
		cp -r "external/Fronter/Fronter/Resources/*yml" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/" &&
		cp external/Fronter/Fronter/Resources/converter.ico ${FRONTER_OUTPUT_DIRECTORY} &&
		cp external/Fronter/Fronter/Resources/GeneralFAQ-READ.ME.FIRST.txt ${FRONTER_OUTPUT_DIRECTORY}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
endif (PLATFORM STREQUAL "Windows")

#### Tests ####

include_directories("./external/commonItems/external/googletest/googletest")
include_directories("./external/commonItems/external/googletest/googletest/include")
include_directories("./external/commonItems/external/googletest/googlemock")
include_directories("./external/commonItems/external/googletest/googlemock/include")

set(TEST_SOURCES ${TEST_SOURCES} "src/configuration/configuration_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/characters/hoi4_characters_converter_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/characters/hoi4_character_converter_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/characters/hoi4_character_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/countries/hoi4_countries_converter_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/countries/hoi4_country_converter_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/countries/hoi4_country_converter_tests_tech.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/decisions/decisions_category_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/decisions/decision_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/decisions/decision_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/diplomacy/hoi4_war_converter_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/events/event_option_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/focus_trees/focus_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/focus_trees/focus_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/focus_trees/focus_tree_assembler_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/localizations/localizations_converter_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/map/buildings_creator_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/map/coastal_provinces_creator_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/map/hoi4_province_definition_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/map/possible_path_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/map/railways_converter_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/map/railway_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/map/resources_map_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/map/strategic_regions_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/map/strategic_regions_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/map/strategic_region_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/map/strategic_region_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/military/convoy_distributor_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/military/convoy_distributor_builder_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/military/division_templates_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/military/equipment_variants_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/military/task_force_template_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/military/task_force_templates_builder_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/roles/repeat_focus_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/roles/role_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/roles/roles_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/roles/stories_creator_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/states/default_states_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/states/hoi4_states_converter_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/states/hoi4_states_converter_tests_state_contents.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/states/hoi4_state_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/states/state_categories_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/technology/technologies_converter_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/technology/technologies_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/world/hoi4_world_converter_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/world/hoi4_world_framework_builder_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/character/character_trait_mapper_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/character/character_trait_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/character/leader_type_mapper_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/character/leader_type_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/country/country_mapper_creator_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/country/country_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/culture/culture_graphics_mapper_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/culture/culture_graphics_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/flags/flag_mapper_builder_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/flags/flag_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/ideology/ideology_mapper_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/ideology/ideology_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/infrastructure/infrastructure_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/provinces/province_mapper_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/provinces/province_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/provinces/province_mapping_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/technology/tech_mappings_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/technology/tech_mapping_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/resources/resource_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/resources/resource_mapper_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/unit/unit_mapper_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/unit/unit_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/world/world_mapper_builder_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/maps/map_data_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/maps/province_definitions_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/maps/province_points_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/maps/utility_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/characters/out_character_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/countries/out_countries_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/countries/out_country_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/decisions/out_decision_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/decisions/out_decisions_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/decisions/out_decisions_categories_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/decisions/out_decisions_category_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/diplomacy/out_war_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/flags/out_flags_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/focus_trees/out_focus_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/focus_trees/out_focus_tree_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/localizations/out_localizations_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/map/out_buildings_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/map/out_building_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/map/out_railways_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/map/out_railway_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/map/out_strategic_regions_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/map/out_strategic_region_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/map/out_supply_nodes_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/military/out_equipment_variant_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/names/out_names_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/names/out_name_list_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/names/out_names_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/out_mod_test.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/portraits/out_portraits_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/portraits/out_portrait_paths_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/states/out_states_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/states/out_state_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/technology/out_technologies_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/world/out_world_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/buildings/buildings_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/buildings/buildings_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/buildings/building_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/characters/vic3_characters_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/characters/vic3_character_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/characters/vic3_character_manager_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/characters/vic3_country_character_map_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/countries/country_definitions_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/countries/vic3_countries_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/countries/vic3_country_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/countries/vic3_country_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/country_rankings/country_rankings_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/cultures/cultures_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/cultures/culture_definitions_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/database/database_parser_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/elections/elections_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/ideologies/ideologies_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/ideologies/ideologies_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/ideologies/ideology_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/institutions/institutions_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/interest_groups/interest_groups_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/interest_groups/interest_group_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/laws/laws_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/military/military_formations_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/military/military_formation_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/pacts/pacts_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/pacts/pact_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/pacts/pact_test.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/provinces/vic3_province_definitions_loader_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/provinces/vic3_province_definitions_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/states/state_regions_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/states/vic3_states_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/states/vic3_state_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/technology/vic3_technology_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/wars/wars_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/wars/war_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/world/vic3_world_builder_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/world/vic3_world_importer_tests.cpp")

set(GTEST_SOURCES ${GTEST_SOURCES} "external/commonItems/external/googletest/googletest/src/gtest_main.cc")
set(GTEST_SOURCES ${GTEST_SOURCES} "external/commonItems/external/googletest/googletest/src/gtest-all.cc")
set(GTEST_SOURCES ${GTEST_SOURCES} "external/commonItems/external/googletest/googlemock/src/gmock-all.cc")

add_executable(
	Vic3ToHoi4Tests
	${TEST_SOURCES}
	${GTEST_SOURCES}
 )
set_target_properties(Vic3ToHoi4Tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
)
target_precompile_headers(Vic3ToHoi4Tests REUSE_FROM Vic3ToHoi4lib)
if (PLATFORM STREQUAL "Windows")
    target_compile_options(Vic3ToHoi4Tests PRIVATE /permissive /W4 /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 
        /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928 
        /external:anglebrackets /external:W0 /WX)
elseif (PLATFORM STREQUAL "Linux")
endif (PLATFORM STREQUAL "Windows")

file(COPY "data/blank_mod" DESTINATION "${TEST_OUTPUT_DIRECTORY}")
file(COPY "data/test_files" DESTINATION "${TEST_OUTPUT_DIRECTORY}")
file(COPY "data/test_files/configurables" DESTINATION "${TEST_OUTPUT_DIRECTORY}")
if (PLATFORM STREQUAL "Windows")
	configure_file("external/rakaly/rakaly.dll" "${TEST_OUTPUT_DIRECTORY}/rakaly.dll" COPYONLY)
endif (PLATFORM STREQUAL "Windows")

target_link_libraries(Vic3ToHoi4Tests LINK_PUBLIC Vic3ToHoi4lib)

enable_testing()

add_test(NAME Vic3ToHoI4Tests COMMAND Vic3ToHoI4Tests WORKING_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

####  Coverage ####

if (COVERAGE STREQUAL true)
	add_custom_target(gcov
		COMMAND mkdir -p gcoverage
	)
	add_custom_command(TARGET gcov
		COMMAND echo "=================== GCOV ===================="
		COMMAND echo "-- Source directories:"
		COMMAND echo "		${CMAKE_SOURCE_DIR}/src/"
		COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/gcoverage"
	)

	add_custom_target(lcov
		COMMAND mkdir -p lcoverage
	)
	add_custom_command(TARGET lcov
		COMMAND echo "=================== LCOV ===================="
		COMMAND echo "-- Passing lcov tool under code coverage"
		COMMAND lcov --c --d CMakeFiles/Vic3ToHoi4lib.dir/src/ --d CMakeFiles/Vic3ToHoi4Tests.dir/src/ --o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/11/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/11/bits/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/11/ext/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/x86_64-linux-gnu/c++/11/bits/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/12/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/12/bits/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/12/ext/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/x86_64-linux-gnu/c++/12/bits/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/13/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/13/bits/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/13/ext/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/include/x86_64-linux-gnu/c++/13/bits/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/lib/gcc/x86_64-linux-gnu/12/include/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info "/usr/lib/gcc/x86_64-linux-gnu/13/include/*" -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info '*/external/bitmap/*' -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info '*/external/commonItems/*' -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info '*/external/fmt/include/fmt/*' -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info '*/external/rakaly/*' -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info '*/external/stb/*' -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info '*/gmock/*' -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info '*/gtest/*' -o lcoverage/coverage.info
		COMMAND lcov --remove lcoverage/coverage.info '*/gtest/internal/*' -o lcoverage/coverage.info
		COMMAND echo "-- Generating HTML output files"
		COMMAND genhtml lcoverage/coverage.info --output-directory lcoverage
	)
	add_dependencies(gcov Vic3ToHoi4Tests)

	# Make sure to clean up the coverage folder
	set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES gcoverage)

	# Create the gcov-clean target. This cleans the build as well as generated 
	# .gcda and .gcno files.
	add_custom_target(init
		COMMAND ${CMAKE_MAKE_PROGRAM} clean
		COMMAND rm -f ${TEST_OUTPUT_DIRECTORY}/*.gcno
		COMMAND rm -f ${TEST_OUTPUT_DIRECTORY}/*.gcda
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
endif (COVERAGE STREQUAL true)


#### Clang-tidy ####

#find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
#set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}")
# checks to maybe add:
#  cert-*
#  cppcoreguidelines-*,

add_library(run_clang_tidy
	${CONVERTER_SOURCES}
	${TEST_SOURCES}
 )
target_link_libraries(run_clang_tidy pthread)
set_target_properties(run_clang_tidy PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
add_dependencies(run_clang_tidy Vic3ToHoi4 Vic3ToHoi4Tests)
