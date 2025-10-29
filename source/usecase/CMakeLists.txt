
add_executable(usecase usecase.cpp)
set_target_properties(usecase PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
target_link_libraries(usecase PRIVATE my_loglib my_project_includes)
