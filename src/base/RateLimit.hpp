//
// NeptuneVM
//
// (C) 2022-2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>

// TODO: Rewrite this file using Abseil time
// (which would in theory allow it to be implemented in a .cpp file)

namespace neptunevm {

	/**
	 * A simple rate limiter object.
	 *
	 * Example usage:
	 * \code
	 * 		neptunevm::RateLimit limit(100, 1s, 15s); // 100 events/sec; 15 second cool-down
	 *
	 * 		// This would pe part of a user/request structure; ideally per-user/per-IP,
	 *		// so that an IP can't bypass it by (for instance) just reconnecting.
	 * 		neptunevm::RateLimit::State state;
	 *
	 * 		if(!limit.TryTakeEvent(state)) {
	 *			// The rate-limit was tripped.
	 *		}
	 *
	 * 		// The rate-limit was not tripped, and we can process this request.
	 * \endcode
	 *
	 * \tparam Clock Clock type used to get time. Monotonic is a sane default.
	 * \tparam Dur Internal duration type. Shouldn't need to be modified for most cases.
	 */
	template <class Clock = std::chrono::steady_clock, class Dur = std::chrono::microseconds>
	struct BasicRateLimiter {
		/**
		 * The native time_point type the rate limiter uses.
		 */
		using TimePointType = std::chrono::time_point<Clock, Dur>;

		/**
		 * Event token type.
		 */
		using EventGrainType = std::uint64_t;

		/**
		 * Per-user state. Mostly opaque
		 *
		 * This is intended to be passed into the rate limiter.
		 */
		struct State {
			[[nodiscard]] bool CoolingDown() const noexcept {
				return coolingDown.load();
			}

		   private:
			friend struct BasicRateLimit;

			std::atomic_bool coolingDown {};

			/**
			 * The time point when the last event was (attempted) to be taken.
			 */
			std::atomic<TimePointType> lastEvent {};

			/**
			 * Current event count.
			 */
			std::atomic<EventGrainType> eventCount {};
		};

		/**
		 * Constructor.
		 *
		 * Is a template so that other input durations are acceptable
		 * & converted into the internal duration type.
		 *
		 * \param[in] maxEvents    Max amount of events in \ref max_rate time.
		 * \param[in] maxRate      Max rate of time. Typically 1 second (for max_events in 1 second).
		 * \param[in] cooldownTime Cooldown time.
		 */
		template <class Dur2, class Dur3>
		constexpr BasicRateLimiter(EventGrainType maxEvents, Dur2 maxRate, Dur3 cooldownTime) noexcept
			: cooldownTime(cooldownTime),
			  maxRate(maxRate),
			  maxEventCount(maxEvents) {
		}

		// Disallow copying, but allow movement, if so desired.
		constexpr BasicRateLimiter(const BasicRateLimiter&) = delete;
		constexpr BasicRateLimiter(BasicRateLimiter&&) noexcept = default;

		/**
		 * Try and take a single event, possibly activating the rate limit.
		 *
		 * \param[in, out] state    The state to use. Will be modified by this function.
		 *
		 * \returns See BasicRateLimit<Clock, Dur>::TryTakeEvents()
		 */
		[[nodiscard]] constexpr bool TryTakeEvent(State& state) const noexcept {
			return TryTakeEvents(state, 1);
		}

		/**
		 * Try and take events, possibly activating the rate limit.
		 *
		 * \param[in, out] state    The state to use. Will be modified by this function.
		 * \param[in]      nrEvents How many events to try taking.
		 *
		 * \returns True if the event was taken. False if the rate-limit was activated or in cooldown.
		 */
		[[nodiscard]] bool TryTakeEvents(State& state, EventGrainType nrEvents) const noexcept {
			// Pre-calculate the current time & the delta time that has
			// elapsed since we last entered this function.
			//
			// This doesn't speed things up per se, but it probably aides
			// the compiler a bit to optimize things a bit better.

			const auto now = std::chrono::time_point_cast<Dur>(Clock::now());
			const auto elapsedSinceLastEvent = (now - state.lastEvent.load());

			if(state.coolingDown.load()) {
				// Check if we have passed the cool-down time (if we're cooling down).
				// If we have, we can let the cooldown go, and let the state take the
				// events.
				if(elapsedSinceLastEvent >= cooldownTime && state.coolingDown.load())
					state.coolingDown.store(false);
				else
					return false;
			}

			if(elapsedSinceLastEvent < maxRate) {
				// Check if the event count has went past [maxEventCount]/[maxRate].
				// If it has, we start the cool-down process.
				if((state.eventCount += nrEvents) >= maxEventCount) {
					state.coolingDown.store(true);
					state.lastEvent.store(now);
					return false;
				}
			} else {
				// The event happened far after max rate, so it's probably fine.
				state.eventCount = 0;
				state.lastEvent.store(now);
			}

			return true;
		}

		/**
		 * Set a new max event count.
		 *
		 * \param[in] count New count.
		 */
		constexpr void SetMaxEventCount(EventGrainType count) noexcept {
			maxEventCount = count;
		}

		/**
		 * Set a new max rate.
		 *
		 * \param[in] dur New max rate.
		 */
		template <class Dur2>
		constexpr void SetMaxRate(Dur2 dur) noexcept {
			maxRate = dur;
		}

		/**
		 * Set a new cooldown time.
		 *
		 * \param[in] dur New cooldown time.
		 */
		template <class Dur2>
		constexpr void SetCooldownTime(Dur2 dur) noexcept {
			cooldownTime = dur;
		}

	   private:
		/**
		 * The amount of time needed to cool down and accept events from a state once
		 * cool-down has begun.
		 */
		Dur cooldownTime;

		/**
		 * Max rate.
		 */
		Dur maxRate;

		/**
		 * Max event count.
		 */
		EventGrainType maxEventCount;
	};

	/**
	 * A default rate limiter.
	 */
	using RateLimiter = BasicRateLimiter<>;

} // namespace mco::ext
