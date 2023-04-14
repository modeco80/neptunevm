//
// NeptuneVM
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>

namespace neptunevm {

	/**
	 * A utility object for safer, non-allocating variants of the impl pattern.
	 * Essentially implements a non-conditional Optional<T>, which doesn't require
	 * T to be fully defined in any other source file, except for the implementation.
	 *
	 * Note that you will have to force the constructor/destructor to be generated
	 * where the type T is fully defined.
	 *
	 * \tparam T 		The class type. Need not be fully defined.
	 * \tparam Size  	Size of T.
	 * \tparam Alignment Alignment of T.
	 */
	template <class T, std::size_t Size, std::size_t Alignment = 4>
	struct SafeImpl {
		using BufferType = std::uint8_t[Size];

		template <class... Args>
		constexpr void Construct(Args&&... args) {
			new(static_cast<void*>(&buffer[0])) T(std::forward<Args>(args)...);
		}

		SafeImpl() {
			Construct();
		}

		SafeImpl(const SafeImpl& copy) {
			Construct(copy.Get());
		}

		// This seems dubious. It might not be right
		SafeImpl(SafeImpl&& move) noexcept {
			Construct(std::move(move.Get()));
		}

		// handy operator overloads to allow for a bit nicer prodding at objects
		constexpr T& operator*() {
			return Get();
		}

		constexpr const T& operator*() const {
			return Get();
		}

		constexpr T& Get() {
			return *reinterpret_cast<T*>(&buffer[0]);
		}

		constexpr const T& Get() const {
			return *reinterpret_cast<const T*>(&buffer[0]);
		}

		~SafeImpl() {
			VerifyConstraints();
			Get().~T();
		}

		constexpr void VerifyConstraints() {
			static_assert(sizeof(T) == Size, "Size invalid");
			static_assert(alignof(T) == Alignment, "Alignment invalid");
		}

	   private:
		alignas(Alignment) BufferType buffer {};
	};

} // namespace mco::ext
