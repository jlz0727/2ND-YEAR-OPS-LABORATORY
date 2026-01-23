#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <algorithm>

using namespace std;

// PROCESS STRUCT
struct Process {
    int id;
    float arrivalTime;
    float burstTime;
    float remainingTime;
    float waitingTime;
    float turnaroundTime;
    float completionTime;
};

// GANTT BLOCK
struct GanttBlock {
    int pid;
    float start;
    float end;
};

// PRINT ARRIVAL TIMELINE (HORIZONTAL)
void printArrivalTimeline(const vector<Process>& processes) {

    cout << "\n============= ARRIVAL TIMELINE =============\n\n";
    cout << fixed << setprecision(2);

    for (const auto& p : processes)
        cout << setw(10) << p.arrivalTime << "(AT)";
    cout << "\n";

    for (size_t i = 0; i < processes.size(); i++)
        cout << setw(13) << "|";
    cout << "\n";

    for (const auto& p : processes)
        cout << setw(13) << ("P" + to_string(p.id));
    cout << "\n\n============================================\n";
}

// PRINT GANTT CHART
void printGanttChart(const vector<GanttBlock>& gantt) {

    cout << "\n==================== GANTT CHART ====================\n|";
    for (const auto& g : gantt)
        cout << " P" << g.pid << " |";
    cout << "\n";

    cout << fixed << setprecision(2);
    cout << gantt.front().start;
    for (const auto& g : gantt)
        cout << setw(6) << g.end;

    cout << "\n=====================================================\n";
}

int main() {

    vector<Process> processes = {
        {1, 0.0,  50.54, 50.54, 0, 0, 0},
        {2, 5.0,  10.11, 10.11, 0, 0, 0},
        {3, 12.0, 33.34, 33.34, 0, 0, 0},
        {4, 20.0, 2.50,  2.50,  0, 0, 0},
        {5, 25.0, 101.1, 101.1, 0, 0, 0}
    };

    float timeQuantum = 20.0;
    float currentTime = 0;
    int completed = 0;
    int n = processes.size();

    queue<int> q;
    vector<bool> inQueue(n, false);
    vector<GanttBlock> gantt;

    // SORT BY ARRIVAL TIME
    sort(processes.begin(), processes.end(),
         [](const Process& a, const Process& b) {
             return a.arrivalTime < b.arrivalTime;
         });

    // ROUND ROBIN SCHEDULING
    while (completed < n) {

        // ADD ARRIVED PROCESSES
        for (int i = 0; i < n; i++) {
            if (!inQueue[i] &&
                processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime > 0) {
                q.push(i);
                inQueue[i] = true;
            }
        }

        // CPU IDLE
        if (q.empty()) {
            currentTime++;
            continue;
        }

        int idx = q.front();
        q.pop();
        Process &p = processes[idx];

        float start = currentTime;
        float execTime = min(timeQuantum, p.remainingTime);

        p.remainingTime -= execTime;
        currentTime += execTime;

        gantt.push_back({p.id, start, currentTime});

        // ADD NEW ARRIVALS DURING EXECUTION
        for (int i = 0; i < n; i++) {
            if (!inQueue[i] &&
                processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime > 0) {
                q.push(i);
                inQueue[i] = true;
            }
        }

        if (p.remainingTime > 0) {
            q.push(idx);
        } else {
            completed++;
            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
        }
    }

    // ================= PROCESS TABLE =================
    cout << fixed << setprecision(2);
    cout << "\n--------+---------+-----------+-------------+-----------------+-----------------\n";
    cout << "Process | Arrival | Burst Time| Waiting Time| Turnaround Time | Completion Time\n";
    cout << "--------+---------+-----------+-------------+-----------------+-----------------\n";

    float totalWT = 0, totalTAT = 0;
    for (const auto& p : processes) {
        totalWT += p.waitingTime;
        totalTAT += p.turnaroundTime;

        cout << setw(7) << p.id << " | "
             << setw(7) << p.arrivalTime << " | "
             << setw(9) << p.burstTime << " | "
             << setw(11) << p.waitingTime << " | "
             << setw(15) << p.turnaroundTime << " | "
             << setw(15) << p.completionTime << "\n";
    }

    cout << "--------+---------+-----------+-------------+-----------------+-----------------\n";
    cout << "\nAverage Waiting Time: " << totalWT / n << " ms\n";
    cout << "Average Turnaround Time: " << totalTAT / n << " ms\n";

    // PRINT TIMELINES
    printArrivalTimeline(processes);
    printGanttChart(gantt);

    return 0;
}
