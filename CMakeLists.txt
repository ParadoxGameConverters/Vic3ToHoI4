cmake_minimum_required(VERSION 3.16)
#set(CMAKE_VERBOSE_MAKEFILE on)

project(Vic3ToHoi4)
set(FRONTER_SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/Fronter/Fronter/Source)
set(FRONTER_COMMONS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/Fronter/commonItems)
set(FRONTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/Release-Linux)
set(CONVERTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/Release-Linux/Vic3ToHoi4)
set(TEST_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/test/Release-Linux)
set(CMAKE_CXX_STANDARD 20)
if (WIN32)
  add_definitions(-DUNICODE -D_UNICODE)
else (WIN32)
  set (UNICODE_DEFAULT OFF)
endif (WIN32)
if (COVERAGE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -fprofile-arcs -ftest-coverage")
	set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
endif (COVERAGE)

include_directories(.)
include_directories("./external/googletest/googletest")
include_directories("./external/googletest/googletest/include")
include_directories("./external/googletest/googlemock")
include_directories("./external/googletest/googlemock/include")


#### Converter ####

set(OUT_HOI4_SOURCES ${OUT_HOI4_SOURCES} "src/out_hoi4/out_mod.cpp")
add_library(
	OutHoi4
	${OUT_HOI4_SOURCES}
)

set(MAIN_SOURCE "src/main.cpp")

set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_to_hoi4_converter.cpp")
set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/out_hoi4/out_mod.cpp")

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
if (MSVC)
	set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/WinUtils.cpp")
else(MSVC)
	set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/LinuxUtils.cpp")
endif(MSVC)

add_executable(
	Vic3ToHoi4
	${MAIN_SOURCE}
	${CONVERTER_SOURCES}
	${COMMON_SOURCES}
)
set_target_properties( Vic3ToHoi4
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CONVERTER_OUTPUT_DIRECTORY}
)
target_link_libraries(Vic3ToHoi4 OutHoi4)

configure_file("data/version.txt" "${FRONTER_OUTPUT_DIRECTORY}/version.txt" COPYONLY)
file(COPY "data/blank_mod" DESTINATION "${CONVERTER_OUTPUT_DIRECTORY}")
add_custom_command(TARGET Vic3ToHoi4 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND git rev-parse HEAD > ${CONVERTER_OUTPUT_DIRECTORY}/../commit_id.txt)


#### Fronter ####

if (NOT COVERAGE)
	file(COPY "data/fronter/" DESTINATION "${FRONTER_OUTPUT_DIRECTORY}/Configuration")
	add_custom_command(TARGET Vic3ToHoi4 PRE_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND tools/build_fronter.sh)
endif (COVERAGE)


#### Tests ####

set(OUT_HOI4_TEST_SOURCES ${OUT_HOI4_TEST_SOURCES} "src/out_hoi4/out_mod_test.cpp")
add_library(
	OutHoi4Tests
	${OUT_HOI4_TEST_SOURCES}
)

set(GTEST_SOURCES ${GTEST_SOURCES} "external/googletest/googletest/src/gtest_main.cc")
set(GTEST_SOURCES ${GTEST_SOURCES} "external/googletest/googletest/src/gtest-all.cc")
set(GTEST_SOURCES ${GTEST_SOURCES} "external/googletest/googlemock/src/gmock-all.cc")

set(TEST_SOURCES ${TEST_SOURCES} "src/out_hoi4/out_mod_test.cpp")

add_executable(
	Vic3ToHoi4Tests
	${GTEST_SOURCES}
	${CONVERTER_SOURCES}
	${COMMON_SOURCES}
	${TEST_SOURCES}
)
set_target_properties( Vic3ToHoi4Tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
)

file(COPY "data/blank_mod" DESTINATION "${TEST_OUTPUT_DIRECTORY}")

if (MSVC)
	target_link_libraries(Vic3ToHoi4Tests OutHoi4Tests)
else (MSVC)
	target_link_libraries(Vic3ToHoi4Tests OutHoi4Tests pthread)
endif(MSVC)

if (COVERAGE)
	add_custom_target(gcov
		COMMAND mkdir -p gcoverage
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
	add_custom_command(TARGET gcov
		COMMAND echo "=================== GCOV ===================="
		COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src
		COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/out_hoi4/*.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/Vic3ToHoi4Tests.dir/src/out_hoi4
		COMMAND echo "-- Source directories:"
		COMMAND echo "		${CMAKE_SOURCE_DIR}/src/"
		COMMAND echo "		${CMAKE_SOURCE_DIR}/src/out_hoi4/"
		COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/gcoverage"
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gcoverage
	)

	add_custom_target(lcov
		COMMAND mkdir -p lcoverage
	)
	add_custom_command(TARGET lcov
		COMMAND echo "=================== LCOV ===================="
		COMMAND echo "-- Passing lcov tool under code coverage"
		COMMAND lcov --capture --directory CMakeFiles/Vic3ToHoi4Tests.dir/src/ --output-file lcoverage/main_coverage.info
		COMMAND echo "-- Generating HTML output files"
		COMMAND genhtml lcoverage/main_coverage.info --output-directory lcoverage
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
endif (COVERAGE)