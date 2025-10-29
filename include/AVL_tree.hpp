#pragma once

#include <cassert>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <string>
#include <string_view>
#include <vector>

#include "AVL_tree_fwd.hpp"
#include "logLib.hpp"

template <typename KeyT = std::int64_t,
          typename ComparatorT = std::less<KeyT>>
class AVL_tree_t {
 public:
  using value_type = KeyT;
  using   key_type = KeyT;
  static const std::size_t size_type = sizeof(KeyT);

 private:
  class node_t; // forward declaration of class node_t
  using node_ind_t = std::size_t;

 public:
  template <typename Ref>
  class AVL_tree_iterator;
  using       iterator = AVL_tree_iterator<KeyT&>;
  using const_iterator = AVL_tree_iterator<const KeyT&>;

  template <typename Ref>
  class AVL_tree_iterator {
    friend class AVL_tree_t;

   private:
    using TreeRef = std::conditional_t<
        std::is_const_v<std::remove_reference_t<Ref>>,
        const AVL_tree_t&,
        AVL_tree_t&
    >;

   public:
    // AVL_tree_iterator() = default;

    explicit AVL_tree_iterator(TreeRef tree, node_ind_t node_ind);

    // prefix ++, e.g. ++it
    AVL_tree_iterator& operator++();

    // prefix --
    AVL_tree_iterator& operator--();

    std::size_t get_cnt_keys_less_or_eq() const;

    std::size_t operator-(const AVL_tree_iterator& other) const;

    Ref operator*();

    bool operator==(const AVL_tree_iterator& other) const;

    bool operator!=(const AVL_tree_iterator& other) const;

   private:
    enum class iterator_move_dir_t {
      kForward,
      kBackward
    };

    node_ind_t get_target_subtree_for_navigate(
      iterator_move_dir_t direction,
      node_ind_t&         child_node_ind
    );

    AVL_tree_iterator<Ref>& navigate(iterator_move_dir_t direction);

    template <typename GetKidFunc>
    static node_ind_t get_extreme_node(
      node_ind_t        start_node_ind,
      GetKidFunc        get_kid_fund
    );

    static node_ind_t get_leftmost_node(TreeRef tree, node_ind_t start_node_ind);

    static node_ind_t get_rightmost_node(TreeRef tree, node_ind_t start_node_ind);

   private:
    const std::string_view kIteratorBeforeBeginErrMsg =
      "Error: operator()-- called on .begin() iterator...";
    const std::string_view kIteratorAfterEndErrMsg =
      "Error: operator()++ called on .end() iterator...";

   private:
    TreeRef tree_;
    node_ind_t cur_node_ind_;
    // const auto get_left_node_  = [tree_](node_ind_t node_ind){ return tree_.get_node(node_ind).left; };
    // const auto get_right_node_ = [tree_](node_ind_t node_ind){ return tree_.get_node(node_ind).right; };
  };

 public:
  AVL_tree_t() = default;

  AVL_tree_t(const std::initializer_list<KeyT>& init_list);

  void clear();

  void insert(const KeyT& new_key);

  const_iterator begin() const;

  iterator begin();

  const_iterator end() const;

  iterator end();

  const_iterator cbegin() const;

  const_iterator cend() const;

  // ================ visualization method ===========
#ifdef DEBUG_
  void visualize_tree(
    const std::string& base_filename
  ) const;
#endif

 private:
  using node_height_t = std::int8_t;

  [[nodiscard]] node_ind_t get_new_node(const KeyT& key);

  [[nodiscard]] node_ind_t add_new_key_recursively(
    node_ind_t cur_node, const KeyT& new_key
  );

  [[nodiscard]] node_ind_t rotate_node_right(node_ind_t node_ind);

  [[nodiscard]] node_ind_t rotate_node_left(node_ind_t node_ind);

  [[nodiscard]] node_ind_t balance_node(node_ind_t node_ind);

  void recalc_node_height_and_size(node_ind_t node_ind);

  [[nodiscard]] node_height_t get_node_balance(node_ind_t node_ind) const;

  [[nodiscard]] node_height_t get_node_height(node_ind_t node_ind) const;

  [[nodiscard]] std::size_t get_node_subtree_size(node_ind_t node_ind) const;

  void set_node_right_son(node_ind_t parent_ind, node_ind_t kid_ind);

  void set_node_left_son (node_ind_t parent_ind, node_ind_t kid_ind);

  [[nodiscard]] node_t& get_node(node_ind_t node_ind);

  [[nodiscard]] const node_t& get_node(node_ind_t node_ind) const;

  // ================ visualization methods ===========
#ifdef DEBUG_
  void generate_dot(const std::string dot_filename) const;

  void generate_dot_recursive(
    node_ind_t     cur_node_ind,
    node_ind_t     parent,
    std::ofstream& file_stream
  ) const;

  void generate_dot_for_edge(
    node_ind_t     start,
    node_ind_t     finish,
    std::ofstream& file_stream
  ) const;

  void generate_dot_for_vertex(node_ind_t cur_node_ind, std::ofstream& file_stream) const;

  static std::string get_node_ind_name(node_ind_t node_ind);

  void open_png_file(const std::string& filename) const;
#endif

 private:
  static const node_ind_t kNullNodeInd = 0;

 private:
  class node_t {
   public:
    KeyT          key{};
    node_ind_t    left{};
    node_ind_t    right{};
    node_ind_t    parent{};
    node_height_t height{};
    std::size_t   subtree_size{};

    node_t() = default;
    node_t(const KeyT& key)
      : key(key), left(kNullNodeInd), right(kNullNodeInd),
        parent(kNullNodeInd), height(1), subtree_size(1) {}
  };

  ComparatorT         comparator_;
  node_ind_t          root_node_ind_ = kNullNodeInd;
  std::vector<node_t> nodes_buffer_ = {{}}; // 0 indexed is occupied by garbage, so nodes have indices >= 1
};

template <typename KeyT, typename ComparatorT>
AVL_tree_t<KeyT, ComparatorT>::AVL_tree_t(const std::initializer_list<KeyT>& init_list) {
  std::size_t iter_num = 1;
  for (auto it = init_list.begin(); it != init_list.end(); ++it) {
    insert(*it);
#ifdef DEBUG_
    // visualize_tree("avl_tree_vizualized" + std::to_string(iter_num++));
#endif
  }
}

template <typename KeyT, typename ComparatorT>
void AVL_tree_t<KeyT, ComparatorT>::insert(const KeyT& new_key) {
#if 0
  LOG_DEBUG_VARS((size_t)new_key);
#endif
  root_node_ind_ = add_new_key_recursively(root_node_ind_, new_key);
  LOG_DEBUG_VARS(root_node_ind_);
}

template <typename KeyT, typename ComparatorT>
void AVL_tree_t<KeyT, ComparatorT>::clear() {
  nodes_buffer_ = {{}};
  root_node_ind_ = kNullNodeInd;
}

// ======================   private methods   ========================

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::add_new_key_recursively(
  node_ind_t cur_node_ind, const KeyT& new_key
) -> node_ind_t {
  if (cur_node_ind == kNullNodeInd) {
    node_ind_t new_node_ind = get_new_node(new_key);
    return new_node_ind;
  }

  node_t& cur_node = nodes_buffer_[cur_node_ind];
  if (comparator_(cur_node.key, new_key)) {
    node_ind_t new_right_son = add_new_key_recursively(cur_node.right, new_key);
    set_node_right_son(cur_node_ind, new_right_son);
  } else if (comparator_(new_key, cur_node.key)) {
    node_ind_t new_left_son  = add_new_key_recursively(cur_node.left,  new_key);
    set_node_left_son(cur_node_ind, new_left_son);
  } else { // new_key has already been inserted before
    LOG_DEBUG_VARS((size_t)new_key, "duplicate");
    return cur_node_ind;
  }
#if 0
  LOG_DEBUG_VARS(cur_node_ind, cur_node.left, cur_node.right);
#endif

  cur_node_ind = balance_node(cur_node_ind);
  return cur_node_ind;
}

template <typename KeyT, typename ComparatorT>
[[nodiscard]] auto AVL_tree_t<KeyT, ComparatorT>::balance_node(
  node_ind_t node_ind
) -> node_ind_t {
  if (node_ind == kNullNodeInd) {
    return kNullNodeInd;
  }

  node_height_t balance = get_node_balance(node_ind);
#if 0
  LOG_DEBUG_VARS(balance, node_ind, 
    get_node_height(get_node(node_ind).left), get_node_height(get_node(node_ind).right));
#endif
  if (std::abs(balance) <= 1) { // node is already balanced
    return node_ind;
  }

  node_t& node = get_node(node_ind);
#if 0
  LOG_DEBUG_VARS("balancing", balance, node_ind, node.left, node.right);
#endif
  if (balance == 2) {
    node_height_t right_node_balance = get_node_balance(node.right);
    if (right_node_balance < 0) {
      node_ind_t new_right = rotate_node_right(node.right);
      set_node_right_son(node_ind, new_right);
    }

    return rotate_node_left(node_ind);
  }

  // ASK: is this copypaste? It's different, but mirrored
#ifdef DEBUG_
  if (balance != -2) {
    LOG_DEBUG_VARS(node_ind, root_node_ind_);
    visualize_tree("avl_tree_vizualized");
  }
#endif
  assert(balance == -2);
  node_height_t left_node_balance = get_node_balance(node.left);
  if (left_node_balance > 0) {
    node_ind_t new_left = rotate_node_left(node.left);
    set_node_left_son(node_ind, new_left);
  }

  return rotate_node_right(node_ind);
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::rotate_node_right(
  node_ind_t cur_node_ind
) -> node_ind_t {
  if (cur_node_ind == kNullNodeInd) {
    return kNullNodeInd;
  }
  node_t& cur_node = get_node(cur_node_ind);
  assert(cur_node.left != kNullNodeInd);

  const node_ind_t kNewSubtreeRootInd = cur_node.left;
  node_ind_t new_root_right_son = get_node(kNewSubtreeRootInd).right;

  set_node_left_son(cur_node_ind, new_root_right_son);
  set_node_right_son(kNewSubtreeRootInd, cur_node_ind);

  return kNewSubtreeRootInd;
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::rotate_node_left(
  node_ind_t cur_node_ind
) -> node_ind_t {
  if (cur_node_ind == kNullNodeInd) {
    return kNullNodeInd;
  }
  node_t& cur_node = get_node(cur_node_ind);
  assert(cur_node.right != kNullNodeInd);

  const node_ind_t kNewSubtreeRootInd = cur_node.right;
  node_ind_t new_root_left_son = get_node(kNewSubtreeRootInd).left;

#if 0
  // LOG_DEBUG_VARS(kNewSubtreeRootInd, new_root_left_son);
  // visualize_tree("avl_tree_vizualiade_22");
#endif
  set_node_right_son(cur_node_ind, new_root_left_son);
  set_node_left_son(kNewSubtreeRootInd, cur_node_ind);

  return kNewSubtreeRootInd;
}

template <typename KeyT, typename ComparatorT>
void AVL_tree_t<KeyT, ComparatorT>::recalc_node_height_and_size(node_ind_t node_ind) {
  node_t& cur_node = get_node(node_ind);
  node_height_t  left_height     = get_node_height(cur_node.left);
  node_height_t right_height     = get_node_height(cur_node.right);
  std::size_t  left_subtree_size = get_node_subtree_size(cur_node.left);
  std::size_t right_subtree_size = get_node_subtree_size(cur_node.right);

  cur_node.height = std::max(left_height, right_height) + 1;
  cur_node.subtree_size = left_subtree_size + 1 + right_subtree_size;
}

template <typename KeyT, typename ComparatorT>
[[nodiscard]] auto AVL_tree_t<KeyT, ComparatorT>::get_node_balance(
  node_ind_t node_ind
) const -> node_height_t {
  const node_t& cur_node = get_node(node_ind);
  return get_node_height(cur_node.right) -
         get_node_height(cur_node.left);
}

template <typename KeyT, typename ComparatorT>
[[nodiscard]] typename AVL_tree_t<KeyT, ComparatorT>::node_height_t
AVL_tree_t<KeyT, ComparatorT>::get_node_height(node_ind_t node_ind) const {
  return node_ind != kNullNodeInd
            ? get_node(node_ind).height
            : 0;
}

template <typename KeyT, typename ComparatorT>
[[nodiscard]] std::size_t AVL_tree_t<KeyT, ComparatorT>::
get_node_subtree_size(node_ind_t node_ind) const {
  return node_ind != kNullNodeInd
            ? get_node(node_ind).subtree_size
            : 0;
}

template <typename KeyT, typename ComparatorT>
void AVL_tree_t<KeyT, ComparatorT>::set_node_right_son(
  node_ind_t parent_ind,
  node_ind_t kid_ind
) {
  get_node(parent_ind).right = kid_ind;
  // WARNING: do it work?
  get_node(parent_ind).parent = kNullNodeInd;
  if (kid_ind != kNullNodeInd) {
    get_node(kid_ind).parent = parent_ind;
  }

  recalc_node_height_and_size(parent_ind);
}

// ASK: copypaste?
template <typename KeyT, typename ComparatorT>
void AVL_tree_t<KeyT, ComparatorT>::set_node_left_son(
  node_ind_t parent_ind,
  node_ind_t kid_ind
) {
  get_node(parent_ind).left = kid_ind;
  // WARNING: do it work?
  get_node(parent_ind).parent = kNullNodeInd;
  if (kid_ind != kNullNodeInd) {
    get_node(kid_ind).parent = parent_ind;
  }

  recalc_node_height_and_size(parent_ind);
}

// ASK: ok? How not to write this every time: AVL_tree_t<KeyT, ComparatorT>?
template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::get_node(node_ind_t node_ind) -> node_t& {
  assert(node_ind != kNullNodeInd);

  return nodes_buffer_[node_ind];
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::get_node(node_ind_t node_ind) const -> const node_t& {
  assert(node_ind != kNullNodeInd);

  return nodes_buffer_[node_ind];
}

template <typename KeyT, typename ComparatorT>
auto AVL_tree_t<KeyT, ComparatorT>::get_new_node(
  const KeyT& key
) -> node_ind_t {
  nodes_buffer_.emplace_back(key);
  return nodes_buffer_.size() - 1;
}

#ifdef DEBUG_
#include "AVL_vizualization.hpp"
#endif

#include "AVL_iterator.hpp"
