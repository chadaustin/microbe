#include <microbe.h>

#include <string>

using namespace microbe;

MICROBM(std_string_allocation_small) {
    for (auto _ : loop()) {
        doNotOptimize(std::string{"small"});
    }
}

MICROBE_MAIN();
