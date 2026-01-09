cmake_minimum_required(VERSION 3.18)
project(openpid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_core MODULE
  cpp/src/bindings.cpp
  cpp/src/pid.cpp
)

target_include_directories(_core PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include
)

if (MSVC)
  target_compile_options(_core PRIVATE /permissive- /W4)
else()
  target_compile_options(_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(_core PROPERTIES OUTPUT_NAME "_core")

install(TARGETS _core
  LIBRARY DESTINATION openpid
  RUNTIME DESTINATION openpid
  ARCHIVE DESTINATION openpid
)
