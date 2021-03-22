#ifndef INCLUDE_MICROBE_H
#define INCLUDE_MICROBE_H

#include <stddef.h>
#include <stdint.h>

#define MICROBE(name)                                                          \
  static void benchmark_##name();                                              \
  static ::microbe::BenchmarkRegistrar benchmark_registrar_##name{             \
      #name, &benchmark_##name};                                               \
  void benchmark_##name()

#define MICROBE_MAIN()                                                         \
  int main(int argc, char **argv) { return ::microbe::runMain(argc, argv); }   \
  int main(int argc, char **argv)

namespace microbe {
template <typename T> void doNotOptimize(T &&) {
  // TODO:
}

namespace detail {
uint64_t getIterationCount();
}

class loop {
public:
  class Iterator {
  public:
    struct Nothing {};

    Iterator() = default;
    explicit Iterator(uint64_t count) : iter_{count} {}

    bool operator!=(const Iterator &that) const { return iter_ != that.iter_; }

    Iterator &operator++() {
      ++iter_;
      return *this;
    }

    Nothing operator*() {
      return Nothing{};
    }

  private:
    uint64_t iter_ = 0;
  };

  loop() : count_{detail::getIterationCount()} {}

  Iterator begin() const { return Iterator{}; }
  // In C++17, this can have a different type.
  Iterator end() const { return Iterator{count_}; }

private:
  loop(const loop &) = delete;
  loop &operator=(const loop &) = delete;

  uint64_t count_;
};

class BenchmarkRegistrar {
public:
  BenchmarkRegistrar(const char *name, void (*fn)());
};

int runMain(int argc, char **argv);

} // namespace microbe

#endif
