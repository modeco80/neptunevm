//
// NeptuneVM
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#include <base/CommonTypes.hpp>
#include <GitVersion.hpp>

#ifdef NEPTUNEVM_DEBUG
#	include <absl/debugging/symbolize.h>
#endif

#include <base/Panic.hpp>

int main(int argc, char** argv) {
#ifdef NEPTUNEVM_DEBUG
	absl::InitializeSymbolizer(argv[0]);
#endif

	neptunevm::Panic("ouchie :(");
	return 0;
}
