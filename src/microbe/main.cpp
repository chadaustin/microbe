#include "microbe.h"

#include "microbe/timer.h"

#include <vector>
#include <stdio.h>

namespace {
struct Benchmark {
  const char *name;
  void (*fn)();
};

std::vector<Benchmark> g_benchmarks;
} // namespace

namespace microbe {

BenchmarkRegistrar::BenchmarkRegistrar(const char *name, void (*fn)()) {
  printf("in registrar %s\n", name);
  g_benchmarks.push_back(Benchmark{name, fn});
    printf("size: %d\n", (int)g_benchmarks.size());
}

int runMain(int argc, char **argv) {
    printf("in runmain\n");

    selectTimer();

    printf("size: %d\n", (int)g_benchmarks.size());
    for (const auto& bm : g_benchmarks) {
        printf("Running %s\n", bm.name);
    }
    return 0; 
 }

} // namespace microbe
