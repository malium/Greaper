cmake_minimum_required(VERSION 3.16)

project(TestApplication
		VERSION 1.0.0.0
		DESCRIPTION "A testing application for Greaper projects."
		LANGUAGES CXX)

include_directories(../Core/Public)

file(GLOB source_files CONFIGURE_DEPENDS
		"../TestApplication/*.cpp"
		"../Core/Public/Core/*.h"
		"../Core/Public/Core/*.inl"
		"../Core/Public/Core/*/*.h"
		"../Core/Public/Core/*/*.inl")


add_executable(TestApplication ${source_files})
target_compile_features(TestApplication PRIVATE cxx_std_20)