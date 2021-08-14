#-*-coding:utf-8-*-
import numpy as np
import pandas as pd
import subprocess
import matplotlib
import matplotlib.pyplot as plt

def main():
    #config
    exec_files = ["./p_scan"]

    args = {"p_scan:thread=1": ["16777216 1", "33554432 1", "67108864 1", "134217728 1"],
    "p_scan:thread=2": ["16777216 2", "33554432 2", "67108864 2", "134217728 2"],
    "p_scan:thread=4": ["16777216 4", "33554432 4", "67108864 4", "134217728 4"],
    "p_scan:thread=8": ["16777216 8", "33554432 8", "67108864 8", "134217728 8"]}

    mapping = {"./p_scan": ["p_scan:thread=1", "p_scan:thread=2", "p_scan:thread=4", "p_scan:thread=8"]}


    fig = plt.figure()
    for proc in exec_files:
        proc_names = mapping[proc]
        for proc_name in proc_names:
            X_num = []
            Y_time = []
            for arg in args[proc_name]:
                cmd = proc +" "+arg
                print("=== execution ===")
                print(cmd)
                result = subprocess.run(cmd, shell=True, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                if result.returncode != 0:
                    print("error")
                    return
                else:
                    out = result.stdout
                    arg = arg.split()[0]
                    X_num.append(int(arg))
                    Y_time.append(int(out))
                    print(str(int(out))+"[msec]")

            plt.plot(X_num, Y_time, label=proc_name)

    plt.xticks(np.arange(10000000,150000000, 10000000))
    plt.xlabel("data num")
    plt.ylabel("time [msec]")
    plt.grid()
    plt.legend()

    fig.savefig("parallel_result.png")

if __name__ == "__main__":
    main()
