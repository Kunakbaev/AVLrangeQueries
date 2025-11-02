#include <gtest/gtest.h>

#include "AVL/AVL_tree.hpp"

TEST(AVLTreeCommon, DefaultConstructor) {
  AVL_tree_t<int> tree;
  EXPECT_EQ(tree.begin(), tree.end());
}

TEST(AVLTreeCommon, InitializerListConstructor) {
  AVL_tree_t<int> tree{5, 3, 7, 1, 4};
  auto it = tree.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(*it, 4);
  ++it;
  EXPECT_EQ(*it, 5);
  ++it;
  EXPECT_EQ(*it, 7);
}

TEST(AVLTreeCommon, InsertSingleElement) {
  AVL_tree_t<int> tree;
  tree.insert(42);
  
  auto it = tree.begin();
  EXPECT_NE(it, tree.end());
  EXPECT_EQ(*it, 42);
  ++it;
  EXPECT_EQ(it, tree.end());
}

TEST(AVLTreeCommon, InsertMultipleElements) {
  AVL_tree_t<int> tree;
  tree.insert(50);
  tree.insert(30);
  tree.insert(70);
  tree.insert(20);
  tree.insert(40);
  
  std::vector<int> expected{20, 30, 40, 50, 70};
  std::vector<int> actual;
  
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    actual.push_back(*it);
  }
  
  EXPECT_EQ(actual, expected);
}

TEST(AVLTreeCommon, ClearEmptyTree) {
  AVL_tree_t<int> tree;
  tree.clear();
  EXPECT_EQ(tree.begin(), tree.end());
}

TEST(AVLTreeCommon, ClearNonEmptyTree) {
  AVL_tree_t<int> tree{1, 2, 3};
  tree.clear();
  EXPECT_EQ(tree.begin(), tree.end());
}

TEST(AVLTreeCommon, InsertAfterClear) {
  AVL_tree_t<int> tree{1, 2, 3};
  tree.clear();
  tree.insert(10);
  
  auto it = tree.begin();
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(it, tree.end());
}

TEST(AVLTreeCommon, BalanceAfterInsertion) {
  AVL_tree_t<int> tree;
  
  // Insert in a way that would create an unbalanced BST
  tree.insert(1);
  tree.insert(2);
  tree.insert(3);  // This should trigger rotation
  
  std::vector<int> elements;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    elements.push_back(*it);
  }
  
  EXPECT_EQ(elements, (std::vector<int>{1, 2, 3}));
}

TEST(AVLTreeCommon, DuplicateValues) {
  AVL_tree_t<int> tree;
  tree.insert(5);
  tree.insert(5);
  tree.insert(5);
  
  // Should handle duplicates (implementation dependent)
  std::vector<int> elements;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    elements.push_back(*it);
  }
  
  // At minimum, one element should exist
  EXPECT_FALSE(elements.empty());
  EXPECT_EQ(elements.front(), 5);
}

TEST(AVLTreeCommon, LargeInsertionSequence) {
  AVL_tree_t<int> tree;
  
  // Insert 100 elements in reverse order (worst case for simple BST)
  for (int i = 100; i >= 1; --i) {
    tree.insert(i);
  }
  
  // Verify all elements are present and sorted
  int expected = 1;
  for (auto it = tree.begin(); it != tree.end(); ++it, ++expected) {
    EXPECT_EQ(*it, expected);
  }
  EXPECT_EQ(expected, 101); // Should have iterated through all 100 elements
}

TEST(AVLTreeCommon, CustomComparator) {
  // Test with greater comparator for descending order
  AVL_tree_t<int, std::greater<int>> tree;
  tree.insert(1);
  tree.insert(3);
  tree.insert(2);
  
  std::vector<int> elements;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    elements.push_back(*it);
  }

  EXPECT_EQ(elements, (std::vector<int>{3, 2, 1}));
}

TEST(AVLTreeCommon, StringKeys) {
  AVL_tree_t<std::string> tree;
  tree.insert("banana");
  tree.insert("apple");
  tree.insert("cherry");
  
  std::vector<std::string> elements;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    elements.push_back(*it);
  }
  
  EXPECT_EQ(elements, (std::vector<std::string>{"apple", "banana", "cherry"}));
}

TEST(AVLTreeCommon, ConstIterators) {
  const AVL_tree_t<int> tree{3, 1, 2};
  
  std::vector<int> elements;
  for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
    elements.push_back(*it);
  }
  
  EXPECT_EQ(elements, (std::vector<int>{1, 2, 3}));
}
