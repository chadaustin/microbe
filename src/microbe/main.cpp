#include "microbe.h"

#include "microbe/timer.h"

#include <stdio.h>
#include <memory>
#include <vector>

namespace {
[[noreturn]] void fatal(const char* msg) {
  fprintf(stderr, "fatal error: %s\n", msg);
  fflush(stderr);
  abort();
}

struct Current {
  static Current& init() {
    if (current_) {
      fatal("duplicate Current::init");
    }
    current_ = std::make_unique<Current>();
    return *current_;
  }

  static Current& get() {
    if (!current_) {
      fatal("missing Current::init");
    }
    return *current_;
  }

  uint64_t iterationCount = 0;

private:
  // C++17: std::optional
  static thread_local std::unique_ptr<Current> current_;
};

thread_local std::unique_ptr<Current> Current::current_;

struct Benchmark {
  const char *name;
  void (*fn)();

  void run() const {
    auto& current = Current::init();

    current.iterationCount = 10;
    fn();
  }
};

std::vector<Benchmark> &accessBenchmarks() {
  static std::vector<Benchmark> benchmarks;
  return benchmarks;
}
} // namespace

namespace microbe {

namespace detail {
uint64_t getIterationCount() {
  return Current::get().iterationCount;
}
} // namespace detail

BenchmarkRegistrar::BenchmarkRegistrar(const char *name, void (*fn)()) {
  accessBenchmarks().push_back(Benchmark{name, fn});
}

int runMain(int argc, char **argv) {
  selectTimer();

  for (const auto &bm : accessBenchmarks()) {
    printf("Running %s\n", bm.name);
    bm.run();
  }
  return 0;
}

} // namespace microbe
