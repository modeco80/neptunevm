//
// NeptuneVM
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#include <GitVersion.hpp>
#include <base/CommonTypes.hpp>
#include <absl/strings/str_cat.h>
#include <absl/flags/usage.h>
#include <absl/flags/parse.h>
#include <absl/flags/flag.h>

#include <absl/debugging/symbolize.h>

#include <base/Panic.hpp>




int main(int argc, char** argv) {
	absl::SetProgramUsageMessage(absl::StrCat("NeptuneVM Daemon version ", neptunevm::version::tag));
	absl::ParseCommandLine(argc, argv);
#ifdef NEPTUNEVM_DEBUG
	absl::InitializeSymbolizer(argv[0]);
#endif

	return 0;
}
