cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    canu
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES NONE
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Add dependencies ----

find_package(ROOT)

# ---- Declare library ----

add_library(canu_canu INTERFACE)
add_library(canu::canu ALIAS canu_canu)

set_property(
    TARGET canu_canu PROPERTY
    EXPORT_NAME canu
)

target_include_directories(
    canu_canu ${warning_guard}
    INTERFACE
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(canu_canu INTERFACE cxx_std_17)

target_link_libraries(canu_canu INTERFACE ROOT::Core ROOT::Gpad)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${canu_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT canu_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of canu"
  )
endif()

include(cmake/dev-mode.cmake)
