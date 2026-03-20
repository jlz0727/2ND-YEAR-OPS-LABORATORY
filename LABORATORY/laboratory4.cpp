#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;     // Only for PQ
    int queue_level;  // 1-RR, 2-Priority, 3-FCFS
    int start_time = -1;
    int finish_time = -1;
    int response_time = -1;
    int waiting_time = 0;
};

// Comparator for Priority Queue (PQ)
struct ComparePriority {
    bool operator()(Process* a, Process* b) {
        if (a->priority == b->priority)
            return a->arrival_time > b->arrival_time;
        return a->priority > b->priority; // lower number = higher priority
    }
};

int main() {
    int n;
    cin >> n;  // No prompt

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        cin >> processes[i].pid >> processes[i].arrival_time >> processes[i].burst_time
            >> processes[i].priority >> processes[i].queue_level;
        processes[i].remaining_time = processes[i].burst_time;
    }

    queue<Process*> RQ; // Round Robin
    priority_queue<Process*, vector<Process*>, ComparePriority> PQ; // Priority
    queue<Process*> FCFS; // FCFS

    int time = 0, completed = 0;
    int quantum = 4; // HOW TIME QUANTUM AFFECTS THE FAIRNESS OF SCHEDULING
    vector<bool> arrived(n, false);
    Process* current = nullptr;
    int rr_quantum_left = quantum;

    vector<int> Gantt(10000, -1); // large enough for high BT

    while (completed < n) {
        // Add newly arrived processes
        for (int i = 0; i < n; i++) {
            if (!arrived[i] && processes[i].arrival_time <= time) {
                arrived[i] = true;
                if (processes[i].queue_level == 1) RQ.push(&processes[i]);
                else if (processes[i].queue_level == 2) PQ.push(&processes[i]);
                else FCFS.push(&processes[i]);
            }
        }

        // HOW STARVATION IS AVOIDED
        if (!current) {
            if (!PQ.empty()) {
                current = PQ.top(); PQ.pop();
                rr_quantum_left = quantum;
            } else if (!RQ.empty()) {
                current = RQ.front(); RQ.pop();
                rr_quantum_left = quantum;
            } else if (!FCFS.empty()) {
                current = FCFS.front(); FCFS.pop();
            }
        } else {
            // Preemption: PQ preempts lower queues
            if (!PQ.empty() && current->queue_level != 2) {
                if (current->queue_level == 1) RQ.push(current);
                else if (current->queue_level == 3) FCFS.push(current);
                current = PQ.top(); PQ.pop();
                rr_quantum_left = quantum;
            }
        }

        if (current) {
            if (current->start_time == -1)
                current->start_time = max(time, current->arrival_time);

            current->remaining_time--;
            Gantt[time] = current->pid;
            time++;

            // HOW CPU BOUND VS I/O BOUND PROCESSES BEHAVE
            if (current->queue_level == 1) rr_quantum_left--;

            // Process finished
            if (current->remaining_time == 0) {
                current->finish_time = time;
                current->response_time = current->start_time - current->arrival_time;
                current->waiting_time = max(0, current->finish_time - current->arrival_time - current->burst_time);
                completed++;
                current = nullptr;
                rr_quantum_left = quantum;
            } else {
                //HOW PROCESSES MOVE BETWEEN QUEUES
                if (current->queue_level == 1 && rr_quantum_left == 0) {  //HOW CPU BOUND VS I/O BOUND PROCESSES BEHAVE
                    current->queue_level = 2; // move to Priority queue
                    PQ.push(current);
                    current = nullptr;
                    rr_quantum_left = quantum;
                }
            }
        } else {
            time++; // CPU idle
        }
    }

    // Sort by PID to match reference output
    sort(processes.begin(), processes.end(), [](Process &a, Process &b) { return a.pid < b.pid; });

    // Output in reference code format: PID Res_time F_time W_time
    for (auto &p : processes) {
        cout << p.pid << " " << p.response_time << " " << p.finish_time << " " << p.waiting_time << "\n";
    }

    return 0;
}
