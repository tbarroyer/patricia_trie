cmake_minimum_required(VERSION 3.5)

enable_language(CXX)

set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES .so;.a)

add_definitions(-g -std=c++14 -pedantic -Wall -Wextra)

FIND_PACKAGE(Boost 1.58 COMPONENTS serialization REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})


project(tmln)

add_executable(
  TextMiningCompiler
  src/main.cc
  src/patricia_trie.cc
  src/tools.cc
)

TARGET_LINK_LIBRARIES(TextMiningCompiler LINK_PUBLIC ${Boost_LIBRARIES})
