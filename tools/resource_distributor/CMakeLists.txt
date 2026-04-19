cmake_minimum_required(VERSION 3.25)
set(VERBOSE true)

project(Vic3ToHoi4-ResourceDistributor)
if (RUN_CLANG_TIDY)
	set(BUILD_TYPE "Clang-tidy")
else (RUN_CLANG_TIDY)
	set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif (RUN_CLANG_TIDY)

set(FRONTER_COMMONS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/../../external/Fronter/commonItems)
set(CONVERTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${BUILD_TYPE}-${PLATFORM}/Vic3ToHoi4-ResourceDistributor)

set(CMAKE_CXX_STANDARD 23)
set (UNICODE_DEFAULT OFF)

if (PLATFORM STREQUAL "Windows")
	ADD_DEFINITIONS(-DUNICODE -D_UNICODE -DWINDOWS)
elseif (PLATFORM STREQUAL "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall")
endif (PLATFORM STREQUAL "Windows")


#### ResourceDistributor ####

include_directories(.)
include_directories("../../external/fmt/include")
include_directories("../../external/commonItems")

set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/bitmap/bitmap_image.hpp")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/BulkParser.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/Color.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/CommonFunctions.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/CommonRegexes.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/ConvenientParser.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/ConverterVersion.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/Date.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/GameVersion.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/Localization/LocalizationBlock.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/Localization/LocalizationDatabase.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/Localization/LocalizationLambdas.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/Log.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/ModLoader/Mod.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/ModLoader/ModFilesystem.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/ModLoader/ModLoader.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/ModLoader/ModParser.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/OSCompatibilityLayer.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/Parser.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/ParserHelpers.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/StringUtils.h")
set(PCH_SOURCES ${PCH_SOURCES} "${PROJECT_SOURCE_DIR}/../../external/commonItems/targa.h")

set(MAIN_SOURCE "main.cpp")

set(FORMAT_SOURCES ${FORMAT_SOURCES} "../../external/fmt/src/format.cc")
set(FORMAT_SOURCES ${FORMAT_SOURCES} "../../external/fmt/src/os.cc")
add_library(fmtLib ${FORMAT_SOURCES})

add_subdirectory(../../external/commonItems ${CMAKE_SOURCE_DIR}/build/${BUILD_TYPE}-${PLATFORM}/Vic3ToHoi4-ResourceDistributor EXCLUDE_FROM_ALL)

add_executable(
	Vic3ToHoi4-ResourceDistributor
	${MAIN_SOURCE}
 )
set_target_properties( Vic3ToHoi4-ResourceDistributor
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CONVERTER_OUTPUT_DIRECTORY}
)
target_precompile_headers(Vic3ToHoi4-ResourceDistributor PUBLIC ${PCH_SOURCES})
if (PLATFORM STREQUAL "Windows")
	target_compile_options(Vic3ToHoi4-ResourceDistributor PRIVATE /permissive /W4 /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 
        /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928 
        /external:anglebrackets /external:W0 /WX)
	target_link_libraries(Vic3ToHoi4-ResourceDistributor LINK_PUBLIC commonLib fmtLib)
elseif (PLATFORM STREQUAL "Linux")
	target_link_libraries(Vic3ToHoi4-ResourceDistributor LINK_PUBLIC commonLib fmtLib)
endif (PLATFORM  STREQUAL "Windows")


#### Clang-tidy ####

#find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
#set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}")
# checks to maybe add:
#  cert-*
#  cppcoreguidelines-*,

add_library(run_clang_tidy
	${MAIN_SOURCE}
 )
target_link_libraries(run_clang_tidy pthread)
set_target_properties(run_clang_tidy PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
add_dependencies(run_clang_tidy Vic3ToHoi4-ResourceDistributor)
