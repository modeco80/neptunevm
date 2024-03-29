# NeptuneVM Error Handling Strategy

This document outlines the strategies the NeptuneVM code will use for error handling.

Exceptions are not outright disabled, however the NeptuneVM code does not use exceptions for error handling.

## Recoverable Errors

`absl::Status` and `absl::StatusOr<T>` are used for handling recoverable errors.

## Fatal Errors

Fatal (non-recoverable) errors can be signaled with the `neptunevm::Panic()` function.

Once called, Panic() will output the given (formatted) message, then exit fast with a non-zero exit code.

## Assertions/(Pre|Post)conditions

NeptuneVM code has two assertion systems, which are used to assert pre/postconditions:

- `NEPTUNEVM_ASSERT(expr)`/`NEPTUNEVM_VERIFY()`
    - These simply Panic() if the assertion fails. VERIFY() is always active, even in Release/optimized builds. These should only be used if violation of the pre/postcondition is fatal. If it is not, then use...
- `NEPTUNEVM_CHECK(expr, ErrorStatement...)`
    - ... which will run a user-defined error statement on assertion failure, regardless of build type.
