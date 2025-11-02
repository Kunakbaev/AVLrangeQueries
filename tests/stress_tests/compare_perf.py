from common import run_solution, AVL_SOLUTION_PATH, SET_SOLUTION_PATH, GENERATED_TESTS_DIR_PATH
import os
import statistics
import time
from typing import List, Tuple

def measure_solution_perf(sol_name, test_data) -> float:
  start = time.time()
  _ = run_solution(sol_name, test_data)
  finish = time.time()
  return finish - start

def run_test_case(filename: str) -> Tuple[float, float, float]:
  with open(filename, "r") as file:
    test_case_data = file.read()

  avl_time = measure_solution_perf(AVL_SOLUTION_PATH, test_case_data)
  set_time = measure_solution_perf(SET_SOLUTION_PATH, test_case_data)
  coef = 1.0
  if abs(avl_time) != 0:
    coef = set_time / avl_time
  # print(f"avl solution is {coef:.3f} times faster than std::set")
  # print(f"avl: {avl_time:.3f}, set: {set_time:.3f}")
  return (avl_time, set_time, coef)

def get_statistics(data: List[float]) -> dict:
  stats = {
    "mean":     statistics.mean(data),
    "median":   statistics.median(data),
    "variance": statistics.variance(data),
    "std_dev":  statistics.stdev(data)
  }

  return stats

def derive_meaning_from_perf_res(perf_measure_res: List[float]):
  avl_perf, set_perf, perf_diff_coefs = zip(*perf_measure_res)
  avl_perf_stats = get_statistics(avl_perf)
  set_perf_stats = get_statistics(set_perf)
  stats = get_statistics(perf_diff_coefs)
  mean_coef = stats['mean']

  print(f"Performance Statistics (set_time/avl_time):")
  print(f"  Mean:               {mean_coef:.3f}")
  print(f"  Median:             {stats['median']:.3f}")
  print(f"  Variance:           {stats['variance']:.3f}")
  print(f"  Standard Deviation: {stats['std_dev']:.3f}")
  print(f"AVL      worked for {avl_perf_stats['mean']:.3f} on average")
  print(f"Std::set worked for {set_perf_stats['mean']:.3f} on average")

  if mean_coef > 1:
    print(f"AVL is {mean_coef:.2f}x faster on average")
  else:
    print(f"std::set is {1/mean_coef:.2f}x faster on average")

if __name__ == "__main__":
  for subdir, dirs, files in os.walk(GENERATED_TESTS_DIR_PATH):
    if len(files) == 0:
      continue

    print("==================================")
    perf_measure_res = []
    for filename in files:
      full_filename = subdir + "/" + filename
      perf_measure_res.append(run_test_case(full_filename))

    test_type = subdir.split('/')[-1]
    print(f"Performance data for test type: {test_type}")
    derive_meaning_from_perf_res(perf_measure_res)      

  print("Done!!!")
