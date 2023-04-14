add_library(neptunevm_base
		AsioImpl.cpp
		Panic.cpp
		)

neptunevm_target(neptunevm_base)

target_link_libraries(neptunevm_base PUBLIC
		Boost::asio
		# Abseil
		absl::base
		absl::flags
		absl::flags_parse
		absl::status
		absl::stacktrace
		absl::strings
		absl::time
		fmt::fmt
		)
		
target_include_directories(neptunevm_base PRIVATE ${PROJECT_BINARY_DIR})
# This allows <base/Asio.hpp> to work.
target_include_directories(neptunevm_base PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_compile_definitions(neptunevm_base PUBLIC
		# ASIO options
		# We compile Asio in another TU (provided in this library)
		-DBOOST_ASIO_SEPARATE_COMPILATION=1
		
		# Disable deprecated functionality
		-DBOOST_ASIO_NO_DEPRECATED=1

		# Disable some things which add additional dependencies or are 
		# simply baggage we aren't ever going to use
		-DBOOST_ASIO_DISABLE_BOOST_ARRAY=1
		-DBOOST_ASIO_DISABLE_BOOST_BIND=1
		-DBOOST_ASIO_DISABLE_BOOST_DATE_TIME=1
		-DBOOST_ASIO_DISABLE_BOOST_REGEX=1
		)

# Use this alias.
add_library(NeptuneVM::Base ALIAS neptunevm_base)
