#include <base/CommonTypes.hpp>

namespace neptunevm {

	/** A string type that is usable as a C++20 cNTTP at compile time. */
	template<usize N>
	struct FixedString {
		char buf[N + 1] {};

		constexpr FixedString(const char* s) { // NOLINT
			for(unsigned i = 0; i != N; ++i)
				buf[i] = s[i];
		}

		constexpr operator const char*() const { // NOLINT
			return buf;
		}

		[[nodiscard]] constexpr usize Length() const { return N; }
	};

	template<usize N>
	FixedString(char const (&)[N]) -> FixedString<N - 1>;

} // namespace neptunevm
