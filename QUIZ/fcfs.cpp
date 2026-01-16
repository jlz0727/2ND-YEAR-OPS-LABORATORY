#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// THIS LINE DEFINES A PROCESS STRUCTURE
struct Process {
    int id;
    int burstTime;
    int waitingTime;
    int turnaroundTime;
    int completionTime;
};
 // THIS LINE IS LIST OF PROCESSES WITH THEIR BURST TIMES
int main() {
    vector<Process> processes = {
        {1, 3, 0, 0, 0},
        {2, 5, 0, 0, 0},
        {3, 4, 0, 0, 0},
        {4, 3, 0, 0, 0},
        {5, 2, 0, 0, 0},
        {6, 1, 0, 0, 0},
        {7, 0, 0, 0, 0},
        {8, 7, 0, 0, 0},
        {9, 8, 0, 0, 0},
        {10, 1, 0, 0, 0}
    };
    // THIS LINE INITIALIZES NECESSARY VARIABLES
    int numProcesses = processes.size();
    int totalWT = 0, totalTAT = 0;
    
    // THIS LINE CALCULATES COMPLETION TIME FOR EACH PROCESS
    processes[0].completionTime = processes[0].burstTime;
    // THIS LINE CALCULATES COMPLETION TIME FOR REMAINING PROCESSES
    for (int i = 1; i < numProcesses; ++i) {
        processes[i].completionTime = processes[i-1].completionTime + processes[i].burstTime; // FCFS LOGIC
    }

    // THIS LINE CALCULATES TURNAROUND TIME AND WAITING TIME FOR EACH PROCESS
    for (int i = 0; i < numProcesses; ++i) {
        processes[i].turnaroundTime = processes[i].completionTime; // SINCE ARRIVAL TIME IS 0
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
        // THIS LINE ACCUMULATES TOTAL WAITING TIME AND TURNAROUND TIME
        totalWT += processes[i].waitingTime;
        totalTAT += processes[i].turnaroundTime;
    }

    // THIS LINE CALCULATES AVERAGE WAITING TIME AND TURNAROUND TIME
    float averageWT = (float)totalWT / numProcesses;
    float averageTAT = (float)totalTAT / numProcesses;

    // THIS LINE DISPLAYS THE RESULTS IN A FORMATTED TABLE
    cout << fixed << setprecision(2);
    cout << "--------+-----------+-------------+-----------------+-----------------\n";
    cout << "Process | Burst Time| Waiting Time| Turnaround Time | Completion Time \n";
    cout << "--------+-----------+-------------+-----------------+-----------------\n";
    // THIS LINE PRINTS THE DETAILS OF EACH PROCESS ALSO SPACES FOR LINES IN THE OUTPUT
    for (const auto& p : processes) {
        cout << setw(7) << p.id << " | " 
             << setw(9) << p.burstTime << " | " 
             << setw(11) << p.waitingTime << " | " 
             << setw(15) << p.turnaroundTime << " | " 
             << setw(15) << p.completionTime << "\n";
    }
    // THIS LINE PRINTS THE AVERAGE WAITING TIME AND TURNAROUND TIME
    cout << "--------+-----------+-------------+-----------------+-----------------\n";
    cout << "\n----------------------------------------------------\n";
    cout << "Average Waiting Time (WT): " << averageWT << " ms\n";
    cout << "Average Turnaround Time (TAT): " << averageTAT << " ms\n";
    cout << "----------------------------------------------------\n";

    return 0;
}
