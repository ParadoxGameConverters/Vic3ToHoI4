cmake_minimum_required(VERSION 3.16)
#set(CMAKE_VERBOSE_MAKEFILE on)

project(Vic3ToHoi4)
#set(DATA_FILE_DIR ${CMAKE_SOURCE_DIR}/Vic2ToHoI4/Data_Files)
#set(FRONTER_DATA_FILE_DIR ${CMAKE_SOURCE_DIR}/Fronter/Fronter/Resources)
#set(FRONTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Release-Linux)
set(CONVERTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/Release-Linux)
set(TEST_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/test/Release-Linux)
set(CMAKE_CXX_STANDARD 20)
if (WIN32)
  add_definitions(-DUNICODE -D_UNICODE)
else (WIN32)
  set (UNICODE_DEFAULT OFF)
endif (WIN32)

include_directories(.)
include_directories("./external/googletest/googletest")
include_directories("./external/googletest/googletest/include")
include_directories("./external/googletest/googlemock")
include_directories("./external/googletest/googlemock/include")

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
else()
	set(COMMON_SOURCES ${COMMON_SOURCES} "external/commonItems/LinuxUtils.cpp")
endif()

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

file(COPY "data/blank_mod" DESTINATION "${CONVERTER_OUTPUT_DIRECTORY}")
add_custom_command(TARGET Vic3ToHoi4 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND git rev-parse HEAD > ${CONVERTER_OUTPUT_DIRECTORY}/commit_id.txt)

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
	Vic3ToHoI4Tests
	${GTEST_SOURCES}
	${CONVERTER_SOURCES}
	${COMMON_SOURCES}
	${TEST_SOURCES}
)
set_target_properties( Vic3ToHoI4Tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
)

file(COPY "data/blank_mod" DESTINATION "${TEST_OUTPUT_DIRECTORY}")

if (MSVC)
	target_link_libraries(Vic3ToHoI4Tests OutHoi4Tests)
else (MSVC)
	target_link_libraries(Vic3ToHoI4Tests OutHoi4Tests pthread)
endif()