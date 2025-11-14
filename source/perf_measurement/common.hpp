#pragma once

#include <chrono>
#include <functional>

// prints time of function execution in milliseconds
inline void measure_exec_time_and_print(std::function<void()> function) {
  auto start_time  = std::chrono::high_resolution_clock::now();
  function();
  auto finish_time = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - start_time);
  std::cout << duration.count() << std::endl;
}
