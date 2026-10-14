# Exercise 4: time vs number of processes, and speedup, for Exercises 2 and 3.
# Reads timings.csv (median of 5 runs each). Needs matplotlib:  python plot.py
import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

rows = list(csv.DictReader(open("timings.csv")))
titles = {"sum_mpi": "Exercise 2: sum of 1..10,000,000", "montecarlo_mpi": "Exercise 3: Monte Carlo Pi"}

for prog, title in titles.items():
    procs = [int(r["processes"]) for r in rows if r["exercise"] == prog]
    times = [float(r["time_s"]) for r in rows if r["exercise"] == prog]
    speedup = [times[0] / t for t in times]
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 4))
    ax1.plot(procs, times, marker="o"); ax1.set_ylabel("Time (s)"); ax1.set_title("Time vs processes")
    ax2.plot(procs, speedup, marker="o", label="measured")
    ax2.plot(procs, procs, linestyle="--", color="grey", label="ideal")
    ax2.set_ylabel("Speedup (T1 / TP)"); ax2.set_title("Speedup vs processes"); ax2.legend()
    for ax in (ax1, ax2):
        ax.set_xlabel("Number of processes"); ax.set_xticks(procs); ax.grid(alpha=0.3)
    fig.suptitle(title); fig.tight_layout(); fig.savefig(f"{prog}.png", dpi=130); plt.close(fig)
    print(title, [f"{p}: {s:.2f}x" for p, s in zip(procs, speedup)])
