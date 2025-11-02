import os
from typing import List
from common import run_solution, AVL_SOLUTION_PATH, SET_SOLUTION_PATH, GENERATED_TESTS_DIR_PATH

def find_first_differ(lhs: List[str], rhs: List[str]) -> int:
  assert len(lhs) == len(rhs)

  for i in range(len(lhs)):
    if lhs[i] != rhs[i]:
      return i
  return -1


def compare_outputs(avl_sol_output: str, set_sol_output: str) -> str:
  if avl_sol_output == set_sol_output:
    return True

  avl_answers = avl_sol_output.split()
  set_answers = set_sol_output.split()
  # print(avl_answers, set_answers)
  if len(avl_answers) != len(set_answers):
    print("Error: answers have different number of elements")
    return False
  
  diff_ind = find_first_differ(avl_answers, set_answers)
  print(f"First difference at query #{diff_ind}:\
          AVL='{avl_answers[diff_ind]}', SET='{set_answers[diff_ind]}'")
  return False


def run_test_case(filename: str) -> bool:
  with open(filename, "r") as file:
    test_case_data = file.read()

  avl_sol_output = run_solution(AVL_SOLUTION_PATH, test_case_data)
  set_sol_output = run_solution(SET_SOLUTION_PATH, test_case_data)
  return compare_outputs(avl_sol_output, set_sol_output)


if __name__ == "__main__":
  for subdir, dirs, files in os.walk(GENERATED_TESTS_DIR_PATH):
    if len(files) == 0:
      continue

    print(f"subdir : {subdir}, dirs : {dirs}, files: {files}")
    for filename in files:
      print(filename)
      full_filename = subdir + "/" + filename
      is_eq = run_test_case(full_filename)
      if not is_eq:
        print(f"Correctness test has failed on test case {full_filename}")
        exit(0)

  print("Correctness test has passed, avl and set solution are most likely work the same")
