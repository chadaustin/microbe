#include <microbe.h>

#include <string>

using namespace microbe;

MICROBE(std_string_allocation_small) {
    printf("outside loop\n");
    for (auto _ : loop()) {
        printf("inside loop\n");
        doNotOptimize(std::string{"small"});
    }
}

MICROBE_MAIN();
