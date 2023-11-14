import matplotlib.pyplot as plt

# Sequential time for N=1000 (from the sequential version of the program)
Ts = 1000  # replace x1 with the actual value

# Parallel times for N=1000
Tp_values = [16, 44, 128, 380]  # replace x1, x2, x3, x4 with actual values

# Number of processors
p_values = [1, 2, 4, 8]

# Compute speedups
speedups = [Ts / Tp for Tp in Tp_values]

plt.figure(figsize=(10, 6))
plt.plot(p_values, speedups, '-o', label=f'N=1000')
plt.xlabel('Number of Processors (p)')
plt.ylabel('Speedup S(p)')
plt.title('Speedup vs. Number of Processors for N=1000')
plt.legend()
plt.grid(True)
plt.xticks(p_values)
plt.show()

