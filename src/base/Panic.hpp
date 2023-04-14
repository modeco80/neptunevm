#pragma once
#include <fmt/core.h>
#include <string_view>

namespace neptunevm {

	namespace detail {
		[[noreturn]] void Panic(std::string_view message);
	}

	[[noreturn]] inline void Panic() {
		detail::Panic({});
	}

	/**
	 * Panic NeptuneVM.
	 */
	template<typename... Args>
	[[noreturn]] inline void Panic(fmt::format_string<Args...> format, Args&&... args) {
			auto formatted = fmt::format(format, static_cast<Args&&>(args)...);
			detail::Panic(formatted);
			__builtin_unreachable();
	}


}
