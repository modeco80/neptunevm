#include <variant>

namespace neptunevm {

	struct QEMUVirtualMachine;

	enum class VirtualMachineRunState {
		Stopped,  ///< The VM is stopped.
		Starting, ///< The VM is starting.
		Started	  ///< The VM is running.
	};

	enum class VirtualMachineKind {
		Invalid,
		QEMU	///< QEMU Virtual machine.
	};

	//clang-format off

	using VirtualMachine = std::variant<
		QEMUVirtualMachine
	>;

	// clang-format on

} // namespace neptunevm
