#pragma once

#include <set>
#include <string_view>

#include "AVL/AVL_tree.hpp"
#include "AVL/utilities.hpp"

namespace solution {

struct avl_solution_tag {};
struct set_solution_tag {};

template <typename ContainerT, typename KeyT, typename solution_tag>
class solution_t {
 public:
  using const_iterator = typename ContainerT::const_iterator;

 private:
  enum class query_types_t {
    kInsert  = 'k',
    kQuery   = 'q',
    kInvalid = '?'
  };

 public:
  solution_t() = default;

  void solve() {
    char query_type_ch{};
    while (try_to_read(query_type_ch)) {
      ++query_index_;
      KeyT key{};
      bool success_read = false;
      query_types_t query_type = get_query_type(query_type_ch);
      switch (query_type) {
        case query_types_t::kInsert:
          success_read = insert_key();
          break;
        case query_types_t::kQuery:
          success_read = process_query();
          break;
        case query_types_t::kInvalid:
        default:
          std::cerr << kUnknownQueryType << "\n";
          success_read = false;
          break;
      }
      if (!success_read) {
        break;
      }
    }
    std::cout.flush(); // just in case
  }

 private:
  [[nodiscard]] static std::size_t get_my_distance_impl(
    const_iterator start, const_iterator finish, avl_solution_tag
  ) {
    return finish - start;
  }

  [[nodiscard]] static std::size_t get_my_distance_impl(
    const_iterator start, const_iterator finish, set_solution_tag
  ) {
    return std::distance(start, finish);
  }

  [[nodiscard]] static std::size_t get_my_distance(const_iterator start, const_iterator finish) {
    return get_my_distance_impl(start, finish, solution_tag{});
  }

  template <typename T>
  bool try_to_read(T& number) const {
    if (std::cin.eof()) {
#if 0
      std::cerr << kStdCinEOF << "\n";
      std::cerr << "Input fail at query with index : " << query_index_ << "\n";
#endif
      return false;
    }

    std::cin >> number;
    if (std::cin.fail()) {
    #if 0
      std::cerr << kStdCinFail << "\n";
      std::cerr << "Input fail at query with index : " << query_index_ << "\n";
    #endif
      return false;
    }

    return true;
  }

  bool insert_key() {
    KeyT key{};
    if (!try_to_read(key)) {
      return false;
    }

    container_.insert(key);
    return true;
  }

  bool process_query() const {
    KeyT low_key{};
    KeyT high_key{};
    
    if (!try_to_read(low_key) ||
        !try_to_read(high_key)) {
      return false;
    }

    const_iterator start = container_.lower_bound(low_key);
    const_iterator fin   = container_.upper_bound(high_key);
    std::size_t dist = get_my_distance(start, fin);

#ifndef TIME_MEASUREMENT_
    std::cout << dist << " ";
#else
    // I don't want compiler to optimize away computation of get_my_distance() method
    do_not_optimize(dist);
#endif

    return true;
  }

  query_types_t get_query_type(char query_type) {
    if (query_type == static_cast<char>(query_types_t::kInsert)) {
      return query_types_t::kInsert;
    }

    if (query_type == static_cast<char>(query_types_t::kQuery)) {
      return query_types_t::kQuery;
    }

    return query_types_t::kInvalid;
  }

 private:
  static constexpr std::string_view kStdCinFail       = "Error: invalid input...";
  static constexpr std::string_view kStdCinEOF        = "Error: unexpected end of input...";
  static constexpr std::string_view kUnknownQueryType = "Error: unknown query type...";

 private:
  ContainerT container_;
  std::size_t query_index_{};
};

}  // namespace solution
