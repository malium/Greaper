
set(TESTAPP_OUTPUT_NAME "TestApplication")

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
	include_directories(${WAYLAND_PROTOCOLS_INCLUDE_DIRS})

	message(STATUS ${WAYLAND_PROTOCOLS_INCLUDE_DIRS})
	
	#find_package(Wayland REQUIRED)

	list(REMOVE_ITEM TESTAPP_SOURCES ${TO_REMOVE})

	add_executable(TestApp ${TESTAPP_SOURCES})

	target_link_libraries(TestApp cJSON wayland-client)
endif()

set_target_properties(TestApp PROPERTIES
						RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin
						RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin
						RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${PROJECT_SOURCE_DIR}/bin
						RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${PROJECT_SOURCE_DIR}/bin
						OUTPUT_NAME ${TESTAPP_OUTPUT_NAME}
						LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
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
elseif(CMAKE_COMPILER_IS_GNUCC)
	target_link_options(TestApp PRIVATE -lstdc++)
endif()
