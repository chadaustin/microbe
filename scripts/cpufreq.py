import sys
import time

cpu_number = int(sys.argv[1])


while True:
    with open(f"/sys/bus/cpu/devices/cpu{cpu_number}/cpufreq/scaling_cur_freq") as f:
        print(f"{cpu_number}: {int(f.read())}")

    end = time.perf_counter() + 0.5
    while time.perf_counter() < end:
        pass
