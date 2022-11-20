cmake_minimum_required(VERSION 3.16)
#set(CMAKE_VERBOSE_MAKEFILE on)

project(Vic3ToHoi4)
set(FRONTER_SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/Fronter/Fronter/Source)
set(FRONTER_COMMONS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/Fronter/commonItems)
set(FRONTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/Release-Linux)
set(CONVERTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/Release-Linux/Vic3ToHoi4)
set(TEST_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/test/Release-Linux)
set(CMAKE_CXX_STANDARD 20)
set (UNICODE_DEFAULT OFF)
#if (DEFINED ENV{COVERAGE})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -fprofile-arcs -ftest-coverage")
	set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
#else (DEFINED ENV{COVERAGE})
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall")
#endif (DEFINED ENV{COVERAGE})

include_directories(.)
include_directories("./external/fmt/include")
include_directories("./external/googletest/googletest")
include_directories("./external/googletest/googletest/include")
include_directories("./external/googletest/googlemock")
include_directories("./external/googletest/googlemock/include")

add_subdirectory(external/commonItems/ZipLib [binary_dir])
link_directories(external/rakaly)


#### Converter ####

set(MAIN_SOURCE "src/main.cpp")

set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_to_hoi4_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/configuration/configuration_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/countries/hoi4_country.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/states/hoi4_states_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/hoi4_world/world/hoi4_world.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/provinces/province_mapper.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/provinces/province_mapper_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/mappers/provinces/province_mapping_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/out_mod.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/countries/out_countries.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/countries/out_country.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/states/out_state.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/states/out_states.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/world/out_world.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/countries/vic3_countries_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/countries/vic3_country_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/provinces/vic3_province_definitions.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/provinces/vic3_province_definitions_loader.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/states/vic3_state_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/states/vic3_states_importer.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_world/world/vic3_world_importer.cpp")

set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/CommonFunctions.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/ConvenientParser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/ConverterVersion.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/GameVersion.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/iconvlite.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/Log.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/OSCommonLayer.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/Parser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/ParserHelpers.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/StringUtils.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/LinuxUtils.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/ModLoader/ModFilesystem.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/ModLoader/ModLoader.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/ModLoader/ModParser.cpp")

set(FORMAT_SOURCES ${FORMAT_SOURCES} "external/fmt/src/format.cc")
set(FORMAT_SOURCES ${FORMAT_SOURCES} "external/fmt/src/os.cc")

add_executable(
	Vic3ToHoi4
	${MAIN_SOURCE}
	${CONVERTER_SOURCES}
	${COMMON_SOURCES}
	${FORMAT_SOURCES}
)
set_target_properties( Vic3ToHoi4
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CONVERTER_OUTPUT_DIRECTORY}
)

target_link_libraries(Vic3ToHoi4 ZIPLIB rakaly)

configure_file("data/version.txt" "${FRONTER_OUTPUT_DIRECTORY}/version.txt" COPYONLY)
file(COPY "data/blank_mod" DESTINATION "${CONVERTER_OUTPUT_DIRECTORY}")
file(COPY "data/configurables" DESTINATION "${CONVERTER_OUTPUT_DIRECTORY}")
add_custom_command(TARGET Vic3ToHoi4 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND git rev-parse HEAD > ${CONVERTER_OUTPUT_DIRECTORY}/../commit_id.txt)


#### Fronter ####

add_custom_target(Fronter
	COMMAND tools/build_fronter.sh
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_custom_command(TARGET Fronter
	COMMAND cp -r "data/fronter/" "${FRONTER_OUTPUT_DIRECTORY}/Configuration"
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)


#### Tests ####

set(TEST_SOURCES ${TEST_SOURCES} "src/configuration/configuration_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/countries/hoi4_country_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/states/hoi4_states_converter_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/hoi4_world/world/hoi4_world_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/country/country_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/provinces/province_mapper_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/provinces/province_mapper_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/mappers/provinces/province_mapping_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/out_mod_test.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/countries/out_countries_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/countries/out_country_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/states/out_state_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/states/out_states_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/world/out_world_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/countries/vic3_countries_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/countries/vic3_country_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/provinces/vic3_province_definitions_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/provinces/vic3_province_definitions_loader_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/states/vic3_state_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/states/vic3_states_importer_tests.cpp")
set(TEST_SOURCES ${TEST_SOURCES} "src/vic3_world/world/vic3_world_importer_tests.cpp")

set(GTEST_SOURCES ${GTEST_SOURCES} "external/googletest/googletest/src/gtest_main.cc")
set(GTEST_SOURCES ${GTEST_SOURCES} "external/googletest/googletest/src/gtest-all.cc")
set(GTEST_SOURCES ${GTEST_SOURCES} "external/googletest/googlemock/src/gmock-all.cc")

add_executable(
	Vic3ToHoi4Tests
	${TEST_SOURCES}
	${GTEST_SOURCES}
	${CONVERTER_SOURCES}
	${COMMON_SOURCES}
	${FORMAT_SOURCES}
)
set_target_properties( Vic3ToHoi4Tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
)

file(COPY "data/blank_mod" DESTINATION "${TEST_OUTPUT_DIRECTORY}")
file(COPY "data/test_files" DESTINATION "${TEST_OUTPUT_DIRECTORY}")
file(COPY "data/test_files/configurables" DESTINATION "${TEST_OUTPUT_DIRECTORY}")

target_link_libraries(Vic3ToHoi4Tests ZIPLIB pthread rakaly)


####  Coverage ####

add_custom_target(gcov
	COMMAND mkdir -p gcoverage
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_custom_command(TARGET gcov
	COMMAND echo "=================== GCOV ===================="
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/vic3_to_hoi4_converter.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/configuration/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/configuration
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/hoi4_world/countries/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/hoi4_world/countries
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/hoi4_world/world/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/hoi4_world/world
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/mappers/country/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/mappers/country
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/mappers/provinces/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/mappers/provinces
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/out_hoi4/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/out_hoi4
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/out_hoi4/countries/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/out_hoi4/countries
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/out_hoi4/states/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/out_hoi4/states
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/vic3_world/countries/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/vic3_world/countries
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/vic3_world/provinces/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/vic3_world/provinces
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/vic3_world/states/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/vic3_world/states
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/vic3_world/world/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/vic3_world/world
	COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/gcoverage"
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gcoverage
)

add_custom_target(lcov
	COMMAND mkdir -p lcoverage
)
add_custom_command(TARGET lcov
	COMMAND echo "=================== LCOV ===================="
	COMMAND echo "-- Passing lcov tool under code coverage"
	COMMAND lcov --c --d CMakeFiles/Vic3ToHoi4Tests.dir/src/ --o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/11/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/11/bits/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/11/ext/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/x86_64-linux-gnu/c++/11/bits/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/12/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/12/bits/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/12/ext/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/x86_64-linux-gnu/c++/12/bits/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info '*/external/commonItems/*' -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info '*/external/fmt/include/fmt/*' -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info '*/external/rakaly/*' -o lcoverage/coverage.info
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

# Testing
enable_testing()

add_test(Vic3ToHoI4Tests ${CMAKE_CURRENT_BINARY_DIR}/test/Release-Linux/Vic3ToHoI4Tests)


#### Clang-tidy ####

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}")
# checks to maybe add:
#  cert-*
#  cppcoreguidelines-*,

add_library(run_clang_tidy
	${CONVERTER_SOURCES}
	${TEST_SOURCES}
)
target_link_libraries(run_clang_tidy ZIPLIB pthread)
set_target_properties(run_clang_tidy PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
