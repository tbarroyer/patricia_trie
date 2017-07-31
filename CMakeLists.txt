cmake_minimum_required(VERSION 3.5)

enable_language(CXX)

set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES .so;.a)
set(DOCUMENTATION_DIR doc)

add_definitions(-O3 -march=native -std=c++14 -pedantic -Wall -Wextra)

#FIND_PACKAGE(Boost 1.58 COMPONENTS serialization REQUIRED)
#INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})


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

### Documentation
find_package(Doxygen)
if (DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}Doxyfile @ONLY)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

TARGET_LINK_LIBRARIES(TextMiningCompiler LINK_PUBLIC ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(TextMiningApp LINK_PUBLIC ${Boost_LIBRARIES})
