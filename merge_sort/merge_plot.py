#-*-coding:utf-8-*-
import numpy as np
import pandas as pd
import subprocess
import matplotlib
import matplotlib.pyplot as plt

def main():
    #config
    exec_files = ["./merge", "./p_merge"]

    args = {"merge": ["2000000", "4000000", "6000000", "8000000", "10000000"],
    "p_merge:thread=2": ["2000000 2", "4000000 2", "6000000 2", "8000000 2", "10000000 2"],
    "p_merge:thread=4": ["2000000 4", "4000000 4", "6000000 4", "8000000 4", "10000000 4"]}

    mapping = {"./merge" : ["merge"],"./p_merge": ["p_merge:thread=2", "p_merge:thread=4"]}


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

    plt.xticks(np.arange(2000000,10000001, 2000000))
    plt.xlabel("data num")
    plt.ylabel("time [msec]")
    plt.grid()
    plt.legend()

    fig.savefig("merge_result.png")

if __name__ == "__main__":
    main()
