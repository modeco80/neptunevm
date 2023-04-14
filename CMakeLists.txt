cmake_minimum_required(VERSION 3.25)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(neptunevm
	LANGUAGES CXX
	DESCRIPTION "NeptuneVM"
	VERSION 0.1
	)

# Set the default C++ standard for (bad) projects
# which do not use compile features.
set(CMAKE_CXX_STANDARD 20)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(Policies)
include(Project)
include(FetchContent)

find_package(Python3 REQUIRED
		COMPONENTS Interpreter)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	# Target the Windows 10 API set (while it would be nice to support older Windows versions
	# it's more legacy baggage and I'd rather not encumber that right away)
	#
	# TODO: Move this to the project setup calls....
	add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

# Add a target for Git version generation.
set(VERSION_FILENAME GitVersion.hpp)
add_custom_target(_git_version
	COMMAND $<TARGET_FILE:Python3::Interpreter> ${PROJECT_SOURCE_DIR}/tools/build/gitversion.py ${VERSION_FILENAME}
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	SOURCES ${PROJECT_BINARY_DIR}/${VERSION_FILENAME})
set_source_files_properties(${PROJECT_BINARY_DIR}/${VERSION_FILENAME} PROPERTIES GENERATED TRUE)


# Tell spdlog *not* to use its bundled {fmt}
set(SPDLOG_FMT_EXTERNAL ON)
set(ABSL_PROPAGATE_CXX_STD ON)

# third party dependencies
add_subdirectory(third_party/absl EXCLUDE_FROM_ALL)
add_subdirectory(third_party/boost EXCLUDE_FROM_ALL)
add_subdirectory(third_party/fmt EXCLUDE_FROM_ALL)
add_subdirectory(third_party/spdlog EXCLUDE_FROM_ALL)

add_subdirectory(src/base)
add_subdirectory(src/neptuned)
