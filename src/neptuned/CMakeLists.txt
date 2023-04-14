add_executable(neptuned
		main.cpp
		)

neptunevm_target(neptuned)
set_target_properties(neptuned PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
		)

target_include_directories(neptuned PRIVATE ${PROJECT_BINARY_DIR})

target_link_libraries(neptuned PRIVATE
	NeptuneVM::Base
)
