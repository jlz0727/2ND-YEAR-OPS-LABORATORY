#include <iostream>
using namespace std;

int main()
{
    int p[20], bt[20], rem_bt[20], wt[20], tat[20];
    int i, n, tq, time = 0;
    float wtavg = 0, tatavg = 0;

    cout << "Enter the number of processes -- ";
    cin >> n;

    for (i = 0; i < n; i++)
    {
        p[i] = i;
        cout << "Enter Burst Time for Process " << i << " -- ";
        cin >> bt[i];
        rem_bt[i] = bt[i];
        wt[i] = 0;
    }

    cout << "Enter Time Quantum -- ";
    cin >> tq;

    // Round Robin Scheduling Logic
    int done;
    do
    {
        done = 1;
        for (i = 0; i < n; i++)
        {
            if (rem_bt[i] > 0)
            {
                done = 0;
                if (rem_bt[i] > tq)
                {
                    time += tq;
                    rem_bt[i] -= tq;
                }
                else
                {
                    time += rem_bt[i];
                    wt[i] = time - bt[i];
                    rem_bt[i] = 0;
                }
            }
        }
    } while (done == 0);

    for (i = 0; i < n; i++)
    {
        tat[i] = wt[i] + bt[i];
        wtavg += wt[i];
        tatavg += tat[i];
    }

    cout << "\n\tPROCESS\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\n";
    for (i = 0; i < n; i++)
    {
        cout << "\tP" << p[i]
             << "\t" << bt[i]
             << "\t\t" << wt[i]
             << "\t\t" << tat[i] << endl;
    }

    cout << "\nAverage Waiting Time -- " << wtavg / n;
    cout << "\nAverage Turnaround Time -- " << tatavg / n;

    return 0;
}
