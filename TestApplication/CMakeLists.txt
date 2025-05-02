
set(TESTAPP_OUTPUT_NAME "TestApplication")

function(generate_wayland_protocols target output_dir protocol_dir protocols)
	message(STATUS "Generating wayland-protocols...")
	find_program(WAYLAND_SCANNER wayland-scanner REQUIRED)
	file(GLOB_RECURSE PROTOCOL_FILES "${protocol_dir}/*.xml")
	foreach(PROTOCOL_LIST ${protocols})
		foreach(PROTOCOL_FILE ${PROTOCOL_FILES})
			get_filename_component(PROTOCOL_NAME ${PROTOCOL_FILE} NAME_WE)
			if (${PROTOCOL_NAME} STREQUAL ${PROTOCOL_LIST})
				#message(STATUS ${PROTOCOL_NAME} " Generated")
				execute_process(
					COMMAND ${WAYLAND_SCANNER} client-header "${PROTOCOL_FILE}" "${output_dir}/${PROTOCOL_NAME}-protocol.h"
				)
				execute_process(
					COMMAND ${WAYLAND_SCANNER} private-code "${PROTOCOL_FILE}" "${output_dir}/${PROTOCOL_NAME}-protocol.c"
				)
				list(APPEND generated_headers "${output_dir}/${PROTOCOL_NAME}-protocol.h")
				target_sources(${target} PRIVATE ${output_dir}/${PROTOCOL_NAME}-protocol.c)
			endif()
		endforeach()
	endforeach()
	target_include_directories(${target} PRIVATE ${output_dir})
	set_source_files_properties(${generated_headers} PROPERTIES HEADER_FILE_ONLY TRUE)
endfunction()

if(WIN32)
	string(APPEND TESTAPP_OUTPUT_NAME "Win")
else()
	string(APPEND TESTAPP_OUTPUT_NAME "Lnx")
endif()

set(release FALSE)
if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(release TRUE)
endif()
string(APPEND TESTAPP_OUTPUT_NAME "_" $<CONFIG>)

file(GLOB_RECURSE TESTAPP_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.c" "*.inl" "*.h")

if(WIN32)
	file(GLOB_RECURSE TO_REMOVE "Lnx/*")

	list(REMOVE_ITEM TESTAPP_SOURCES ${TO_REMOVE})

	add_executable(TestApp ${TESTAPP_SOURCES})

	target_link_libraries(TestApp cJSON)
else()
	file(GLOB_RECURSE TO_REMOVE "Win/*")
	find_package(PkgConfig REQUIRED)
	
	pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)
	
	list(REMOVE_ITEM TESTAPP_SOURCES ${TO_REMOVE})
	
	add_executable(TestApp ${TESTAPP_SOURCES})

	set(generated_protocol_dir "${PROJECT_SOURCE_DIR}/External/wayland-protocols")
	file(MAKE_DIRECTORY ${generated_protocol_dir})

	generate_wayland_protocols(TestApp ${generated_protocol_dir} "/usr/share/wayland-protocols/"
		"xdg-shell;xdg-decoration-unstable-v1")

	target_link_libraries(TestApp cJSON wayland-client)
endif()

set_target_properties(TestApp PROPERTIES
						RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin
						RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin
						RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${PROJECT_SOURCE_DIR}/bin
						RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${PROJECT_SOURCE_DIR}/bin
						OUTPUT_NAME ${TESTAPP_OUTPUT_NAME}
						LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
						CXX_STANDARD 23
						C_STANDARD 17)

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
elseif(CMAKE_COMPILER_IS_GNUCC)
	target_link_options(TestApp PRIVATE -lstdc++ -luuid)
endif()
