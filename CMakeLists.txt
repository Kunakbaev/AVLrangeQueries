cmake_minimum_required(VERSION 3.16)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AVL_range_queries)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# ===================================================

add_library(my_project_includes INTERFACE)
target_include_directories(my_project_includes INTERFACE
  ${CMAKE_SOURCE_DIR}/include/
  ${CMAKE_SOURCE_DIR}/3dPartyModules/LoggerLib/include
)

# ===================================================

# basic flags for compilation and debugging
set(DEBUG_FLAGS -ggdb3 -fno-omit-frame-pointer)

# general warnings
set(WARNING_FLAGS -Wall -Wextra -Wpedantic)

# security flags
set(SECURITY_FLAGS -fstack-protector -pie -fPIE -Wformat=2 -Wformat-security)

# specific flags
set(CPP_SPECIFIC_FLAGS -Wnon-virtual-dtor -Woverloaded-virtual)

set(COMMON_CXX_FLAGS ${WARNING_FLAGS})

# ======================================================

message("CXX_COMPILER: " ${CMAKE_CXX_COMPILER})
message("CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(NO_LOG NDEBUG)
  set(COMPILATION_FLAGS ${COMMON_CXX_FLAGS} -O2 -DNDEBUG)
  message(STATUS "Building in Release mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG_)
  set(COMPILATION_FLAGS ${COMMON_CXX_FLAGS} -O0 -g ${SECURITY_FLAGS} ${CPP_SPECIFIC_FLAGS})
  message(STATUS "Building in Debug mode")
endif()

# ====================================================

# 1. Сначала генерируем config файл
include(CMakePackageConfigHelpers)
configure_package_config_file(
  3dPartyModules/LoggerLib/cmake/my_loglibConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/my_loglibTargets.cmake
  INSTALL_DESTINATION lib/cmake/my_loglib
)

# 2. Указываем КОНКРЕТНЫЙ путь к файлу
set(my_loglib_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# 3. Добавляем subdirectory
add_subdirectory(3dPartyModules/LoggerLib)

# ====================================================

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/help_message.txt CONTENT "
Available targets:
  * usecase targets (dynamic range queries task solvers):
    * avl_usecase
    * set_usecase
    Both targets expect input in the following format: 'k 10 k 20 q 8 31'.
      + 'k' represents insert operation and one integer number is expected after it.
      + 'q' denotes range query, expects 2 integers after symbol: low_key and high_key
    Stdout: for each query of type 'q' (range query), number of keys, that are contained in the set at the moment of query, is printed
  * unit tests (tests for different groups of methods of AVL tree):
    * avl_tree_common - checks correctness of insert operations (however to get data from tree it also uses iterators)
    * avl_tree_iterators - validates increment, decrement of iterators and distance between them
    * avl_tree_lower_upper_bound - checks that methods 'lower_bound' and 'upper_bound' work correctly
  * to run all tests perform following (from the project root dir):
    cd build && ctest
")

add_custom_target(show_help_msg
  COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_CURRENT_BINARY_DIR}/help_message.txt
)

# ====================================================

add_subdirectory(tests/unit_tests)
add_subdirectory(source/usecase)
