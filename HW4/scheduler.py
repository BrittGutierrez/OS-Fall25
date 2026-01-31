
import sys, csv
from collections import deque
import heapq

# read in csv file that has ProcessID, Arrival Time, Burst Time
def read_processes(path):
    procs = []
    with open(path) as f:
        r = csv.reader(f)
        next(r)  # skip the header line
        for row in r:
            if len(row) < 3:
                continue
            pid = int(row[0])
            at = int(row[1])
            bt = int(row[2])
            procs.append({"pid": pid, "arrival": at, "burst": bt})
    return procs


# returns any processes that just arrived at the current time
# removes them from the not_arrived list
def arrivals_now(not_arrived, time):
    arr = [p for p in not_arrived if p["arrival"] == time]
    arr.sort(key=lambda p: p["pid"])  # if two show up at same time, smaller id first
    for p in arr:
        not_arrived.remove(p)
    return arr


#FCFS
def fcfs(processes):
    # sort by arrival time first, then pid
    procs = sorted(processes, key=lambda p: (p["arrival"], p["pid"]))
    time = 0
    gantt = []
    wait = {}
    turn = {}

    for p in procs:
        # if CPU is idle (nothing ready yet)
        if time < p["arrival"]:
            gantt.append((time, "IDLE", p["arrival"]))
            time = p["arrival"]

        start = time
        end = start + p["burst"]
        gantt.append((start, p["pid"], end))
        time = end

        # calc wait/turnaround times
        wait[p["pid"]] = start - p["arrival"]
        turn[p["pid"]] = end - p["arrival"]

    avg_w = sum(wait.values()) / len(processes)
    avg_t = sum(turn.values()) / len(processes)
    throughput = len(processes) / gantt[-1][2]
    return wait, turn, avg_w, avg_t, throughput, gantt


# SJF PREEMPTIVE
def sjf_preemptive(processes):
    not_arrived = [dict(p) for p in processes]
    remaining = {p["pid"]: p["burst"] for p in processes}
    ready = []  # (remaining_time, pid)
    gantt = []
    time = 0
    current = None
    last_start = 0
    done = set()

    while len(done) < len(processes):
        # check if anything new arrived right now
        for p in arrivals_now(not_arrived, time):
            heapq.heappush(ready, (remaining[p["pid"]], p["pid"]))

        # if nothing is running
        if current is None:
            if not ready:
                # skip ahead to next arrival (CPU idle)
                if not_arrived:
                    nxt = min(p["arrival"] for p in not_arrived)
                    gantt.append((time, "IDLE", nxt))
                    time = nxt
                    continue
            else:
                _, current = heapq.heappop(ready)
                last_start = time
        else:
            # if a new shorter job showed up, switch
            if ready and remaining[current] > ready[0][0]:
                gantt.append((last_start, current, time))
                heapq.heappush(ready, (remaining[current], current))
                _, current = heapq.heappop(ready)
                last_start = time

        # run one time unit
        remaining[current] -= 1
        time += 1

        # done with this process
        if remaining[current] == 0:
            gantt.append((last_start, current, time))
            done.add(current)
            current = None

    # figure out when each one finished
    completion = {}
    for s, lab, e in gantt:
        if lab != "IDLE":
            completion[lab] = e

    wait = {}
    turn = {}
    for p in processes:
        pid = p["pid"]
        turn[pid] = completion[pid] - p["arrival"]
        wait[pid] = turn[pid] - p["burst"]

    avg_w = sum(wait.values()) / len(processes)
    avg_t = sum(turn.values()) / len(processes)
    throughput = len(processes) / gantt[-1][2]
    return wait, turn, avg_w, avg_t, throughput, gantt


# ROUND ROBIN
def round_robin(processes, q):
    not_arrived = [dict(p) for p in processes]
    remaining = {p["pid"]: p["burst"] for p in processes}
    ready = deque()
    gantt = []
    time = 0
    done = set()

    # load whatever starts at 0
    for p in arrivals_now(not_arrived, 0):
        ready.append(p["pid"])

    while len(done) < len(processes):
        if not ready:
            # CPU idle until something arrives
            if not_arrived:
                nxt = min(p["arrival"] for p in not_arrived)
                gantt.append((time, "IDLE", nxt))
                time = nxt
                for p in arrivals_now(not_arrived, time):
                    ready.append(p["pid"])
            continue

        pid = ready.popleft()
        start = time
        slice_len = min(q, remaining[pid])

        # run that process for the time slice
        for step in range(slice_len):
            time += 1
            # arrivals that happen during the slice go to the back
            if step < slice_len - 1:
                for p in arrivals_now(not_arrived, time):
                    ready.append(p["pid"])

        end = time
        gantt.append((start, pid, end))
        remaining[pid] -= slice_len

        # arrivals that show up exactly when we switch go to the front
        new_at_switch = arrivals_now(not_arrived, time)
        for p in reversed(new_at_switch):
            ready.appendleft(p["pid"])

        if remaining[pid] > 0:
            ready.append(pid)  # still has time left
        else:
            done.add(pid)

    # find when each one finished
    completion = {}
    for s, lab, e in gantt:
        if lab != "IDLE":
            completion[lab] = e

    wait = {}
    turn = {}
    for p in processes:
        pid = p["pid"]
        turn[pid] = completion[pid] - p["arrival"]
        wait[pid] = turn[pid] - p["burst"]

    avg_w = sum(wait.values()) / len(processes)
    avg_t = sum(turn.values()) / len(processes)
    throughput = len(processes) / gantt[-1][2]
    return wait, turn, avg_w, avg_t, throughput, gantt


# prints everything out in the same format the homework wants
def show(title, name, processes, wait, turn, avg_w, avg_t, thr, gantt):
    print(title)
    pids = sorted(p["pid"] for p in processes)
    print("Process ID", *pids)
    print("Waiting Time", *(wait[p] for p in pids))
    print("Turnaround Time", *(turn[p] for p in pids))
    print()
    print(f"{name} Gantt Chart")
    for s, lab, e in gantt:
        print(f"[{s}]--{lab}--[{e}]")
    print(f"Average Waiting Time: {avg_w:.2f}")
    print(f"Average Turnaround Time: {avg_t:.2f}")
    print(f"Throughput: {thr}")
    print()


# main just runs all three back-to-back
def main():
    if len(sys.argv) != 2 and len(sys.argv) != 3:
        print("Usage: python3 scheduler.py <file.csv> <quantum>")
        sys.exit(1)

    csvfile = sys.argv[1]
    quantum = int(sys.argv[2])

    processes = read_processes(csvfile)

    # FCFS
    w,t,aw,at,th,g = fcfs(processes)
    show("_______________FCFS_______________", "FCFS", processes, w,t,aw,at,th,g)

    # SJF-P
    w,t,aw,at,th,g = sjf_preemptive(processes)
    show("___Shortest Job First with Preemption____",
         "Shortest Job First with Preemption", processes, w,t,aw,at,th,g)

    # RR
    w,t,aw,at,th,g = round_robin(processes, quantum)
    show("___________Round Robin___________", "Round Robin", processes, w,t,aw,at,th,g)


if __name__ == "__main__":
    main()
