import os
import time
import matplotlib.pyplot as plt

input_size = 10000

# Function to run the C program and measure its execution time
def run_and_measure(program, input_size):
    start_time = time.time()
    os.system(f"./{program}")
    return time.time() - start_time

def main():
    programs = ['main.o', 'main_threads.o']
    input_sizes = [100, 200, 300, 400, 500]  
    speedups = {'main.o': [], 'main_threads.o': []}

    sequential_times = [run_and_measure('main', size) for size in input_sizes]

    for program in programs:
        parallel_times = [run_and_measure(program, size) for size in input_sizes]
        speedup = [seq / par for seq, par in zip(sequential_times, parallel_times)]
        speedups[program] = speedup

    plt.figure(figsize=(10, 5))
    for program, speedup in speedups.items():
        plt.plot(input_sizes, speedup, '-o', label=program)

    plt.title("Speedup of Parallel Implementations")
    plt.xlabel("Input Size")
    plt.ylabel("Speedup")
    plt.legend()
    plt.grid(True)
    plt.show()
    
if __name__ == "__main__":
    main()
