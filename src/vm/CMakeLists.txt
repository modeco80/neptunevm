add_library(neptunevm_vm
		AsioImpl.cpp
		Panic.cpp
		)

neptunevm_target(neptunevm_vm)

target_link_libraries(neptunevm_base PUBLIC
		NeptuneVM::Base
		)
		

# Use this alias.
add_library(NeptuneVM::VM ALIAS neptunevm_vm)
