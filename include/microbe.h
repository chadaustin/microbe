#ifndef INCLUDE_MICROBE_H
#define INCLUDE_MICROBE_H

#define MICROBM(name)                                                          \
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

class IterationLoop {
public:
  int *begin() const { return 0; }
  int *end() const { return 0; }
};

constexpr IterationLoop loop() { return IterationLoop(); }

class BenchmarkRegistrar {
public:
  BenchmarkRegistrar(const char *name, void (*fn)());
};

int runMain(int argc, char **argv);

} // namespace microbe

#endif
