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

include_directories(.)
include_directories("./external/googletest/googletest")
include_directories("./external/googletest/googletest/include")
include_directories("./external/googletest/googlemock")
include_directories("./external/googletest/googlemock/include")


#### Fronter ####

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

include_directories("./external/Fronter/commonItems")
include_directories("./external/Fronter/single_include")
include_directories("/usr/include/wx-3.1-unofficial")
include_directories("/usr/lib/x86_64-linux-gnu/wx/include/gtk3-unicode-3.1-unofficial3")

file(GLOB FRONTER_MAIN_SOURCES "${FRONTER_SOURCE_DIR}/*.cpp")
file(GLOB FRONTER_CONFIGURATION_SOURCES "${FRONTER_SOURCE_DIR}/Configuration/*.cpp")
file(GLOB FRONTER_CONFIGURATION_OPTIONS_SOURCES "${FRONTER_SOURCE_DIR}/Configuration/Options/*.cpp")
file(GLOB FRONTER_FRAMES_SOURCES "${FRONTER_SOURCE_DIR}/Frames/*.cpp")
file(GLOB FRONTER_FRAMES_TABS_SOURCES "${FRONTER_SOURCE_DIR}/Frames/Tabs/*.cpp")
file(GLOB FRONTER_UTILS_SOURCES "${FRONTER_SOURCE_DIR}/Utils/*.cpp")
list(REMOVE_ITEM FRONTER_UTILS_SOURCES "${FRONTER_SOURCE_DIR}/Utils/WinFunctions.cpp")
file(GLOB FRONTER_UTILS_LOCALIZATION_SOURCES "${FRONTER_SOURCE_DIR}/Utils/Localization/*.cpp")
file(GLOB FRONTER_CONVERTERLAUNCHER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/ConverterLauncher/*.cpp")
list(REMOVE_ITEM FRONTER_CONVERTERLAUNCHER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/ConverterLauncher/WinConverterLauncher.cpp")
file(GLOB FRONTER_LOGWATCHER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/LogWatcher/*.cpp")
file(GLOB FRONTER_MODCOPIER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/ModCopier/*.cpp")
file(GLOB FRONTER_UPDATECHECKER_SOURCES "${FRONTER_SOURCE_DIR}/UpdateChecker/*.cpp")
set(FRONTER_COMMON_SOURCES "${FRONTER_COMMONS_SOURCE_DIR}/CommonFunctions.cpp")
set(FRONTER_COMMON_SOURCES ${FRONTER_COMMON_SOURCES} "${FRONTER_COMMONS_SOURCE_DIR}/LinuxUtils.cpp")
set(FRONTER_COMMON_SOURCES ${FRONTER_COMMON_SOURCES} "${FRONTER_COMMONS_SOURCE_DIR}/Log.cpp")
set(FRONTER_COMMON_SOURCES ${FRONTER_COMMON_SOURCES} "${FRONTER_COMMONS_SOURCE_DIR}/OSCommonLayer.cpp")
set(FRONTER_COMMON_SOURCES ${FRONTER_COMMON_SOURCES} "${FRONTER_COMMONS_SOURCE_DIR}/Parser.cpp")
set(FRONTER_COMMON_SOURCES ${FRONTER_COMMON_SOURCES} "${FRONTER_COMMONS_SOURCE_DIR}/ParserHelpers.cpp")
set(FRONTER_COMMON_SOURCES ${FRONTER_COMMON_SOURCES} "${FRONTER_COMMONS_SOURCE_DIR}/StringUtils.cpp")

add_executable(ConverterFrontend
	${FRONTER_MAIN_SOURCES}
	${FRONTER_CONFIGURATION_SOURCES}
	${FRONTER_CONFIGURATION_OPTIONS_SOURCES}
	${FRONTER_FRAMES_SOURCES}
	${FRONTER_FRAMES_TABS_SOURCES}
	${FRONTER_UTILS_SOURCES}
	${FRONTER_UTILS_LOCALIZATION_SOURCES}
	${FRONTER_CONVERTERLAUNCHER_SOURCES}
	${FRONTER_LOGWATCHER_SOURCES}
	${FRONTER_MODCOPIER_SOURCES}
	${FRONTER_UPDATECHECKER_SOURCES}
	${FRONTER_COMMON_SOURCES}
)
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(ConverterFrontend ${wxWidgets_LIBRARIES} ${CURL_LIBRARIES})
set_target_properties( ConverterFrontend
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${FRONTER_OUTPUT_DIRECTORY}
)

configure_file("external/Fronter/Fronter/Resources/converter.ico" "${FRONTER_OUTPUT_DIRECTORY}/converter.ico" COPYONLY)
configure_file("external/Fronter/Fronter/Resources/converter_l_english.yml" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/converter_l_english.yml" COPYONLY)
configure_file("external/Fronter/Fronter/Resources/converter_l_french.yml" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/converter_l_french.yml" COPYONLY)
configure_file("external/Fronter/Fronter/Resources/converter_l_german.yml" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/converter_l_german.yml" COPYONLY)
configure_file("external/Fronter/Fronter/Resources/converter_l_italian.yml" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/converter_l_italian.yml" COPYONLY)
configure_file("external/Fronter/Fronter/Resources/converter_l_latin.yml" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/converter_l_latin.yml" COPYONLY)
configure_file("external/Fronter/Fronter/Resources/converter_l_russian.yml" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/converter_l_russian.yml" COPYONLY)
configure_file("external/Fronter/Fronter/Resources/converter_l_simp_chinese.yml" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/converter_l_simp_chinese.yml" COPYONLY)
configure_file("external/Fronter/Fronter/Resources/converter_languages.yml" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/converter_languages.yml" COPYONLY)
configure_file("external/Fronter/Fronter/Resources/GeneralFAQ-READ.ME.FIRST.txt" "${FRONTER_OUTPUT_DIRECTORY}/GeneralFAQ-READ.ME.FIRST.txt" COPYONLY)
file(COPY "data/fronter/" DESTINATION "${FRONTER_OUTPUT_DIRECTORY}/Configuration")


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

configure_file("data/version.txt" "${FRONTER_OUTPUT_DIRECTORY}/version.txt" COPYONLY)
file(COPY "data/blank_mod" DESTINATION "${CONVERTER_OUTPUT_DIRECTORY}")
add_custom_command(TARGET Vic3ToHoi4 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND git rev-parse HEAD > ${CONVERTER_OUTPUT_DIRECTORY}/../commit_id.txt)


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
else ()
	target_link_libraries(Vic3ToHoI4Tests OutHoi4Tests pthread)
endif()