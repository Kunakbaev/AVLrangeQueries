
function(create_usecase_target target_name source_file)
  add_executable(${target_name} ${source_file})
  target_compile_definitions(${target_name} PRIVATE NO_LOG TIME_MEASUREMENT_)
  set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
  target_link_libraries(${target_name} PRIVATE my_loglib my_project_includes)
endfunction()

create_usecase_target(avl_perf_measurement perf_measurement_avl.cpp)
create_usecase_target(set_perf_measurement perf_measurement_set.cpp)
