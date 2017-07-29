cmake_minimum_required(VERSION 3.5)

add_definitions(-g -std=c++14 -pedantic -Wall -Wextra -Werror)

project(tmln)

add_executable(
  TextMiningCompiler
  src/main.cc
)
