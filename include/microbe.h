#ifndef INCLUDE_MICROBE_H
#define INCLUDE_MICROBE_H

#define MICROBM(name)                                                          \
  static void benchmark_##name();                                              \
  void benchmark_name()

#define MICROBE_MAIN()                                                         \
  int main() {}                                                                \
  int main()

namespace microbe {
template <typename T> void doNotOptimize(T &&) {}

class IterationLoop {
public:
  int *begin() const { return 0; }
  int *end() const { return 0; }
};

constexpr IterationLoop loop() { return IterationLoop(); }
} // namespace microbe

#endif
