cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    Connect4
    VERSION 0.1.0
    DESCRIPTION "A Connect4 Bot"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

set(CMAKE_CXX_COMPILER "clang++")

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    Connect4_lib OBJECT
    source/board.hpp
    source/board.cpp
    source/game.hpp
    source/game.cpp
)

target_include_directories(
    Connect4_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(Connect4_lib PUBLIC cxx_std_17)


# ---- Declare executable ----

add_executable(Connect4_exe source/main.cpp)
add_executable(Connect4::exe ALIAS Connect4_exe)

set_property(TARGET Connect4_exe PROPERTY OUTPUT_NAME Connect4)

target_compile_features(Connect4_exe PRIVATE cxx_std_17)

target_link_libraries(Connect4_exe PRIVATE Connect4_lib)

target_compile_options(Connect4_lib PRIVATE "-mbmi2")
target_compile_options(Connect4_exe PRIVATE "-mbmi2")

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT Connect4_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of Connect4"
  )
endif()

include(cmake/dev-mode.cmake)
