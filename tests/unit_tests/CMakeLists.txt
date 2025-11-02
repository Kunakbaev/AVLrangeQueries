find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

# Define a function to create test targets
function(create_unit_test test_name source_file)
  add_executable(${test_name} ${source_file})

  # target_compile_options(${test_name} PRIVATE -DNO_LOG -DTESTS_)
  target_compile_definitions(${test_name} PRIVATE
    NO_LOG
    TESTS_
  )
  set_target_properties( ${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")

  target_link_libraries(${test_name} PRIVATE
    GTest::gtest
    GTest::gtest_main
    my_loglib
    Threads::Threads
    my_project_includes
  )
  add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

create_unit_test(avl_tree_common            avl_tree_common_methods_tests.cpp)
create_unit_test(avl_tree_iterators         avl_tree_iterator_tests.cpp)
create_unit_test(avl_tree_lower_upper_bound avl_tree_lower_upper_bound_tests.cpp)

add_custom_target(run_all_tests
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  COMMENT "Running tests with CTest"
)
