cmake_minimum_required(VERSION 3.16)
#set(CMAKE_VERBOSE_MAKEFILE on)

project(Vic2ToHoI4)
#set(DATA_FILE_DIR ${CMAKE_SOURCE_DIR}/Vic2ToHoI4/Data_Files)
#set(FRONTER_DATA_FILE_DIR ${CMAKE_SOURCE_DIR}/Fronter/Fronter/Resources)
#set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Vic2ToHoI4/Source)
#set(TEST_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Vic2ToHoI4Tests)
#set(FRONTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Release-Linux)
#set(CONVERTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Release-Linux/Vic2ToHoI4)
#set(TEST_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Release-Tests)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
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

#add_compile_options("-std=c++20")
#add_compile_options("-lstdc++fs")
#add_compile_options("-O3")

set(OUT_HOI4_SOURCES ${OUT_HOI4_SOURCES} "src/out_hoi4/out_mod.cpp")
add_library(
	OutHoi4
	${OUT_HOI4_SOURCES}
)

set(MAIN_SOURCE "src/main.cpp")

set(CONVERTER_SOURCES ${CONVERTER_SOURCES} "src/vic3_to_hoi4_converter.cpp")

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
endif()

add_executable(
	Vic3ToHoi4Converter
	${MAIN_SOURCE}
	${CONVERTER_SOURCES}
	${COMMON_SOURCES}
)
target_link_libraries(Vic3ToHoi4Converter OutHoi4)

set(OUT_HOI4_TEST_SOURCES ${OUT_HOI4_TEST_SOURCES} "src/out_hoi4/out_mod_test.cpp")
add_library(
	OutHoi4Tests
	${OUT_HOI4_TEST_SOURCES}
)

set(GTEST_SOURCES "external/googletest/googletest/src/gtest_main.cc")
set(GTEST_SOURCES ${GTEST_SOURCES} "external/googletest/googletest/src/gtest-all.cc")
set(GTEST_SOURCES ${GTEST_SOURCES} "external/googletest/googlemock/src/gmock-all.cc")

add_executable(
	Vic3ToHoi4ConverterTests
	${GTEST_SOURCES}
)
if (MSVC)
	target_link_libraries(Vic3ToHoi4ConverterTests OutHoi4Tests)
else (MSVC)
	target_link_libraries(Vic3ToHoi4ConverterTests OutHoi4Tests pthread)
endif()