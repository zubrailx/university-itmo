import sys
import time
import re

import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
import numpy as np

def get_lines_path(path):
    file = open(path, "r")
    lines = file.readlines()
    file.close()
    return lines

def parse_and_display_cpu(path):
    data_cpu = {
        "header": ["DATE", "UID", "PID", "%usr", "%system", "%guest", "%wait", "%CPU", "CPU", "Command"],
        "pattern_header": r"^(\d\d:\d\d:\d\d\s+[AP]M)\s+(UID)\s+(PID)\s+(%usr)\s+(%system)\s+(%guest)\s+(%wait)\s+(%CPU)\s+(CPU)\s+(Command)$",
        "pattern_data": r"^(\d\d:\d\d:\d\d\s+[AP]M)\s+(\d+)\s+(\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+)\s+(.+)$",
        "data" : []
    }
    data_mem = {
        "header":["DATE", "UID", "PID", "minftl/s", "majflt/s", "VSZ", "RSS", "%MEM", "Command"], 
        "pattern_header": r"(\d\d:\d\d:\d\d\s+[AP]M)\s+(UID)\s+(PID)\s+(minflt\/s)\s+(majflt\/s)\s+(VSZ)\s+(RSS)\s+(%MEM)\s+(Command)",
        "pattern_data": r"^(\d\d:\d\d:\d\d\s+[AP]M)\s+(\d+)\s+(\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+)\s+(\d+)\s+(\d+\.\d+)\s+(.+)$",
        "data" : []
    }
    regex_title_cpu = re.compile(data_cpu["pattern_header"], flags=re.M)
    regex_data_cpu = re.compile(data_cpu["pattern_data"], flags=re.M)
    regex_title_mem = re.compile(data_mem["pattern_header"], flags=re.M)
    regex_data_mem = re.compile(data_mem["pattern_data"], flags=re.M)

    def process_data_cpu(match):
        data_cpu["data"].append([
            # datetime.datetime.fromtimestamp(time.mktime(time.strptime(match[1], r"%I:%M:%S %p"))),
            time.strptime(match[1], r"%I:%M:%S %p"),
            int(match[2]),
            int(match[3]),
            float(match[4]),
            float(match[5]),
            float(match[6]),
            float(match[7]),
            float(match[8]),
            int(match[9]),
            match[10]
        ])

    def process_data_mem(match):
        data_mem["data"].append([
            time.strptime(match[1], r"%I:%M:%S %p"),
            int(match[2]),
            int(match[3]),
            float(match[4]),
            float(match[5]),
            int(match[6]),
            int(match[7]),
            float(match[8]),
            match[9]
        ])

    is_inside_cpu = False
    is_inside_mem = False
    for line in get_lines_path(path):
        # check next line for data
        if is_inside_cpu:
            res_cpu = regex_data_cpu.search(line)
            if res_cpu is None:
                is_inside_cpu = False
            else:
                process_data_cpu(res_cpu)
                continue
        elif is_inside_mem:
            res_mem = regex_data_mem.search(line)
            if res_mem is None:
                is_inside_mem = False
            else:
                process_data_mem(res_mem)
                continue

        # check if next line is header
        res_cpu = regex_title_cpu.search(line)
        res_mem = regex_title_mem.search(line)
        if res_cpu is not None:
            is_inside_cpu = True
        elif res_mem is not None:
            is_inside_mem = True

    # Visualize
    # Three graphs
    fig, axs = plt.subplots(3, 1)
    l = len(data_cpu["data"])
    usr_x = np.arange(0, l, 1)
    usr_y = np.array([data[3] for data in data_cpu["data"]])
    sys_y = np.array([data[4] for data in data_cpu["data"]])
    axs[0].plot(usr_x, usr_y, color="green")
    axs[0].set_xlim(0, l)
    axs[0].grid()
    axs[0].set_ylabel(data_cpu["header"][3])
    axs[0].set_xlabel("seconds")

    axs[1].plot(usr_x, sys_y, color = "red")
    axs[1].set_xlim(0, l)
    axs[1].grid()
    axs[1].set_ylabel(data_cpu["header"][4])
    axs[1].set_xlabel("seconds")

    axs[2].plot(usr_x, usr_y + sys_y, color = "orange")
    axs[2].set_xlim(0, l)
    axs[2].grid()
    axs[2].set_ylabel(data_cpu["header"][3] + " + " + data_cpu["header"][4])
    axs[2].set_xlabel("seconds")

    fig.tight_layout()
    plt.savefig("fig-cpu.png")
    plt.clf()

    # One graph cpu
    fig, ax = plt.subplots()
    ax.plot(usr_x, usr_y, color="green", label=data_cpu["header"][3])
    ax.plot(usr_x, sys_y, color = "red", label=data_cpu["header"][4])
    ax.plot(usr_x, usr_y + sys_y, color = "orange", label="total")
    ax.set_xlim(0, l)
    ax.set_xlabel("seconds")
    ax.set_ylabel("%CPU usage")
    ax.legend()
    plt.savefig("fig-cpu-1.png")
    plt.clf()

    # One graph cpu
    l = len(data_mem["data"])
    mem_x = np.arange(0, l, 1)
    vsz_y = np.array([data[5] for data in data_mem["data"]])
    rss_y = np.array([data[6] for data in data_mem["data"]])
    mem_y = np.array([data[7] for data in data_mem["data"]])

    fig, axs = plt.subplots(3, 1)
    axs[0].plot(mem_x, mem_y, color="blue")
    axs[0].set_xlim(0, l)
    axs[0].grid()
    axs[0].set_ylabel(data_mem["header"][7])
    axs[0].set_xlabel("seconds")

    axs[1].plot(usr_x, vsz_y, color = "blue")
    axs[1].set_xlim(0, l)
    axs[1].grid()
    axs[1].set_ylabel(data_mem["header"][5] + " Kb")
    axs[1].set_xlabel("seconds")

    axs[2].plot(usr_x, rss_y, color = "blue")
    axs[2].set_xlim(0, l)
    axs[2].grid()
    axs[2].set_ylabel(data_mem["header"][6] + " Kb")
    axs[2].set_xlabel("seconds")

    fig.tight_layout()
    plt.savefig("fig-mem.png")
    plt.clf()
    return data_cpu


def parse_and_display_io(path):
    data_io = {
        "header":["Device", "tps", "kB_read/s", "kB_wrtn/s", "kB_dscd/s", "kB_read", "kB_wrtn", "kB_dscd"], 
        "pattern_header": r"^(Device)\s+(tps)\s+(kB_read/s)\s+(kB_wrtn/s)\s+(kB_dscd/s)\s+(kB_read)\s+(kB_wrtn)\s+(kB_dscd)$",
        "pattern_data": r"^(\S+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+)\s+(\d+)\s+(\d+)$",
        "data" : []
    }
    regex_title_io = re.compile(data_io["pattern_header"], flags=re.M)
    regex_data_io = re.compile(data_io["pattern_data"], flags=re.M)

    def process_data_io(match):
        data_io["data"].append([
            # datetime.datetime.fromtimestamp(time.mktime(time.strptime(match[1], r"%I:%M:%S %p"))),
            match[1],
            float(match[2]),
            float(match[3]),
            float(match[4]),
            float(match[5]),
            int(match[6]),
            int(match[7]),
            int(match[8]),
        ])

    is_inside_io = False
    for line in get_lines_path(path):
        # check next line for data
        if is_inside_io:
            res_io = regex_data_io.search(line)
            if res_io is None:
                is_inside_io = False
            else:
                process_data_io(res_io)
                continue
        # check if next line is header
        res_io = regex_title_io.search(line)
        if res_io is not None:
            is_inside_io = True

    # Visualize
    # Three graphs
    fig, axs = plt.subplots(3, 1)
    l = len(data_io["data"])
    usr_x = np.arange(0, l, 1)
    tps_y = np.array([data[1] for data in data_io["data"]])
    kbr_y = np.array([data[2] for data in data_io["data"]])
    kbw_y = np.array([data[3] for data in data_io["data"]])

    axs[0].plot(usr_x, tps_y)
    axs[0].set_xlim(0, l)
    axs[0].grid()
    axs[0].set_ylabel(data_io["header"][1])
    axs[0].set_xlabel("seconds")

    axs[1].plot(usr_x, kbr_y)
    axs[1].set_xlim(0, l)
    axs[1].grid()
    axs[1].set_ylabel(data_io["header"][2])
    axs[1].set_xlabel("seconds")

    axs[2].plot(usr_x, kbw_y)
    axs[2].set_xlim(0, l)
    axs[2].grid()
    axs[2].set_ylabel(data_io["header"][3])
    axs[2].set_xlabel("seconds")

    fig.tight_layout()
    plt.savefig("fig-io.png")
    plt.clf()
    return data_io

def parse_and_display_net(path):
    PATTERN_HEADER = r"\s+(KB/s in)\s+(KB/s out)"
    PATTERN_DATA = r"\s+(\d+\.\d+)\s+(\d+\.\d+)"

    data_net = {
        "header":{"KB/s in", "KB/s out"}, 
        # idx - device idx, value - as previous
        "pattern_header": None,
        "pattern_data": None,
        "devices": None,
        # time, [{}, {}] - idx - device
        "data" : [],
    }
    
    lines = get_lines_path(path)
    regex_main_header_time = re.compile(r"^\s+(Time)")
    
    for line in lines:
        res = regex_main_header_time.match(line)
        if res is not None:
            lst = re.findall(r"\s+(\S+)", line[res.end():])
            data_net["devices"] = lst
            data_net["pattern_header"] = r"^(HH:MM:SS)" + PATTERN_HEADER * len(lst) + "$"
            data_net["pattern_data"] = r"^(\d\d:\d\d:\d\d)" + PATTERN_DATA * len(lst) + "$"
            break
        
    regex_title_net = re.compile(data_net["pattern_header"], flags=re.M)
    regex_data_net = re.compile(data_net["pattern_data"], flags=re.M)

    def process_data_net(match):
        data = []
        data.append(time.strptime(match[1], r"%H:%M:%S"))
        data.append([])
        for idx in range(len(data_net["devices"])):
            data[1].append([
                float(match[(idx + 1) * 2]),
                float(match[(idx + 1) * 2 + 1])
           ])
        data_net["data"].append(data)
            

    is_inside_net = False
    for line in get_lines_path(path):
        # check next line for data
        if is_inside_net:
            res_net = regex_data_net.search(line)
            if res_net is None:
                is_inside_net = False
            else:
                process_data_net(res_net)
                continue
        # check if next line is header
        res_net = regex_title_net.search(line)
        if res_net is not None:
            is_inside_net = True

    # Visualize
    # Three graphs
    devlen = len(data_net["devices"])
    fig, axs = plt.subplots(devlen + 1, 1)
    l = len(data_net["data"])
    x = np.arange(0, l, 1)
    for i in range(devlen):
        net_in = np.array([data[1][i][0] for data in data_net["data"]])
        net_out = np.array([data[1][i][1] for data in data_net["data"]])
        axs[i].plot(x, net_in, color="green")
        axs[i].plot(x, net_out, color="red")
        axs[i].set_xlim(0, l)
        axs[i].grid()
        axs[i].set_ylabel(data_net["devices"][i] + ": KB/s")
        axs[i].set_xlabel("seconds")

    arr_in = []
    for i in range(l):
            arr_in.append(sum([data_net["data"][i][1][j][0] for j in range(devlen)]))
    arr_out = []
    for i in range(l):
            arr_out.append(sum([data_net["data"][i][1][j][1] for j in range(devlen)]))
    axs[devlen].plot(x, np.array(arr_in), color="green")
    axs[devlen].plot(x, np.array(arr_out), color="red")
    axs[devlen].set_xlim(0, l)
    axs[devlen].grid()
    axs[devlen].set_ylabel("total: KB/s")
    axs[devlen].set_xlabel("seconds")

    fig.tight_layout()
    plt.savefig("fig-net.png")
    plt.clf()
    return data_net

def parse_and_display_state(path):
    data_state = {
        "header":["PID", "USER", "PR", "NI", "VIRT", "RES", "SHR", "S", "%CPU", "%MEM", "TIME+", "COMMAND"], 
        "pattern_header": r"^[^\S\n\r]+(PID)\s+(USER)\s+(PR)\s+(NI)\s+(VIRT)\s+(RES)\s+(SHR)\s+(S)\s+(%CPU)\s+(%MEM)\s+(TIME\+)\s+(COMMAND)$",
        "pattern_data": r"^\s+(\d+)\s+(\S+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\S+)\s+(\d+)\s+(\S)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+:\d+\.\d+)\s+(\S+)$",
        "data" : []
    }
    regex_title_state = re.compile(data_state["pattern_header"], flags=re.M)
    regex_data_state = re.compile(data_state["pattern_data"], flags=re.M)

    def process_data_state(match, idx):
        data_state["data"][idx].append([
            int(match[1]),
            (match[2]),
            int(match[3]),
            int(match[4]),
            int(match[5]),
            match[6],
            int(match[7]),
            match[8],
            float(match[9]),
            match[10],
            match[11],
        ])
    is_inside_state = False
    idx = -1
    for line in get_lines_path(path):
        # check next line for data
        if is_inside_state:
            res_state = regex_data_state.search(line)
            if res_state is None:
                is_inside_state = False
            else:
                process_data_state(res_state, idx)
                continue
        # check if next line is header
        res_state = regex_title_state.search(line)
        if res_state is not None:
            is_inside_state = True
            data_state["data"].append([])
            idx += 1
    # Visualize
    # Three graphs
    l = len(data_state["data"]) // 2
    x = np.arange(0, l, 1)

    state_x = np.arange(0, l, 1)
    colors = {
        "S": "blue",
        "D": "green",
        "R": "red",
        "T": "orange",
        "Z": "purple"
    }
    arr = []
    states = list(colors.keys())
    # all_states = []
    max_t = 0
    for idx in range(l):
        data = data_state["data"][idx]
        arr.append([0, 0, 0, 0, 0])
        max_t = max(max_t, len(data))
        for thread in data:
            arr[idx][states.index(thread[7][0])] += 1
            # if thread[7][0] not in all_states:
            #     all_states.append(thread[7][0])
    # print(all_states)

    arr_s = np.array([(data[0] / max_t) for data in arr])
    arr_d = np.array([(data[1] / max_t) for data in arr])
    arr_r = np.array([(data[2] / max_t) for data in arr])
    arr_t = np.array([(data[3] / max_t) for data in arr])
    arr_z = np.array([(data[4] / max_t) for data in arr])

    fig, ax = plt.subplots()
    ax.bar(x, arr_r, 0.6, label="R", color=colors["R"])
    ax.bar(x, arr_d, 0.6, bottom=arr_r, label="D", color=colors["D"])
    ax.bar(x, arr_s, 0.6, bottom=arr_r + arr_d, label="S", color=colors["S"])
    ax.bar(x, arr_t, 0.6, bottom=arr_r + arr_d + arr_s, label="T", color=colors["T"])
    ax.bar(x, arr_z, 0.6, bottom=arr_r + arr_d + arr_s + arr_t, label="Z", color=colors["Z"])
                
    ax.set_xlabel("seconds")
    ax.set_ylabel("Threads")
    ax.set_xlim(0, l)
    ax.legend()

    plt.tight_layout()
    plt.savefig("fig-state.png", dpi=300)
    plt.clf()

    # Vertical figure
    tabl = {} # key - pid, val - array of states
    for i in range(l):
        for thread in data_state["data"][i]:
            pid = thread[0]
            state = thread[7]
            if pid not in tabl:
                tabl[pid] = []
            tabl_pid = tabl[pid]
            for i in range(len(tabl_pid), i):
                tabl_pid.append("-")
            tabl_pid.append(state)
                
    pids = list(tabl.keys())
    fig, ax = plt.subplots()
    h_coef = 0.75
    height = 2
    for i in range(len(pids)):
        pid = pids[i]
        states = tabl[pid]
        if len(states) == 0:
            continue
        start = 0
        for cur in range(l):
            if (start != states[cur]):
                color = colors[states[start]]
                ax.broken_barh([(start, (cur - start))], 
                               (i * height + (1 - h_coef) * height * 0.5 + height * 0.5, height * h_coef),
                               facecolors=color)
                start = cur
        ax.broken_barh([(start, (l - start))],
                       (i * height + (1 - h_coef) * height * 0.5 + height * 0.5, height * h_coef),
                       facecolors=colors[states[start]])

    ax.set_xlabel("Seconds")
    ax.set_ylabel("Threads")
    ax.set_yticks([i * height for i in range(1, len(pids) + 1)], labels=[str(pid) for pid in pids])
    ax.set_xlim(0, l)
    ax.set_ylim(0, (len(pids) + 1) * height)

    patch_s = mpatches.Patch(color = colors["S"], label="S")
    patch_d = mpatches.Patch(color = colors["D"], label="D")
    patch_r = mpatches.Patch(color = colors["R"], label="R")
    patch_t = mpatches.Patch(color = colors["T"], label="T")
    patch_z = mpatches.Patch(color = colors["Z"], label="Z")
    ax.legend(handles=[patch_s, patch_d, patch_r, patch_t, patch_z])

    plt.tight_layout()
    plt.savefig("fig-state-vert.png", dpi=300)
    plt.clf()
    return data_state


# -c=PATH, -i=PATH, -n=PATH
if __name__ == "__main__":
    path_cpu = None
    path_io = None
    path_net = None
    path_state = None

    path_state = "/home/nikit/git/University-ITMO/Year-3/Operating-systems/os-1-state.log"
    args = sys.argv
    for arg in args[1:]:
        key, val = arg[0:2], arg[3:]
        match key:
            case "-c":
                path_cpu = val
            case "-i":
                path_io = val
            case "-n":
                path_net = val
            case "-s":
                path_state = val
            case _:
                print(f"Unknown argument: {key}={val}")
    if path_cpu is not None:
        parse_and_display_cpu(path_cpu)
    if path_io is not None:
        parse_and_display_io(path_io)
    if path_net is not None:
        parse_and_display_net(path_net)
    if path_state is not None:
        parse_and_display_state(path_state)
