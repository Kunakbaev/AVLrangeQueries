#pragma once

#include "AVL_tree_fwd.hpp"

template <typename KeyT, typename ComparatorT>
[[nodiscard]] auto AVL_tree_t<KeyT, ComparatorT>::
general_find_node_after_key(KeyT key, bool skip_equal) const -> node_ind_t {
  if (!root_node_ind_) { // tree is empty
    return kNullNodeInd;
  }

  node_ind_t candidate_ind = kNullNodeInd;
  node_ind_t cur_node_ind = root_node_ind_;
  while (cur_node_ind != kNullNodeInd) {
    const node_t& cur_node = get_node(cur_node_ind);
    
    if (!skip_equal && cur_node.key == key) {
      // exact match and it's lower bound (no skip)
      return cur_node_ind;
    }
    
    if (comparator_(key, cur_node.key)) {
      // current node is greater than key - it's a potential candidate
      candidate_ind = cur_node_ind;
      // search left to find a closer (smaller but still >= key) candidate
      cur_node_ind = cur_node.left;
    } else {
      // current node is less than or equal to key
      // if equal and skip_equal is true, we need to go right
      // if less than, we need to go right to find larger values
      cur_node_ind = cur_node.right;
    }
  }
  
  return candidate_ind;
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::
lower_bound(KeyT key) -> iterator {
  node_ind_t ans_node_ind = general_find_node_after_key(key, false);
  return iterator(*this, ans_node_ind);
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::
upper_bound(KeyT key) -> iterator {
  node_ind_t ans_node_ind = general_find_node_after_key(key, true);
  return iterator(*this, ans_node_ind);
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::
lower_bound(KeyT key) const -> const_iterator {
  node_ind_t ans_node_ind = general_find_node_after_key(key, false);
  return const_iterator(*this, ans_node_ind);
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::
upper_bound(KeyT key) const -> const_iterator {
  node_ind_t ans_node_ind = general_find_node_after_key(key, true);
  return const_iterator(*this, ans_node_ind);
}
