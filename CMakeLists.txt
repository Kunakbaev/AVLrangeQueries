cmake_minimum_required(VERSION 3.16)
project(AVL_range_queries)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===================================================

add_library(my_project_includes INTERFACE)
target_include_directories(my_project_includes INTERFACE
  ${CMAKE_SOURCE_DIR}/include/
  ${CMAKE_SOURCE_DIR}/3dPartyModules/LoggerLib/include
)

# ===================================================

set(COMMON_CXX_FLAGS "-lm -ggdb3 -std=c++17 -Werror -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  # removes assert checks and log lib output
  add_compile_options(-O3 -DNDEBUG -DNO_LOG)
  message(STATUS "Building in Release mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(${COMMON_CXX_FLAGS})
  add_compile_options(-g -O0 -D_DEBUG)
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

add_subdirectory(tests/unit_tests)
add_subdirectory(source/usecase)
