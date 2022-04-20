
#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <vector>

auto _1_sort(std::vector<int> const& vec) {
  auto copy = vec;
  std::sort(copy.begin(), copy.end(), std::greater{});
  return std::pair{copy[0], copy[1]};
}

auto _2_partial_sort(std::vector<int> const& vec) {
  auto copy = vec;
  std::partial_sort(copy.begin(), copy.begin() + 2, copy.end(), std::greater{});
  return std::pair{copy[0], copy[1]};
}

auto _3_nth_element(std::vector<int> const& vec) {
  auto copy = vec;
  std::nth_element(copy.begin(), copy.begin() + 1, copy.end(), std::greater{});
  return std::pair{copy[0], copy[1]};
}

auto _4_max_element_erase(std::vector<int> const& vec) {
  auto copy = vec;
  auto it = std::max_element(copy.begin(), copy.end());
  auto const val = *it;
  copy.erase(it);
  return std::pair{val, *std::max_element(copy.begin(), copy.end())};
}

auto _5_max_element_swap(std::vector<int> const& vec) {
  auto copy = vec;
  auto it = std::max_element(copy.begin(), copy.end());
  auto const val = *it;
  std::iter_swap(it, std::prev(copy.end()));
  return std::pair{val, *std::max_element(copy.begin(), std::prev(copy.end()))};
}

constexpr auto MIN = std::numeric_limits<int>::min();

auto _6_accumulate(std::vector<int> const& vec) {
  return std::accumulate(vec.cbegin(), vec.cend(), std::pair{MIN, MIN},
                         [](std::pair<int, int> acc, int e) {
                           auto const [a, b] = acc;
                           if (e <= b) return acc;
                           if (e > a) return std::pair{e, a};
                           return std::pair{a, e};
                         });
}

auto _7_transform_reduce(std::vector<int> const& vec) {
  return std::transform_reduce(
      vec.cbegin(), vec.cend(), std::pair{MIN, MIN},
      [](auto l, auto r) {
        auto [a, b] = l;
        auto [c, d] = r;
        if (b >= c) return l;
        if (d >= a) return r;
        if (a >= c) return std::pair{a, c};
        return std::pair{c, a};
      },
      [](auto e) {
        return std::pair{e, MIN};
      });
}

auto average(std::vector<int> const& vec) {
  return std::accumulate(vec.begin(), vec.end(), 0) / vec.size();
}

class timer {
 public:
  timer() = default;

  void set(std::string const& name, int N) {
    current_function = name;
    current_N = N;
  }

  void start() { begin = std::chrono::steady_clock::now(); }

  void stop() {
    end = std::chrono::steady_clock::now();
    auto const diff =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)
            .count();
    times[current_function][current_N].push_back(diff);
  }

  // void print(char c) {
  //   for (auto [k, v] : times) {
  //     std::cout << k << c;
  //     std::cout << *std::min_element(v.begin(), v.end()) << c;
  //     std::cout << average(v) << c;
  //     std::cout << *std::max_element(v.begin(), v.end()) << '\n';
  //   }
  // }

  void print_for_excel() {
    for (auto [k, n_vec_map] : times) {
      std::cout << k << ',';
      // std::cout << n_vec_map.size() << '\n';
      for (auto [n, vec] : n_vec_map) {
        std::cout << average(vec) << ',';
      }
      std::cout << '\n';
    }
  }

 private:
  std::unordered_map<std::string, std::map<int, std::vector<int>>> times;
  std::string current_function;
  int current_N;
  std::chrono::steady_clock::time_point begin;
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
};

#define BENCHMARK(f, s)                                             \
  {                                                                 \
    t.set(s, N);                                                    \
    for (int i = 0; i < 100; i++) {                                 \
      std::vector<int> vec(N);                                      \
      std::iota(vec.begin(), vec.end(), 0);                         \
      std::shuffle(vec.begin(), vec.end(), std::mt19937{1337 * i}); \
      t.start();                                                    \
      auto [a, b] = f(vec);                                         \
      t.stop();                                                     \
      std::cout << a << ' ' << b << '\n';                           \
    }                                                               \
  }

auto main() -> int {
  timer t{};

  for (int N : {10, 100, 1000, 10000, 100000, 1000000}) {
    BENCHMARK(_1_sort, "sort")
    BENCHMARK(_2_partial_sort, "partial_sort")
    BENCHMARK(_3_nth_element, "nth_element")
    BENCHMARK(_4_max_element_erase, "max_element_erase")
    BENCHMARK(_5_max_element_swap, "max_element_swap")
    BENCHMARK(_6_accumulate, "accumulate")
    BENCHMARK(_7_transform_reduce, "transform_reduce")
    std::cout << N << '\n';
  }
  // t.print(',');

  t.print_for_excel();

  return 0;
}
