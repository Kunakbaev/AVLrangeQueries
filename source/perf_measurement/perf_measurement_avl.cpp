#include <chrono>

#include "AVL/AVL_tree.hpp"
#include "common.hpp"
#include "logLib.hpp"
#include "solutions/solutions_impl.hpp"

int main() {
  solution::solution_t<AVL_tree_t<int>, int, solution::avl_solution_tag> solution;
  measure_exec_time_and_print([&solution]{
    solution.solve();
  });

  return 0;
}
