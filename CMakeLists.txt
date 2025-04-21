# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Greaper" VERSION 0.3.0.0 LANGUAGES CXX C)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
	string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -Wall -msse4.1 -Wno-long-long -pedantic -Wno-comment -Wno-unused-function -Wno-ignored-attributes")
endif()

add_subdirectory("cJSON")

if(MSVC)
	if(CMAKE_CXX_FLAGS MATCHES "/EHs")
		string(REGEX REPLACE "/EHs" "/EHa" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	endif()
endif()

# Include sub-projects.
add_subdirectory ("GreaperCore")

#add_subdirectory ("GreaperGAL")


add_subdirectory ("TestApplication")
