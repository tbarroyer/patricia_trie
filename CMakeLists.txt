cmake_minimum_required(VERSION 3.5)

enable_language(CXX)

set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES .so;.a)

add_definitions(-O3 -std=c++14 -pedantic -Wall -Wextra)

FIND_PACKAGE(Boost 1.58 COMPONENTS serialization REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})


project(tmln)

add_executable(
  TextMiningCompiler
  src/main.cc
  src/patricia_trie.cc
  src/tools.cc
)

add_executable(
  TextMiningApp
  src/main_app.cc
  src/patricia_trie.cc
  src/tools.cc
)

TARGET_LINK_LIBRARIES(TextMiningCompiler LINK_PUBLIC ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(TextMiningApp LINK_PUBLIC ${Boost_LIBRARIES})
