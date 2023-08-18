#include "microbe/timer.h"

#include <algorithm>
#include <chrono>
#include <inttypes.h>
#include <iterator>
#include <stdio.h>
#include <time.h>

#include <sched.h>

#include "microbe/port.h"

namespace {

int64_t std_steady_clock() {
  return std::chrono::steady_clock::now().time_since_epoch().count();
}

int64_t std_high_resolution_clock() {
  return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

int64_t unix_monotonic_clock() {
  timespec ts;
  int res = clock_gettime(CLOCK_MONOTONIC, &ts);
  if (MICROBE_UNLIKELY(res)) {
    perror("clock_gettime(CLOCK_MONOTONIC) failed");
    abort();
  }
  return 1000000000 * ts.tv_sec + ts.tv_nsec;
}

int64_t unix_monotonic_clock_raw() {
  timespec ts;
  int res = clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  if (MICROBE_UNLIKELY(res)) {
    perror("clock_gettime(CLOCK_MONOTONIC_RAW) failed");
    abort();
  }
  return 1000000000 * ts.tv_sec + ts.tv_nsec;
}

// TODO: QueryPerformanceCounter

struct Timer {
  const char *name;
  int64_t (*fn)();
};

constexpr Timer gTimers[] = {
    {"std_steady_clock", std_steady_clock},
    {"std_high_resolution_clock", std_high_resolution_clock},
    {"unix_monotonic_clock", unix_monotonic_clock},
    {"unix_monotonic_clock_raw", unix_monotonic_clock_raw},
};

void evaluateTimer(const Timer &timer) {
  printf("evaluating timer: %s\n", timer.name);

  // If all of the values in 10000 entries are identical, the timer's precision
  // is worse than one microsecond, even if it can be evaluated in one cycle at 10
  // GHz.
  constexpr size_t N = 10000;
  constexpr int64_t Limit =
      1000000000ll; // Spend no longer than one second evaluating the timer.
  int64_t times[N];
  int64_t *out = times;
  do {
    static_assert(N % 8 == 0);
    out[0] = timer.fn();
    out[1] = timer.fn();
    out[2] = timer.fn();
    out[3] = timer.fn();
    out[4] = timer.fn();
    out[5] = timer.fn();
    out[6] = timer.fn();
    out[7] = timer.fn();
    out += 8;
  } while (out < times + N && out[-1] < times[0] + Limit);
  size_t count = out - times;

  bool monotonic = true;
  for (size_t i = 1; i < count; ++i) {
    if (times[i] < times[i - 1]) {
      monotonic = false;
      break;
    }
  }

  int64_t durations[N - 1];
  for (size_t i = 0; i < N - 1; ++i) {
    durations[i] = times[i + 1] - times[i];
  }
  std::sort(durations, std::end(durations));
  auto minimum = durations[0];
  auto median = durations[N / 2];

  printf("  evaluated %" PRId64 " in %" PRId64 " ns\n", count,
         times[count - 1] - times[0]);
  printf("  minimum: %" PRId64 " ns\n", minimum);
  printf("  median: %" PRId64 " ns\n", median);
  printf("  monotonic? %s\n", monotonic ? "yes" : "no");
}

struct AffinityScope {
  AffinityScope() {
    if (-1 ==
        sched_getaffinity(0, sizeof(previous_cpu_set_), &previous_cpu_set_)) {
      perror("sched_getaffinty failed");
      abort();
    }

    // TODO: read sysfs /sys/devices/system/cpu/kernel_max to dynamically
    // determine maximum number of CPUs supported by kernel For now, assume
    // cpu_set_t is big enough.
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    // Assume the CPU currently scheduled is appropriate, since it was chosen to
    // run the process until now.
    unsigned cpu = sched_getcpu();
    if (-1 == cpu) {
      perror("sched_getcpu failed");
      abort();
    }
    CPU_SET(cpu, &cpuset);

    if (-1 == sched_setaffinity(0, sizeof(cpuset), &cpuset)) {
      perror("sched_setaffinity failed");
      abort();
    }
  }

  ~AffinityScope() {
    if (-1 ==
        sched_setaffinity(0, sizeof(previous_cpu_set_), &previous_cpu_set_)) {
      perror("sched_setaffinity failed");
      abort();
    }
  }

  cpu_set_t previous_cpu_set_;
};

} // namespace

namespace microbe {

void selectTimer() {
  AffinityScope scope;

  for (const auto &timer : gTimers) {
    evaluateTimer(timer);
  }
}

} // namespace microbe
