#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

// THIS STRUCT DEFINES A PROCESS WITH NECESSARY ATTRIBUTES
struct Process {
    int id;
    float burstTime;
    float remainingTime;
    float waitingTime;
    float turnaroundTime;
    float completionTime;
};

int main() {
    // THIS LINE INITIALIZES THE PROCESS LIST
    vector<Process> processes = {
        {1, 50.54, 50.54, 0, 0, 0},
        {2, 10.11, 10.11, 0, 0, 0},
        {3, 33.34, 33.34, 0, 0, 0},
        {4, 2.5 , 2.5, 0, 0, 0},
        {5, 101.1, 101.1, 0, 0, 0}
    };

    // THIS IS THE TIME QUANTUM GIVEN FOR ROUND ROBIN SCHEDULING
    float timeQuantum = 20.0;
    float totalWT = 0, totalTAT = 0;
    int numProcesses = processes.size();

    queue<int> q;

    // THIS LINE ADDS ALL THE PROCESSES INTO THE QUEUE
    for (int i = 0; i < numProcesses; ++i) {
        q.push(i);
    }

    // THIS LINE STARTS THE ROUND ROBIN SCHEDULING SIMULATION
    int currentTime = 0;
    while (!q.empty()) {
        int idx = q.front();
        q.pop();

        Process &p = processes[idx];
        if (p.remainingTime > timeQuantum) {
            // THIS INDICATES THE PROCESS WILL USE THE FULL TIME QUANTUM
            p.remainingTime -= timeQuantum;
            currentTime += timeQuantum;
            
            // THIS LINE UPDATES WAITING TIME FOR OTHER PROCESSES
            for (int i = 0; i < numProcesses; ++i) { //CHECK ALL PROCESSES
                if (i != idx && processes[i].remainingTime > 0) { //Skip current process, only check OTHERS
                    processes[i].waitingTime += timeQuantum; // ADD TIME QUANTUM TO WAITING TIME
                }
            }
            q.push(idx);  // THIS LINE RE-ADDS THE PROCESS TO THE QUEUE
        } else {
            // THIS LINE INDICATES THE PROCESS WILL FINISH EXECUTION
            currentTime += p.remainingTime; // ADD REMAINING TIME TO CURRENT TIME
            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime;
            p.waitingTime = p.turnaroundTime - p.burstTime; // CALCULATE WAITING TIME
            totalWT += p.waitingTime; //ACCUMULATE
            totalTAT += p.turnaroundTime;
            p.remainingTime = 0; // PROCESS IS COMPLETE
        }
    }

    // THIS LINE NOW CALCULATES AVERAGE WAITING TIME AND TURNAROUND TIME
    float averageWT = totalWT / numProcesses;
    float averageTAT = totalTAT / numProcesses;

    // Print the result table
    cout << fixed << setprecision(2);
    cout << "--------+-----------+-------------+-----------------+-----------------\n";
    cout << "Process | Burst Time| Waiting Time| Turnaround Time | Completion Time \n";
    cout << "--------+-----------+-------------+-----------------+-----------------\n";

    // THIS LINE PRINTS THE DETAILS OF EACH PROCESS also spaces for lines in the output
    for (const auto& p : processes) {
        cout << setw(7) << p.id << " | " 
             << setw(9) << p.burstTime << " | " 
             << setw(11) << p.waitingTime << " | " 
             << setw(15) << p.turnaroundTime << " | " 
             << setw(15) << p.completionTime << "\n";
    }
    //THIS LINE PRINTS THE AVERAGE WAITING TIME AND TURNAROUND TIME
    cout << "--------+-----------+-------------+-----------------+-----------------\n";
    cout << "\n-----------------------------------------------------\n";
    cout << "Average Waiting Time (WT): " << averageWT << " ms\n";
    cout << "Average Turnaround Time (TAT): " << averageTAT << " ms\n";
    cout << "-----------------------------------------------------\n";

    return 0;
}
