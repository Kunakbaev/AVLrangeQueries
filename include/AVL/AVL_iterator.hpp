#pragma once

#include <functional>

#include "AVL_tree_fwd.hpp"

template <typename KeyT, typename ComparatorT>
template <typename Ref>
AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::AVL_tree_iterator(
  TreeRef    tree,
  node_ind_t cur_node_ind
) : tree_(tree), cur_node_ind_(cur_node_ind) {}

template <typename KeyT, typename ComparatorT>
template <typename Ref>
auto AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::navigate(
  iterator_move_dir_t direction
) -> AVL_tree_iterator<Ref>& {
  bool is_forward_dir = direction == iterator_move_dir_t::kForward;
  auto node_kid_getter = [this, is_forward_dir](node_ind_t node_ind){
    const node_t& cur_node = tree_.get_node(node_ind);
    if (is_forward_dir) {
      return cur_node.right;
    } else {
      return cur_node.left;
    }
  };

  node_ind_t kid_node_ind = node_kid_getter(cur_node_ind_);
  if (kid_node_ind != kNullNodeInd) {
    // has right child - successor is leftmost of right subtree
    cur_node_ind_ = is_forward_dir
      ?  get_leftmost_node(tree_, kid_node_ind)
      : get_rightmost_node(tree_, kid_node_ind);
  } else {
    // no right child - go up until we come from left side
    node_ind_t parent = tree_.get_node(cur_node_ind_).parent;
    while (parent != kNullNodeInd && cur_node_ind_ == node_kid_getter(parent)) {
      cur_node_ind_ = parent;
      parent = tree_.get_node(cur_node_ind_).parent;
    }
    cur_node_ind_ = parent;
  }

  return *this;
}

template <typename KeyT, typename ComparatorT>
template <typename Ref>
auto AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::
    operator++() -> AVL_tree_iterator<Ref>& {
  if (*this == tree_.end()) {
    // technically this is UB:
    std::cerr << kIteratorAfterEndErrMsg << std::endl;
    assert(*this != tree_.end() && kIteratorAfterEndErrMsg.data() != nullptr);
    return *this;
  }

  return navigate(iterator_move_dir_t::kForward);
}

template <typename KeyT, typename ComparatorT>
template <typename Ref>
auto AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::
operator--() -> AVL_tree_iterator<Ref>& {
  if (*this == tree_.end()) {
    cur_node_ind_ = get_rightmost_node(tree_, tree_.root_node_ind_);
    return *this;
  }

  return navigate(iterator_move_dir_t::kBackward);
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::begin() -> iterator {
  return iterator{
    *this, iterator::get_leftmost_node(*this, root_node_ind_)
  };
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::begin() const -> const_iterator {
  return const_iterator{
    *this, const_iterator::get_leftmost_node(*this, root_node_ind_)
  };
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::end() -> iterator {
  return iterator{
    *this, kNullNodeInd
  };
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::end() const -> const_iterator {
  return const_iterator{
    *this, kNullNodeInd
  };
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::cbegin() const -> const_iterator {
  return begin();
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::cend()
    const -> const_iterator {
  return end();
}

template <typename KeyT, typename ComparatorT>
template <typename Ref>
std::size_t AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::
get_cnt_keys_less_or_eq() const {
  LOG_DEBUG_VARS(cur_node_ind_);
  if (cur_node_ind_ == kNullNodeInd) { // this is end() iterator
    return tree_.nodes_buffer_.size();
  }

  std::size_t cnt_keys_less_or_eq = 0;
  node_ind_t node_ind = cur_node_ind_;
  bool last_is_left_child = false;
  while (node_ind != kNullNodeInd) {
    node_ind_t left_son = tree_.get_node(node_ind).left;
    if (!last_is_left_child) {
      cnt_keys_less_or_eq += tree_.get_node_subtree_size(left_son);
      ++cnt_keys_less_or_eq;
    }
    last_is_left_child = false;

    node_ind_t parent = tree_.get_node(node_ind).parent;
    if (parent != kNullNodeInd &&
        tree_.get_node(parent).left == node_ind) {
      last_is_left_child = true;
    }
    node_ind = parent;
  }

  return cnt_keys_less_or_eq;
}

template <typename KeyT, typename ComparatorT>
template <typename Ref>
std::size_t AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::
    operator-(const AVL_tree_iterator& other) const {
  // assert(false && "Not implemented yet");

  std::size_t less_than_start  = other.get_cnt_keys_less_or_eq();
  std::size_t less_than_finish = get_cnt_keys_less_or_eq();
  if (less_than_finish > less_than_finish) {
    return 0;
  }

  return less_than_finish - less_than_start;
}

template <typename KeyT, typename ComparatorT>
template <typename Ref>
Ref AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::
    operator*() {
  // static bruh = tree_.get_node(cur_node_ind_);
  return tree_.get_node(cur_node_ind_).key;
}

template <typename KeyT, typename ComparatorT>
template <typename Ref>
bool AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::
    operator==(const AVL_tree_iterator& other) const {
  return cur_node_ind_ == other.cur_node_ind_ &&
         &tree_         == &other.tree_;
}


template <typename KeyT, typename ComparatorT>
template <typename Ref>
bool AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::
    operator!=(const AVL_tree_iterator& other) const {
  return !(*this == other);
}

// ================    PRIVATE METHODS   ========================

template <typename KeyT, typename ComparatorT>
template <typename Ref>
template <typename GetKidFunc>
auto AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::
get_extreme_node(
  node_ind_t        start_node_ind,
  GetKidFunc        get_kid_func
) -> node_ind_t {
  if (start_node_ind == kNullNodeInd) {
    return kNullNodeInd;
  }

  node_ind_t left_kid = kNullNodeInd;
  while ((left_kid = get_kid_func(start_node_ind)) != kNullNodeInd) {
    start_node_ind = left_kid;
  }

  return start_node_ind;
}

template <typename KeyT, typename ComparatorT>
template <typename Ref>
auto AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::
get_leftmost_node(
  TreeRef    tree,
  node_ind_t start_node_ind
) -> node_ind_t {
  return get_extreme_node(
    start_node_ind,
    [&tree](node_ind_t node_ind){ return tree.get_node(node_ind).left; }
  );
}

template <typename KeyT, typename ComparatorT>
template <typename Ref>
auto AVL_tree_t<KeyT, ComparatorT>::AVL_tree_iterator<Ref>::
get_rightmost_node(
  TreeRef    tree,
  node_ind_t start_node_ind
) -> node_ind_t {
  return get_extreme_node(
    start_node_ind,
    [&tree](node_ind_t node_ind){ return tree.get_node(node_ind).right; }
  );
}
