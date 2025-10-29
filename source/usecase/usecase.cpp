#include <iostream>
#include <vector>

#include "AVL_tree.hpp"
#include "logLib.hpp"

int main() {
  setLoggingLevel(DEBUG);
  LOG_DEBUG("HELLO FROM LOGGER!");
  AVL_tree_t<std::int64_t> avl_tree{1, 2, 10, 5, 8, 2, 3, 9, 11, 9};
  // AVL_tree_t<std::int64_t> avl_tree{1, 2, 10, 5, 8, 2};
  avl_tree.visualize_tree("avl_tree_vizualized");

  return 0;
}
