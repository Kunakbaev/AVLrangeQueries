#include <iostream>
#include <vector>
#include <set>

#include "AVL/AVL_tree.hpp"
#include "logLib.hpp"
#include "solutions/solutions_impl.hpp"

int main() {
  setLoggingLevel(DEBUG);

  // solution::solution_t<std::set<int>, int, solution::set_solution_tag> solution;
  // solution.solve();

  solution::solution_t<AVL_tree_t<int>, int, solution::avl_solution_tag> solution;
  solution.solve();


  // AVL_tree_t<int> tree;
  
  // // Insert in a way that would create an unbalanced BST
  // tree.insert(1);
  // tree.insert(2);
  // tree.insert(3);  // This should trigger rotation
  // // tree.visualize_tree("avl_tree_vizualized");
  // //exit(0);

  // std::vector<int> elements;
  // for (auto it = tree.begin(); it != tree.end(); ++it) {
  //   elements.push_back(*it);
  //   if (elements.size() > 8) exit(0);
  // }


  // AVL_tree_t<int> tree{1, 2, 3, 4, 5};
  // auto begin = tree.begin();
  // auto end = tree.end();
  
  // std::size_t distance = end - begin;
  // LOG_DEBUG_VARS(distance);



  // AVL_tree_t<int> tree{1, 2, 3, 4, 5};
  // tree.visualize_tree("avl_tree_vizualized");
  // auto it = tree.begin();
  // auto jt = tree.begin(); ++jt; ++jt;
  // LOG_DEBUG_VARS(jt - it);



  // LOG_DEBUG_VARS(*it); ++it;
  // LOG_DEBUG_VARS(*it); ++it;
  // LOG_DEBUG_VARS(*it); ++it;
  // LOG_DEBUG_VARS(*it); ++it;
  // LOG_DEBUG_VARS(*it); ++it;

  // AVL_tree_t<int> tree;
  // for (int i = 100; i >= 1; --i) {
  //   tree.insert(i);
  // }
  // tree.visualize_tree("avl_tree_vizualized");

  // AVL_tree_t<int>::iterator begin = tree.begin();
  // LOG_DEBUG_VARS(*begin);
  // ++begin;
  // LOG_DEBUG_VARS(*begin);
  // ++begin;
  // LOG_DEBUG_VARS(*begin);

  // ++it;
  // LOG_DEBUG_VARS(it == tree.end());

  // AVL_tree_t<std::uint64_t> avl_tree{1, 2, 10, 5, 8, 2, 3, 9, 11, 9};
  // // AVL_tree_t<std::int64_t> avl_tree{1, 2, 10, 5, 8, 2};
  // avl_tree.visualize_tree("avl_tree_vizualized");
  // AVL_tree_t<std::uint64_t>::iterator begin = avl_tree.begin();

  // --begin;
  // LOG_DEBUG_VARS(*begin);
  // ++begin;
  // LOG_DEBUG_VARS(*begin);
  // --begin;
  // LOG_DEBUG_VARS(*begin);
  // ++begin;
  // LOG_DEBUG_VARS(*begin);
  // for (std::size_t i = 0; i < 10; ++i) {
  //   ++begin;
  //   LOG_DEBUG_VARS(*begin);
  // }

  return 0;
}

/*

input example from presentation:
k 10 k 20 q 8 31 q 6 9 k 30 k 40 q 15 40

correct output:
2 0 3



*/
