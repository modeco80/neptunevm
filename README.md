# NeptuneVM

Fake PC Website But Cooler:tm:

# Building

You will need:
- A Linux box (Windows and macOS are not supported)
- CMake 3.25 or higher
- Python 3.9 or higher
- A C++20 compiler of some sort (clang and gcc work)

Note that you may manually specify `CMAKE_C_COMPILER` or `CMAKE_CXX_COMPILER` to use a specific compiler, if required.

```
$ cmake -Bbuild --preset [release or debug]
$ cmake --build build
```
