#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
using namespace std;

struct PCB {
    int pid;
    int at;
    int bt;
    int ct;
    int tt;
    int wt;
    int rbt;
};

/* ---------- RECOMMENDED TIME QUANTUM (MEDIAN BT) ---------- */
int recommendedTimeQuantum(vector<PCB> p) {
    vector<int> bt;

    cout << "\nBT VALUES: ";
    for (auto &x : p) {
        bt.push_back(x.bt);
        cout << x.bt << " ";
    }

    sort(bt.begin(), bt.end());

    cout << "\nSORTED BT: ";
    for (int x : bt)
        cout << x << " ";

    int n = bt.size();
    int tq;

    if (n % 2 == 0) {
        cout << "\nMEDIAN = (" << bt[n/2 - 1] << " + " << bt[n/2] << ") / 2";
        tq = (bt[n/2 - 1] + bt[n/2]) / 2;
    } else {
        cout << "\nMEDIAN = " << bt[n/2];
        tq = bt[n/2];
    }

    cout << "\nRECOMMENDED TIME QUANTUM = " << tq << "\n";
    return tq;
}

/* ---------- DISPLAY TABLE ---------- */
void display(string title, vector<PCB> p) {
    float avett = 0, avewt = 0;

    cout << "\n" << title << "\n";
    cout << "-------------------------------------------------------------\n";
    cout << "PID\tAT\tBT\tCT\tTT\tWT\n";
    cout << "-------------------------------------------------------------\n";

    for (auto &x : p) {
        x.tt = x.ct - x.at;
        x.wt = x.tt - x.bt;
        avett += x.tt;
        avewt += x.wt;

        cout << "P" << x.pid << "\t"
             << x.at << "\t"
             << x.bt << "\t"
             << x.ct << "\t"
             << x.tt << "\t"
             << x.wt << "\n";
    }

    cout << "-------------------------------------------------------------\n";
    cout << "AVE TT = " << avett / p.size() << endl;
    cout << "AVE WT = " << avewt / p.size() << endl;
}

/* ---------- ROUND ROBIN ---------- */
void roundRobin(vector<PCB> p, int tq) {
    int t = 0, done = 0, n = p.size();

    while (done < n) {
        bool exe = false;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= t && p[i].rbt > 0) {
                exe = true;

                if (p[i].rbt > tq) {
                    t += tq;
                    p[i].rbt -= tq;
                } else {
                    t += p[i].rbt;
                    p[i].rbt = 0;
                    p[i].ct = t;
                    done++;
                }
            }
        }

        if (!exe)
            t++;
    }

    display("ROUND ROBIN", p);
}

/* ---------- SJF (NON-PREEMPTIVE) ---------- */
void sjf(vector<PCB> p) {
    int t = 0, done = 0, n = p.size();

    while (done < n) {
        int idx = -1, mn = 9999;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= t && p[i].ct == 0 && p[i].bt < mn) {
                mn = p[i].bt;
                idx = i;
            }
        }

        if (idx != -1) {
            t += p[idx].bt;
            p[idx].ct = t;
            done++;
        } else {
            t++;
        }
    }

    display("SJF (NON-PREEMPTIVE)", p);
}

/* ---------- SRTF (PREEMPTIVE) ---------- */
void srtf(vector<PCB> p) {
    int t = 0, done = 0, n = p.size();

    while (done < n) {
        int idx = -1, mn = 9999;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= t && p[i].rbt > 0 && p[i].rbt < mn) {
                mn = p[i].rbt;
                idx = i;
            }
        }

        if (idx != -1) {
            p[idx].rbt--;
            t++;

            if (p[idx].rbt == 0) {
                p[idx].ct = t;
                done++;
            }
        } else {
            t++;
        }
    }

    sort(p.begin(), p.end(), [](PCB a, PCB b) {
        return a.pid < b.pid;
    });

    display("SRTF (PREEMPTIVE)", p);
}

/* ------------------ MAIN ------------------ */
int main() {
    vector<PCB> p(8);
 
    int at[8] = {4, 2, 3, 6, 1, 5, 8, 7};
    int bt[8] = {53, 43, 18, 16, 44, 73, 99, 27};

    for (int i = 0; i < 8; i++) {
        p[i].pid = i + 1;
        p[i].at = at[i];
        p[i].bt = bt[i];
        p[i].rbt = bt[i];
        p[i].ct = 0;
    }

    sort(p.begin(), p.end(), [](PCB a, PCB b) {
        return a.at < b.at;
    });

    int tq = recommendedTimeQuantum(p);

    roundRobin(p, tq);
    sjf(p);
    srtf(p);

    return 0;
}
