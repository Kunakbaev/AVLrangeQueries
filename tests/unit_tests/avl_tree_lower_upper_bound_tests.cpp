#include <gtest/gtest.h>

#include "AVL/AVL_tree.hpp"

TEST(AVLTreeBounds, LowerBoundBasic) {
  AVL_tree_t<int> tree{1, 3, 5, 7, 9};
  
  // Exact matches
  EXPECT_EQ(*tree.lower_bound(3), 3);
  EXPECT_EQ(*tree.lower_bound(5), 5);
  EXPECT_EQ(*tree.lower_bound(7), 7);
  
  // Values between elements
  EXPECT_EQ(*tree.lower_bound(2), 3);
  EXPECT_EQ(*tree.lower_bound(4), 5);
  EXPECT_EQ(*tree.lower_bound(6), 7);
  EXPECT_EQ(*tree.lower_bound(8), 9);
  
  // Edge cases
  EXPECT_EQ(*tree.lower_bound(0), 1);  // Before first
  EXPECT_EQ(*tree.lower_bound(1), 1);  // First element
  EXPECT_EQ(tree.lower_bound(10), tree.end());  // After last
}

TEST(AVLTreeBounds, LowerBoundConst) {
  const AVL_tree_t<int> tree{1, 3, 5, 7, 9};
  
  // Exact matches
  EXPECT_EQ(*tree.lower_bound(3), 3);
  EXPECT_EQ(*tree.lower_bound(5), 5);
  EXPECT_EQ(*tree.lower_bound(7), 7);
  
  // Values between elements
  EXPECT_EQ(*tree.lower_bound(2), 3);
  EXPECT_EQ(*tree.lower_bound(4), 5);
  EXPECT_EQ(*tree.lower_bound(6), 7);
  EXPECT_EQ(*tree.lower_bound(8), 9);
  
  // Edge cases
  EXPECT_EQ(*tree.lower_bound(0), 1);  // Before first
  EXPECT_EQ(*tree.lower_bound(1), 1);  // First element
  EXPECT_EQ(tree.lower_bound(10), tree.end());  // After last
}

TEST(AVLTreeBounds, UpperBoundBasic) {
  AVL_tree_t<int> tree{1, 3, 5, 7, 9};
  
  // Exact matches
  EXPECT_EQ(*tree.upper_bound(1), 3);
  EXPECT_EQ(*tree.upper_bound(3), 5);
  EXPECT_EQ(*tree.upper_bound(5), 7);
  EXPECT_EQ(*tree.upper_bound(7), 9);
  
  // Values between elements
  EXPECT_EQ(*tree.upper_bound(2), 3);
  EXPECT_EQ(*tree.upper_bound(4), 5);
  EXPECT_EQ(*tree.upper_bound(6), 7);
  EXPECT_EQ(*tree.upper_bound(8), 9);
  
  // Edge cases
  EXPECT_EQ(*tree.upper_bound(0), 1);  // Before first
  EXPECT_EQ(tree.upper_bound(9), tree.end());  // Last element
  EXPECT_EQ(tree.upper_bound(10), tree.end());  // After last
}

TEST(AVLTreeBounds, UpperBoundConst) {
  const AVL_tree_t<int> tree{1, 3, 5, 7, 9};
  
  // Exact matches
  EXPECT_EQ(*tree.upper_bound(1), 3);
  EXPECT_EQ(*tree.upper_bound(3), 5);
  EXPECT_EQ(*tree.upper_bound(5), 7);
  EXPECT_EQ(*tree.upper_bound(7), 9);
  
  // Values between elements
  EXPECT_EQ(*tree.upper_bound(2), 3);
  EXPECT_EQ(*tree.upper_bound(4), 5);
  EXPECT_EQ(*tree.upper_bound(6), 7);
  EXPECT_EQ(*tree.upper_bound(8), 9);
  
  // Edge cases
  EXPECT_EQ(*tree.upper_bound(0), 1);  // Before first
  EXPECT_EQ(tree.upper_bound(9), tree.end());  // Last element
  EXPECT_EQ(tree.upper_bound(10), tree.end());  // After last
}

TEST(AVLTreeBounds, LowerUpperBoundComparison) {
  AVL_tree_t<int> tree{1, 3, 5, 7, 9};
  
  // For values not in tree, lower_bound and upper_bound should be equal
  EXPECT_EQ(tree.lower_bound(2), tree.upper_bound(2));
  EXPECT_EQ(tree.lower_bound(4), tree.upper_bound(4));
  EXPECT_EQ(tree.lower_bound(6), tree.upper_bound(6));
  EXPECT_EQ(tree.lower_bound(8), tree.upper_bound(8));
  
  // For values in tree, lower_bound points to the element, upper_bound to next
  EXPECT_NE(tree.lower_bound(3), tree.upper_bound(3));
  EXPECT_EQ(*tree.lower_bound(3), 3);
  EXPECT_EQ(*tree.upper_bound(3), 5);
}

TEST(AVLTreeBounds, LowerBoundEmptyTree) {
  AVL_tree_t<int> tree;
  EXPECT_EQ(tree.lower_bound(42), tree.end());
}

TEST(AVLTreeBounds, UpperBoundEmptyTree) {
  AVL_tree_t<int> tree;
  EXPECT_EQ(tree.upper_bound(42), tree.end());
}

TEST(AVLTreeBounds, LowerBoundSingleElement) {
  AVL_tree_t<int> tree{5};
  
  EXPECT_EQ(*tree.lower_bound(3), 5);  // Before element
  EXPECT_EQ(*tree.lower_bound(5), 5);  // Exact match
  EXPECT_EQ(tree.lower_bound(7), tree.end());  // After element
}

TEST(AVLTreeBounds, UpperBoundSingleElement) {
  AVL_tree_t<int> tree{5};
  
  EXPECT_EQ(*tree.upper_bound(3), 5);  // Before element
  EXPECT_EQ(tree.upper_bound(5), tree.end());  // Exact match
  EXPECT_EQ(tree.upper_bound(7), tree.end());  // After element
}

TEST(AVLTreeBounds, LowerBoundDuplicates) {
  AVL_tree_t<int> tree{1, 2, 2, 2, 3, 4};
  
  // lower_bound should point to first occurrence
  auto lb = tree.lower_bound(2);
  ASSERT_NE(lb, tree.end());
  EXPECT_EQ(*lb, 2);
  
  // Verify it's the first occurrence by checking previous
  auto prev = lb;
  --prev;
  EXPECT_EQ(*prev, 1);
}

TEST(AVLTreeBounds, UpperBoundDuplicates) {
  AVL_tree_t<int> tree{1, 2, 2, 2, 3, 4};
  
  // upper_bound should point to element after the duplicates
  auto ub = tree.upper_bound(2);
  ASSERT_NE(ub, tree.end());
  EXPECT_EQ(*ub, 3);
}

TEST(AVLTreeBounds, LowerBoundCustomComparator) {
  AVL_tree_t<int, std::greater<int>> tree{9, 7, 5, 3, 1};  // Descending order
  
  // In descending order, lower_bound semantics change
  EXPECT_EQ(*tree.lower_bound(6), 5);  // Should find first element not less than 6
  EXPECT_EQ(*tree.lower_bound(5), 5);  // Exact match
  EXPECT_EQ(*tree.lower_bound(8), 7);  // Between 9 and 7
}

TEST(AVLTreeBounds, UpperBoundCustomComparator) {
  AVL_tree_t<int, std::greater<int>> tree{9, 7, 5, 3, 1};  // Descending order
  
  // In descending order, upper_bound semantics change
  EXPECT_EQ(*tree.upper_bound(6), 5);  // Should find first element greater than 6
  EXPECT_EQ(*tree.upper_bound(5), 3);  // Exact match - should point to next
  EXPECT_EQ(*tree.upper_bound(8), 7);  // Between 9 and 7
}

TEST(AVLTreeBounds, StringKeysLowerBound) {
  AVL_tree_t<std::string> tree{"apple", "banana", "cherry", "date"};
  
  EXPECT_EQ(*tree.lower_bound("banana"), "banana");
  EXPECT_EQ(*tree.lower_bound("berry"), "cherry");
  EXPECT_EQ(*tree.lower_bound("apricot"), "banana");
  EXPECT_EQ(tree.lower_bound("elderberry"), tree.end());
}

TEST(AVLTreeBounds, StringKeysUpperBound) {
  AVL_tree_t<std::string> tree{"apple", "banana", "cherry", "date"};
  
  EXPECT_EQ(*tree.upper_bound("banana"), "cherry");
  EXPECT_EQ(*tree.upper_bound("berry"), "cherry");
  EXPECT_EQ(*tree.upper_bound("apricot"), "banana");
  EXPECT_EQ(tree.upper_bound("date"), tree.end());
}

TEST(AVLTreeBounds, LargeTreeBounds) {
  AVL_tree_t<int> tree;
  
  // Insert 1000 elements
  for (int i = 0; i < 1000; i += 2) {
    tree.insert(i);  // Even numbers only: 0, 2, 4, ..., 998
  }
  
  // Test lower_bound on various values
  EXPECT_EQ(*tree.lower_bound(500), 500);  // Exact match
  EXPECT_EQ(*tree.lower_bound(501), 502);  // Between elements
  // EXPECT_EQ(*tree.lower_bound(999), 1000); // Should be end() since 1000 not inserted
  EXPECT_EQ(tree.lower_bound(1000), tree.end());
  
  // Test upper_bound on various values
  EXPECT_EQ(*tree.upper_bound(500), 502);  // Exact match
  EXPECT_EQ(*tree.upper_bound(501), 502);  // Between elements
  EXPECT_EQ(tree.upper_bound(998), tree.end());  // Last element
}

TEST(AVLTreeBounds, ConstNonConstConsistency) {
  AVL_tree_t<int> tree{1, 3, 5, 7, 9};
  const AVL_tree_t<int>& const_tree = tree;
  
  // Lower bound should give same results for const and non-const
  auto non_const_lb = tree.lower_bound(4);
  auto const_lb = const_tree.lower_bound(4);
  EXPECT_EQ(*non_const_lb, *const_lb);
  
  // Upper bound should give same results for const and non-const
  auto non_const_ub = tree.upper_bound(4);
  auto const_ub = const_tree.upper_bound(4);
  EXPECT_EQ(*non_const_ub, *const_ub);
}

TEST(AVLTreeBounds, IteratorValidity) {
  AVL_tree_t<int> tree{1, 3, 5, 7, 9};
  
  // Get iterators before modification
  auto lb_before = tree.lower_bound(4);
  auto ub_before = tree.upper_bound(4);
  
  // Insert new elements
  tree.insert(2);
  tree.insert(6);
  
  // Iterators should remain valid (assuming your implementation supports this)
  // This depends on your AVL tree implementation details
  EXPECT_EQ(*lb_before, 5);  // Should still point to 5
  EXPECT_EQ(*ub_before, 5);  // Should still point to 5
}
