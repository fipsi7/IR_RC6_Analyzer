cmake_minimum_required (VERSION 3.13)

project(RC6Analyzer)

add_definitions( -DLOGIC2 )

# enable generation of compile_commands.json, helpful for IDEs to locate include files.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# custom CMake Modules are located in the cmake directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAnalyzerSDK)

set(SOURCES 
src/RC6Analyzer.cpp
src/RC6Analyzer.h
src/RC6AnalyzerResults.cpp
src/RC6AnalyzerResults.h
src/RC6AnalyzerSettings.cpp
src/RC6AnalyzerSettings.h
src/RC6SimulationDataGenerator.cpp
src/RC6SimulationDataGenerator.h
)

add_analyzer_plugin(${PROJECT_NAME} SOURCES ${SOURCES})
