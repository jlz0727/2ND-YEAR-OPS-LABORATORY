#include <iostream>
#include <iomanip>
#include <string> // For to_string

using namespace std;

int main()
{
    int p[20];
    float bt[20], wt[20], tat[20];
    int i, k, n;
    float temp;
    float wtavg, tatavg;

    n = 5; // Number of processes

    // Hardcoded burst times
    bt[0] = 50.54;
    bt[1] = 10.11;
    bt[2] = 33.34;
    bt[3] = 2.5;
    bt[4] = 101.1;

    // Set process IDs
    for(i = 0; i < n; i++)
    {
        p[i] = i;
    }

    // Sort by burst time (SJF)
    for(i = 0; i < n; i++)
    {
        for(k = i + 1; k < n; k++)
        {
            if(bt[i] > bt[k])
            {
                // Swap burst times
                temp = bt[i];
                bt[i] = bt[k];
                bt[k] = temp;

                // Swap process IDs
                temp = p[i];
                p[i] = p[k];
                p[k] = temp;
                // temp is used for both burst time and process ID swapping
            }
        }
    }

    // Calculate waiting time and turnaround time
    wt[0] = 0;
    tat[0] = bt[0];

    for(i = 1; i < n; i++)
    {
        wt[i] = wt[i-1] + bt[i-1];
        tat[i] = tat[i-1] + bt[i];
    }

    // Calculate averages
    wtavg = 0;
    tatavg = 0;
    for(i = 0; i < n; i++)
    {
        wtavg += wt[i];
        tatavg += tat[i];
    }

    // Display results
    cout << fixed << setprecision(2);
    cout << "--------+-----------+-------------+-----------------\n";
    cout << "Process | Burst Time| Waiting Time| Turnaround Time \n";
    cout << "--------+-----------+-------------+-----------------\n";
    for(i = 0; i < n; i++)
    {
        cout << setw(7) << ("P" + to_string(p[i])) << " | " 
             << setw(9) << bt[i] << " | " 
             << setw(11) << wt[i] << " | " 
             << setw(15) << tat[i] << "\n";
    }
    cout << "--------+-----------+-------------+-----------------\n";

    cout << "\n--------------------------------------------------\n";
    cout << "Average Waiting Time -- " << (wtavg / n);
    cout << "\nAverage Turnaround Time -- " << (tatavg / n);
    cout << "\n--------------------------------------------------\n";

    return 0;
}
