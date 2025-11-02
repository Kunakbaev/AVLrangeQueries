#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "AVL_tree_fwd.hpp"

template <typename KeyT, typename ComparatorT>
void AVL_tree_t<KeyT, ComparatorT>::visualize_tree(
  const std::string& base_filename
) const {
  const std::string dot_filename = base_filename + ".dot";
  const std::string png_filename = base_filename + ".png";

  generate_dot(dot_filename);

  // WARNING: be careful with ';' in filenames
  std::string png_gen_command =
    "dot -Gmargin=0 -Gpad=0.1 -Tpng "
    + dot_filename + " -o " + png_filename;
  int status = system(png_gen_command.c_str());
  if (status == -1) {
    perror("system command failed, couldn't use dot utility...");
  } else {
    std::cerr << "System command failed, couldn't use dot utility, exited with code: " << status << std::endl;
  }

  open_png_file(png_filename);
}

template <typename KeyT, typename ComparatorT>
void AVL_tree_t<KeyT, ComparatorT>::generate_dot(
  const std::string dot_filename
) const {
  std::ofstream file_stream(dot_filename);
  file_stream <<
    "digraph AVLtree {\n"
    "pad     = 0.1;\n"
    "margin  = 0;\n"
    "overlap = false\n"
    "bgcolor = \"black\"\n"
    "rankdir = TB\n"
    "node [style=\"rounded,filled\","
    "fillcolor=white, margin=0, pendwidth=\"3%%\","
    "radius=0.25, shape=rectangle, margin=\"0.2,0.2\","
    "fontcolor=black;]\n";
  generate_dot_recursive(root_node_ind_, kNullNodeInd, file_stream);
  file_stream << "}\n";
}

template <typename KeyT, typename ComparatorT>
void AVL_tree_t<KeyT, ComparatorT>::generate_dot_recursive(
  node_ind_t     cur_node_ind,
  node_ind_t     parent_node_ind,
  std::ofstream& file_stream
) const {
  #if 0
  LOG_DEBUG_VARS(cur_node_ind, parent_node_ind);
  #endif
  if (cur_node_ind == kNullNodeInd) {
    return;
  }

  if (parent_node_ind != kNullNodeInd) {
    generate_dot_for_edge(parent_node_ind, cur_node_ind, file_stream);
  }

  generate_dot_for_vertex(cur_node_ind, file_stream);

  const node_t& cur_node = get_node(cur_node_ind);
  generate_dot_recursive(cur_node.left,  cur_node_ind, file_stream);
  generate_dot_recursive(cur_node.right, cur_node_ind, file_stream);
}

template <typename KeyT, typename ComparatorT>
void AVL_tree_t<KeyT, ComparatorT>::open_png_file(
  const std::string& png_filename
) const {
  // TODO: I hope this works fine, however I don't want to think about non linux systems for now
  int status = 0;
  #ifdef _WIN32
  status = system(("start " + png_filename).c_str());
  #elif __APPLE__
  status = system(("open " + png_filename).c_str());
  #else
  status = system(("xdg-open " + png_filename).c_str());
  #endif

  if (status == -1) {
    perror("system command failed, couldn't open image...");
  } else {
    std::cerr << "System command failed, couldn't open image, exited with code: " << status << std::endl;
  }
}

template <typename KeyT, typename ComparatorT>
void AVL_tree_t<KeyT, ComparatorT>::generate_dot_for_edge(
  node_ind_t     start,
  node_ind_t     finish,
  std::ofstream& file_stream
) const {
  bool is_left_son = get_node(start).left == finish;
  const std::string kEdgeColor = is_left_son ? "orange" : "lightblue";
  file_stream << start << " -> " << finish
    << " [color=" << kEdgeColor << ", fontcolor=white, weight=1]";
}

template <typename KeyT, typename ComparatorT>
std::string AVL_tree_t<KeyT, ComparatorT>::get_node_ind_name(
  node_ind_t node_ind
) {
  return node_ind == kNullNodeInd ? "nil" : std::to_string(node_ind);
}

template <typename KeyT, typename ComparatorT>
void AVL_tree_t<KeyT, ComparatorT>::generate_dot_for_vertex(
  node_ind_t     cur_node_ind,
  std::ofstream& file_stream
) const {
  const node_t& cur_node = get_node(cur_node_ind);
  const std::string  kLeftKidName = get_node_ind_name(cur_node.left);
  const std::string kRightKidName = get_node_ind_name(cur_node.right);
  file_stream << cur_node_ind << " [label=\"key: " << cur_node.key
              << ", h: " << static_cast<int>(cur_node.height)
              << "\nid: " << cur_node_ind
              << ", l: " << kLeftKidName << ", r: " << kRightKidName << "\"];\n";
}

