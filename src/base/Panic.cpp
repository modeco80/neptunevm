
#include <base/CommonTypes.hpp>
#include <spdlog/spdlog.h>

#ifdef NEPTUNEVM_DEBUG
#include <absl/debugging/stacktrace.h>
#include <absl/debugging/symbolize.h>
#endif

namespace {

	inline pid_t GetPanicTid() {
		return static_cast<pid_t>(syscall(SYS_gettid));
	}


}

namespace neptunevm {
	namespace detail {

		void Panic(std::string_view message) {
			if(message.empty())
				spdlog::critical("Thread {} paniced", GetPanicTid());
			else
				spdlog::critical("Thread {} paniced: {}", GetPanicTid(), message);

#ifdef NEPTUNEVM_DEBUG
			constexpr auto PANIC_MAX_FRAMES = 32;
			constexpr auto PANIC_MAX_FN_SIZE = 256;

			void* stack[PANIC_MAX_FRAMES]{};
			auto stack_size = absl::GetStackTrace(&stack[0], PANIC_MAX_FRAMES, 2);

			spdlog::critical("Stack trace:");
			for(auto i = 0; i <= stack_size - 1; ++i) {
				char symbolized_name[PANIC_MAX_FN_SIZE]{};
				if(absl::Symbolize(stack[i], &symbolized_name[0], PANIC_MAX_FN_SIZE)) {
					spdlog::critical("    {}    {}", stack[i], symbolized_name);
				} else {
					spdlog::critical("    {}    (unknown)", stack[i]);
				}
			}
#endif

			std::quick_exit(4004); 
		}
	}


}
