# This defines the subset of boost we pin and use for NeptuneVM
set(_NEPTUNEVM_BOOST_VERSION 1.81.0)
set(BOOST_INCLUDE_LIBRARIES asio beast process)

FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-${_NEPTUNEVM_BOOST_VERSION}/boost-${_NEPTUNEVM_BOOST_VERSION}.tar.xz
  URL_MD5 6cf0cdd797bca685910d527ae3c08cb3
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)

FetchContent_MakeAvailable(Boost)
