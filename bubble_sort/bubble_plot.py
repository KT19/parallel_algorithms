#-*-coding:utf-8-*-
import numpy as np
import pandas as pd
import subprocess
import matplotlib
import matplotlib.pyplot as plt

def main():
    #config
    exec_files = ["./bubble", "./p_bubble"]

    args = {"bubble": ["100000", "200000", "300000", "400000", "500000"],
    "p_bubble:thread=2": ["100000 2", "200000 2", "300000 2", "400000 2", "500000 2"],
    "p_bubble:thread=4": ["100000 4", "200000 4", "300000 4", "400000 4", "500000 4"]}

    mapping = {"./bubble" : ["bubble"],"./p_bubble": ["p_bubble:thread=2", "p_bubble:thread=4"]}


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

    plt.xticks(np.arange(100000, 500001, 100000))
    plt.xlabel("data num")
    plt.ylabel("time [msec]")
    plt.grid()
    plt.legend()

    fig.savefig("bubble_result.png")

if __name__ == "__main__":
    main()
