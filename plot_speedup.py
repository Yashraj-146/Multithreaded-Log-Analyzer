import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("/Users/yashraj146/Documents/MultithreadedLogAnalyzer/build/profiling_data.csv", names=["Threads","Sequential","Parallel","Speedup"])

plt.figure(figsize=(8,5))
plt.plot(df["Threads"], df["Speedup"], marker="o", linewidth=2)
plt.title("Speedup vs Number of Threads")
plt.xlabel("Number of Threads")
plt.ylabel("Speedup (x)")
plt.grid(True)
plt.tight_layout()
plt.savefig("speedup_plot.png")
plt.show()
