macro(_neptunevm_target_gcclike target)
	set(_CORE_COMPILE_ARGS -Wall -Wextra)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		set(_CORE_COMPILE_ARGS ${_CORE_COMPILE_ARGS} -Werror)
	endif()
endmacro()

function(neptunevm_target target)
	target_compile_definitions(${target} PRIVATE "$<$<CONFIG:DEBUG>:NEPTUNEVM_DEBUG>")
	target_compile_features(${target} PUBLIC cxx_std_20)

	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		_neptunevm_target_gcclike(${target})
	else()
		message(FATAL_ERROR "I do not know how to setup ${CMAKE_CXX_COMPILER_ID}")
	endif()

	# default compile options to the core compile flags set by the macro implementation
	target_compile_options(${target} PRIVATE ${_CORE_COMPILE_ARGS})

	# add git version dependency
	add_dependencies(${target} _git_version)
endfunction()
