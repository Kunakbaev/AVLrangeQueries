import subprocess as sub
from pathlib import Path

PARENT_DIR_PATH = Path(__file__).parent.absolute()
GENERATED_TESTS_DIR_PATH = PARENT_DIR_PATH / "../tests_data"
SOLUTIONS_BINARIES_DIR = PARENT_DIR_PATH / "../../build/source/usecase/"
AVL_SOLUTION_PATH = SOLUTIONS_BINARIES_DIR / "avl_usecase"
SET_SOLUTION_PATH = SOLUTIONS_BINARIES_DIR / "set_usecase"

def run_solution(solution_name: str, test_data: str) -> str:
  return sub.run(solution_name, input=test_data, text=True, capture_output=True).stdout
