
#set(CJSON_TARGET_NAME "cJSON")
set(TESTAPP_OUTPUT_NAME "TestApplication")

if(WIN32)
	#string(APPEND CJSON_TARGET_NAME "Win")
	string(APPEND TESTAPP_OUTPUT_NAME "Win")
else()
	#string(APPEND CJSON_TARGET_NAME "Lnx")
	string(APPEND TESTAPP_OUTPUT_NAME "Lnx")
endif()

#string(APPEND TESTAPP_OUTPUT_NAME $<$<CONFIG:DEBUG>:"_Debug"$<$<CONFIG:RELEASE>:"_Release">)
set(release FALSE)
if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	#string(APPEND CJSON_TARGET_NAME "_Release")
	#string(APPEND TESTAPP_OUTPUT_NAME "_Release")
	set(release TRUE)
else()
	#string(APPEND CJSON_TARGET_NAME "_Debug")
	#string(APPEND TESTAPP_OUTPUT_NAME "_Debug")
endif()
string(APPEND TESTAPP_OUTPUT_NAME "_" $<CONFIG>)

file(GLOB_RECURSE TESTAPP_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.inl" "*.h")

if(WIN32)
	file(GLOB_RECURSE TO_REMOVE "Lnx/*")
else()
	file(GLOB_RECURSE TO_REMOVE "Win/*")
endif()

list(REMOVE_ITEM TESTAPP_SOURCES ${TO_REMOVE})

add_executable(TestApp ${TESTAPP_SOURCES})
#set_target_properties(TestApp PROPERTIES OUTPUT_NAME ${TESTAPP_OUTPUT_NAME})

target_link_libraries(TestApp cJSON)

set_target_properties(TestApp PROPERTIES
						RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin
						RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin
						RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${PROJECT_SOURCE_DIR}/bin
						RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${PROJECT_SOURCE_DIR}/bin
						OUTPUT_NAME ${TESTAPP_OUTPUT_NAME}
						CXX_STANDARD 17
						C_STANDARD 11)

target_compile_definitions(TestApp PRIVATE $<CONFIG>)
if(MSVC)
	set(MSVC_COMPILE_OPTIONS "/sdl;/Gm-;/EHa;/GF;/Zc:inline;/Zc:forScope;/Zc:wchar_t;/permissive-;/openmp-;/GR;/GS")
	if(release EQUAL TRUE)
		string(APPEND MSVC_COMPILE_OPTIONS ";/Oi;/Ot;/Gy;/fp:fast")
	endif()
	target_compile_options(TestApp PRIVATE ${MSVC_COMPILE_OPTIONS})
	target_compile_definitions(TestApp PRIVATE _CONSOLE=1)
	target_link_options(TestApp PRIVATE /SUBSYSTEM:CONSOLE /MANIFEST:NO)
	
	add_custom_command(TARGET TestApp POST_BUILD COMMAND 
					"mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\Manifest.xml\" -outputresource:\"${PROJECT_SOURCE_DIR}\\bin\\${TESTAPP_OUTPUT_NAME}.exe\"\;\#1
					COMMENT "Adding custom manifest...")
endif()
