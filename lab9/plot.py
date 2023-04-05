import re
import matplotlib.pyplot as plt

def readResult(file):
    with open(file, 'r', encoding='utf8') as f:
        res = {}
        lines = list(e.strip() for e in f.readlines() if e != '\n')
        for line in lines:
            content = line.split(":")
            print(line)
            res[int(content[0])] = float(content[1])/1000000000.0
        print(res)
        return res

def generatePlot():
    data = readResult("time_data.txt")
    nice_value = data.keys()
    time = data.values()

    plt.xlabel("Nice Values")
    plt.ylabel("Time Cost (sec)")
    plt.plot(nice_value, time)
    plt.title("Nice Value vs Time Cost to hack RSA")
    # plt.legend()
    plt.savefig("nice_value_vs_time.jpg")
    plt.show()

# readResult("time_data.txt")
generatePlot()
