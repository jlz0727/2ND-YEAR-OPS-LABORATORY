#include <iostream>
using namespace std;

int main()
{
    int n, m;

    cout << "Enter number of processes: ";
    cin >> n;

    cout << "Enter number of resources: ";
    cin >> m;

    int alloc[n][m], max[n][m], need[n][m];
    int avail[m], work[m];
    bool finish[n];
    int safeSeq[n];

    cout << "\nEnter Allocation Matrix:\n";
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            cin >> alloc[i][j];

    cout << "\nEnter Maximum (Claim) Matrix:\n";
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            cin >> max[i][j];

    cout << "\nEnter Available Resources:\n";
    for(int i = 0; i < m; i++)
        cin >> avail[i];

    // Step 4: Calculate Need Matrix
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    // Initialize work and finish
    for(int i = 0; i < m; i++)
        work[i] = avail[i];

    for(int i = 0; i < n; i++)
        finish[i] = false;

    int count = 0;

    cout << "\n----- Checking System State -----\n";

    // Safety Algorithm
    while(count < n)
    {
        bool found = false;

        for(int i = 0; i < n; i++)
        {
            if(!finish[i])
            {
                bool possible = true;

                for(int j = 0; j < m; j++)
                {
                    if(need[i][j] > work[j])
                    {
                        possible = false;
                        break;
                    }
                }

                if(possible)
                {
                    cout << "\nAll resources can be allocated to Process P" << i+1 << endl;

                    // Add allocated resources back to work
                    for(int j = 0; j < m; j++)
                        work[j] += alloc[i][j];

                    cout << "Available resources after execution: ";
                    for(int j = 0; j < m; j++)
                        cout << work[j] << " ";
                    cout << endl;

                    finish[i] = true;
                    safeSeq[count] = i;
                    count++;

                    cout << "Process P" << i+1 << " executed successfully." << endl;
                    cout << "--------------------------------------------" << endl;

                    found = true;
                }
            }
        }

        if(!found)
            break;
    }

    if(count == n)
    {
        cout << "\nSystem is in SAFE state.\n";
        cout << "Safe sequence: ";
        for(int i = 0; i < n; i++)
            cout << "P" << safeSeq[i] + 1 << " ";
        cout << endl;
    }
    else
    {
        cout << "\nSystem is NOT in safe state.\n";
    }

    return 0;
}