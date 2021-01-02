#include "microbe.h"

#include "microbe/timer.h"

#include <vector>
#include <stdio.h>

namespace {
struct Benchmark {
  const char *name;
  void (*fn)();
};

std::vector<Benchmark>& accessBenchmarks() {
  static std::vector<Benchmark> benchmarks;
  return benchmarks;
}
} // namespace

namespace microbe {

BenchmarkRegistrar::BenchmarkRegistrar(const char *name, void (*fn)()) {
  accessBenchmarks().push_back(Benchmark{name, fn});
}

int runMain(int argc, char **argv) {
    selectTimer();

    for (const auto& bm : accessBenchmarks()) {
        printf("Running %s\n", bm.name);
    }
    return 0; 
 }

} // namespace microbe
