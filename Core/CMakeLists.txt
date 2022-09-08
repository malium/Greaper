cmake_minimum_required(VERSION 3.16)



project(Core
		VERSION 0.2.7.0
		DESCRIPTION "A cross-platform application framework"
		LANGUAGES CXX)

include_directories(Public/)

file(GLOB source_files CONFIGURE_DEPENDS
	"Private/Core/*.h"
	"Private/Core/*.cpp")

add_library(Core SHARED ${source_files})

target_compile_definitions(Core PRIVATE GREAPER_CORE_DLL)
target_compile_features(Core PRIVATE cxx_std_20)