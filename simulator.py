import subprocess
import time
import os
import math
import random
import statistics
from array import array
import csv  # cloudlab machines don't have matplot

EXECUTABLE = "./sorter"  # declared here in case you want to use a different executable
INPUT_FILE = "input.bin"
OUTPUT_FILE = "output.bin"
REPEATS = 3  # you can decrease this if the simulator takes too long to run
SEED = 67  # im so sorry
SIZES = [10, 100, 1000, 10_000, 100_000, 1_000_000, 10_000_000]
DATASET_TYPES = ["sorted", "uniform", "normal", "longtail"]
CSV_FILE = "results.csv"


# compile sorting.c:

def compile_sorter():
    if not os.path.exists("sorting.c"):
        print(f"sorting.c not found.")
        exit(1)

    cmd = ["gcc", "-O3", "sorting.c", "-o", "sorter"]
    print("Compiling:", " ".join(cmd))
    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        print("Compilation failed\n", result.stderr)
        exit(1)

    print("Compilation complete")


# make datasets:

def write_input_file(data):
    """Write array to input.bin as uint32 values"""
    arr = array('I', data)
    with open(INPUT_FILE, "wb") as f:
        f.write(arr.tobytes())

def read_output_file(n):
    """Read sorted values from output.bin"""
    arr = array('I')
    with open(OUTPUT_FILE, "rb") as f:
        arr.fromfile(f, n)
    return arr.tolist()

def gen_sorted(n):
    return list(range(n))

def gen_uniform(n, rng):
    return [rng.getrandbits(32) for _ in range(n)]

def gen_normal(n, rng):
    vals = []
    for _ in range(n):
        g = rng.gauss(0, 1)
        g = max(min(g, 6), -6)
        t = (g + 6) / 12.0
        vals.append(int(t * 0xFFFFFFFF))
    return vals

def gen_longtail(n, rng):
    alpha = 1.5
    vals = []
    for _ in range(n):
        p = rng.paretovariate(alpha)
        x = math.log(p)
        frac = (x % 10) / 10.0
        vals.append(int(frac * 0xFFFFFFFF))
    return vals

GENERATORS = {
    "sorted": gen_sorted,
    "uniform": gen_uniform,
    "normal": gen_normal,
    "longtail": gen_longtail
}


# sort a dataset:

def run_trial():
    """
    Runs ./sorter and returns:
    execution_time, peak_memory_MB (best if /usr/bin/time exists)
    """
    use_time = os.path.exists("/usr/bin/time")

    if use_time:
        cmd = ["/usr/bin/time", "-v", EXECUTABLE]
    else:
        cmd = [EXECUTABLE]

    start = time.perf_counter()
    proc = subprocess.run(cmd, capture_output=True, text=True)
    elapsed = time.perf_counter() - start

    peak_mb = float("nan")

    if use_time:
        import re
        match = re.search(r"Maximum resident set size \(kbytes\): (\d+)", proc.stderr)
        if match:
            kb = int(match.group(1))
            peak_mb = kb / 1024

    return elapsed, peak_mb


# generate & sort datasets:

def main():
    random.seed(SEED)
    compile_sorter()
    csv_data = []  # for jaxson's plots

    print("\nOutput format: TIME: MEAN (SEC), TIME: STD DEV (SEC), MEMORY: MEAN (MB), CORRECT (1/0), TIME/GB (SEC)")

    for dtype in DATASET_TYPES:
        for n in SIZES:
            print(f"\nGenerating {dtype} | size={n:,}")
            rng = random.Random(SEED + n + hash(dtype))

            # Generate data
            if dtype == "sorted":
                data = gen_sorted(n)
            else:
                data = GENERATORS[dtype](n, rng)

            expected = sorted(data)  # used to verify correctness of sorting.c

            # write to input.bin
            write_input_file(data)

            times = []
            memories = []
            correct = True

            for trial in range(REPEATS):
                t, mem = run_trial()
                times.append(t)
                memories.append(mem if not math.isnan(mem) else 0)

                output = read_output_file(n)
                if output != expected:
                    correct = False

            mean_time = statistics.mean(times)
            std_time = statistics.stdev(times) if len(times) > 1 else 0.0
            mean_mem = statistics.mean(memories)

            size_bytes = n * 4
            size_gb = size_bytes / (1024**3)

            time_per_gb = mean_time / size_gb if size_gb != 0 else float("inf")

            csv_data.append([mean_time, std_time, mean_mem, time_per_gb])  # for jaxson's plots
            print(f"{mean_time:.6f}, {std_time:.6f}, {mean_mem:.2f}, {int(correct)}, {time_per_gb:.4f}")

    print("\nAll tests finished")

    # FOR JAXSON'S PLOTS
    with open(CSV_FILE, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["mean_time", "std_time", "mean_memory_MB", "time_per_gb"])
        writer.writerows(csv_data)
    print("Saved results to", CSV_FILE)
    # FOR JAXSON'S PLOTS

if __name__ == "__main__":
    main()

