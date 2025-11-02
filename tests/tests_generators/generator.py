from pathlib import Path
from typing import Tuple
import random

PARENT_DIR_PATH = Path(__file__).parent.absolute()
GENERATED_TESTS_DIR_PATH = PARENT_DIR_PATH / "../tests_data"
ASK_WITHIN_RANGE_PROB = 0.8



def add_insert_query(test_data, key) -> None:
  test_data.append(f"k {key}")

def add_range_query(test_data, min_key, max_key) -> None:
  test_data.append(f"q {min_key} {max_key}")

def generate_queries(
  num_queries: int,
  key_range: Tuple[int,int],
  insert_prob: float,
  # seed: int
) -> str:
  # random.seed(seed)

  test_data = []
  # existing_key = set()
  for _ in range(num_queries):
    if random.uniform(0, 1) < insert_prob:
      key = random.randint(key_range[0], key_range[1])
      add_insert_query(test_data, key)
      # existing_key.add(key)
    else:
      # if existing_key and random.uniform(0, 1) < ASK_WITHIN_RANGE_PROB:
      #   min_key = min(existing_key)
      #   max_key = max(existing_key)
      #   low = random.randint(min_key, max_key)
      #   high = random.randint(low, max_key)
      # else:

      low = random.randint(key_range[0], key_range[1])
      high = random.randint(low, key_range[1])
      add_range_query(test_data, low, high)
      # print(low, high, test_data)

  return "\n".join(test_data)


def generate_test_case_suite(
  base_dir: Path,
  test_case_name: str,
  num_test_cases: int,
  num_queries: int,
  key_range: Tuple[int,int],
  insert_prob: float,
) -> None:
  output_dir = base_dir / test_case_name
  output_dir.mkdir(exist_ok=True)

  for test_id in range(num_test_cases):
    output_file = output_dir / f"test_{test_id}.dat"
    test_case = generate_queries(
      num_queries, key_range, insert_prob
    )

    with open(output_file, "w") as f:
      f.write(str(test_case))


if __name__ == "__main__":
  random.seed(228)
  GENERATED_TESTS_DIR_PATH.mkdir(exist_ok=True)
  generate_test_case_suite(GENERATED_TESTS_DIR_PATH, "tiny",   10, 30,     (-100,    100),    0.3)
  generate_test_case_suite(GENERATED_TESTS_DIR_PATH, "small",  10, 1000,   (-1000,   1000),   0.3)
  generate_test_case_suite(GENERATED_TESTS_DIR_PATH, "medium", 10, 10000,  (-10000,  10000),  0.3)
  generate_test_case_suite(GENERATED_TESTS_DIR_PATH, "large",  10, 100000, (-100000, 100000), 0.3)
