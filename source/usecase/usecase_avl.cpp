#include "AVL/AVL_tree.hpp"
#include "logLib.hpp"
#include "solutions/solutions_impl.hpp"

int main() {
  setLoggingLevel(DEBUG);

  solution::solution_t<AVL_tree_t<int>, int, solution::avl_solution_tag> solution;
  solution.solve();

  return 0;
}

/*

input example from presentation:
k 10 k 20 q 8 31 q 6 9 k 30 k 40 q 15 40

correct output:
2 0 3

*/
