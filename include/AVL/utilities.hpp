#pragma once

template <typename T>
static void do_not_optimize(const T& value) {
  asm volatile("" : : "r,m"(value) : "memory");
}
