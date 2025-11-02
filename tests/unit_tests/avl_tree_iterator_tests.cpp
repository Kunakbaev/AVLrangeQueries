#include <gtest/gtest.h>

#include "AVL/AVL_tree.hpp"

TEST(AVLTreeIterator, BeginEndEmptyTree) {
  AVL_tree_t<int> tree;
  EXPECT_EQ(tree.begin(), tree.end());
  EXPECT_EQ(tree.cbegin(), tree.cend());
}

TEST(AVLTreeIterator, BeginSingleElement) {
  AVL_tree_t<int> tree;
  tree.insert(42);
  
  auto begin_it = tree.begin();
  auto end_it = tree.end();
  
  EXPECT_NE(begin_it, end_it);
  EXPECT_EQ(*begin_it, 42);
  
  ++begin_it;
  EXPECT_EQ(begin_it, end_it);
}

TEST(AVLTreeIterator, PrefixIncrement) {
  AVL_tree_t<int> tree{1, 2, 3};
  auto it = tree.begin();
  
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(it, tree.end());
}

TEST(AVLTreeIterator, PrefixDecrement) {
  AVL_tree_t<int> tree{1, 2, 3};
  auto it = tree.begin();
  ++it; // points to 2
  ++it; // points to 3
  
  --it;
  EXPECT_EQ(*it, 2);
  --it;
  EXPECT_EQ(*it, 1);
}

TEST(AVLTreeIterator, IteratorDereference) {
  AVL_tree_t<int> tree;
  tree.insert(100);
  
  auto it = tree.begin();
  EXPECT_EQ(*it, 100);
  
  // Test that dereference returns reference
  *it = 200;
  EXPECT_EQ(*tree.begin(), 200);
}

TEST(AVLTreeIterator, ConstIteratorDereference) {
  const AVL_tree_t<int> tree{42};
  
  auto it = tree.cbegin();
  EXPECT_EQ(*it, 42);
  // *it = 100; // This should not compile for const_iterator
}

TEST(AVLTreeIterator, IteratorEquality) {
  AVL_tree_t<int> tree{1, 2};
  auto it1 = tree.begin();
  auto it2 = tree.begin();
  
  EXPECT_EQ(it1, it2);
  
  ++it1;
  EXPECT_NE(it1, it2);
  
  ++it2;
  EXPECT_EQ(it1, it2);
}

TEST(AVLTreeIterator, IteratorInequality) {
  AVL_tree_t<int> tree{1, 2, 3};
  auto it1 = tree.begin();
  auto it2 = tree.begin();
  
  EXPECT_FALSE(it1 != it2);
  
  ++it1;
  EXPECT_TRUE(it1 != it2);
}

TEST(AVLTreeIterator, FullCycleIncrement) {
  AVL_tree_t<int> tree{1, 2, 3, 4, 5};
  std::vector<int> collected;
  
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    collected.push_back(*it);
  }
  
  EXPECT_EQ(collected, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(AVLTreeIterator, FullCycleDecrement) {
  AVL_tree_t<int> tree{1, 2, 3, 4, 5};
  auto it = tree.end();
  
  std::vector<int> collected;
  while (it != tree.begin()) {
    --it;
    collected.push_back(*it);
  }
  
  EXPECT_EQ(collected, (std::vector<int>{5, 4, 3, 2, 1}));
}

TEST(AVLTreeIterator, IteratorDistance) {
  AVL_tree_t<int> tree{1, 2, 3, 4, 5};
  auto begin = tree.begin();
  auto end = tree.end();
  
  std::size_t distance = end - begin;
  EXPECT_EQ(distance, 5);
}

TEST(AVLTreeIterator, IteratorDistancePartial) {
  AVL_tree_t<int> tree{1, 2, 3, 4, 5};
  auto it1 = tree.begin();
  auto it2 = tree.begin();
  ++it2; ++it2; // it2 points to 3

  std::size_t distance = it2 - it1;
  EXPECT_EQ(distance, 2);
}

TEST(AVLTreeIterator, ReverseIteration) {
  AVL_tree_t<int> tree{1, 3, 2}; // Should be sorted as 1, 2, 3
  auto it = tree.end();
  
  --it;
  EXPECT_EQ(*it, 3);
  --it;
  EXPECT_EQ(*it, 2);
  --it;
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(it, tree.begin());
}

TEST(AVLTreeIterator, BeginAfterModification) {
  AVL_tree_t<int> tree;
  tree.insert(10);
  
  auto begin1 = tree.begin();
  tree.insert(5); // This might rebalance the tree
  
  auto begin2 = tree.begin();
  EXPECT_EQ(*begin2, 5); // New smallest element
}

TEST(AVLTreeIterator, IteratorInvalidation) {
  AVL_tree_t<int> tree{1, 2, 3};
  auto it = tree.begin();
  ++it; // points to 2
  
  tree.clear(); // This should invalidate iterators
  
  // Behavior after invalidation is implementation defined
  // The test documents the current behavior
}

TEST(AVLTreeIterator, ComplexTreeStructure) {
  // Create a tree that requires multiple rotations
  AVL_tree_t<int> tree;
  tree.insert(50);
  tree.insert(25);
  tree.insert(75);
  tree.insert(10);
  tree.insert(30);
  tree.insert(60);
  tree.insert(80);
  tree.insert(5);
  tree.insert(15);
  tree.insert(27);
  tree.insert(55);
  
  std::vector<int> elements;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    elements.push_back(*it);
  }
  
  // Verify sorted order
  for (size_t i = 1; i < elements.size(); ++i) {
    EXPECT_LT(elements[i-1], elements[i]);
  }
  
  // Verify all elements are present
  std::vector<int> expected{5, 10, 15, 25, 27, 30, 50, 55, 60, 75, 80};
  EXPECT_EQ(elements, expected);
}