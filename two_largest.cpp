
#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <vector>
#include <benchmark/benchmark.h>

struct fixture : public benchmark::Fixture
{

  void SetUp(benchmark::State const &state)
  {
    input = std::vector<int>(state.range(0));
    std::iota(input.begin(), input.end(), 0);
    std::shuffle(input.begin(), input.end(), std::mt19937{1337});
  }

  void TearDown(benchmark::State const &state) {}

  std::vector<int> input;
};

auto _1_sort(std::vector<int> const &vec)
{
  auto copy = vec;
  std::sort(copy.begin(), copy.end(), std::greater{});
  return std::pair{copy[0], copy[1]};
}

auto _2_partial_sort(std::vector<int> const &vec)
{
  auto copy = vec;
  std::partial_sort(copy.begin(), copy.begin() + 2, copy.end(), std::greater{});
  return std::pair{copy[0], copy[1]};
}

auto _3_nth_element(std::vector<int> const &vec)
{
  auto copy = vec;
  std::nth_element(copy.begin(), copy.begin() + 1, copy.end(), std::greater{});
  return std::pair{copy[0], copy[1]};
}

auto _4_max_element_erase(std::vector<int> const &vec)
{
  auto copy = vec;
  auto it = std::max_element(copy.begin(), copy.end());
  auto const val = *it;
  copy.erase(it);
  return std::pair{val, *std::max_element(copy.begin(), copy.end())};
}

auto _5_max_element_swap(std::vector<int> const &vec)
{
  auto copy = vec;
  auto it = std::max_element(copy.begin(), copy.end());
  auto const val = *it;
  std::iter_swap(it, std::prev(copy.end()));
  return std::pair{val, *std::max_element(copy.begin(), std::prev(copy.end()))};
}

constexpr auto MIN = std::numeric_limits<int>::min();

auto _6_accumulate(std::vector<int> const &vec)
{
  return std::accumulate(vec.cbegin(), vec.cend(), std::pair{MIN, MIN},
                         [](std::pair<int, int> acc, int e)
                         {
                           auto const [a, b] = acc;
                           if (e <= b)
                             return acc;
                           if (e > a)
                             return std::pair{e, a};
                           return std::pair{a, e};
                         });
}

auto _7_transform_reduce(std::vector<int> const &vec)
{
  return std::transform_reduce(
      vec.cbegin(), vec.cend(), std::pair{MIN, MIN},
      [](auto l, auto r)
      {
        auto [a, b] = l;
        auto [c, d] = r;
        if (b >= c)
          return l;
        if (d >= a)
          return r;
        if (a >= c)
          return std::pair{a, c};
        return std::pair{c, a};
      },
      [](auto e)
      {
        return std::pair{e, MIN};
      });
}

#define BENCH(name)                                \
  BENCHMARK_DEFINE_F(fixture, name)                \
  (benchmark::State & state)                       \
  {                                                \
    for (auto _ : state)                           \
    {                                              \
      benchmark::DoNotOptimize(name(this->input)); \
    }                                              \
  }                                                \
  BENCHMARK_REGISTER_F(fixture, name)->RangeMultiplier(10)->Range(10, 1'000'000);

BENCH(_1_sort);
BENCH(_2_partial_sort);
BENCH(_3_nth_element);
BENCH(_4_max_element_erase);
BENCH(_5_max_element_swap);
BENCH(_6_accumulate);
BENCH(_7_transform_reduce);
