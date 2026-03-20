#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>
#include <cstdlib>

using namespace std;

enum class AA { SYSTEM, INTERACTIVE, BATCH };
enum class BB { READY, RUNNING, BLOCKED, TERMINATED };

struct CC {
    int DD;
    AA EE;
    int FF;
    int GG;

    int HH;
    int II;
    BB JJ;

    vector<int> KK;
    vector<int> LL;
    vector<int> MM;

    CC(int pid, AA t, int cpuTime, const vector<int>& maxRes)
        : DD(pid), EE(t), FF(cpuTime), GG(cpuTime),
          HH(0), II(0), JJ(BB::READY),
          KK(maxRes), LL(maxRes.size(), 0), MM(maxRes) {}
};

class NN {
private:
    vector<int> available;
    vector<shared_ptr<CC>> allProcesses;

public:
    NN(const vector<int>& initialResources)
        : available(initialResources) {}

    void addProcess(shared_ptr<CC> p) {
        allProcesses.push_back(p);
    }

    bool requestResources(shared_ptr<CC> p, const vector<int>& request) {
        for (size_t i = 0; i < request.size(); ++i) {
            if (request[i] < 0 || request[i] > p->MM[i]) {
                return false;
            }
        }

        for (size_t i = 0; i < request.size(); ++i) {
            if (request[i] > available[i]) {
                p->JJ = BB::BLOCKED;
                return false;
            }
        }

        for (size_t i = 0; i < request.size(); ++i) {
            available[i] -= request[i];
            p->LL[i] += request[i];
            p->MM[i] -= request[i];
        }

        p->JJ = BB::READY;
        return true;
    }

    void releaseResources(shared_ptr<CC> p) {
        for (size_t i = 0; i < p->LL.size(); ++i) {
            available[i] += p->LL[i];
        }
    }

    vector<shared_ptr<CC>> tryUnblock() {
        vector<shared_ptr<CC>> wokeUp;

        for (auto& p : allProcesses) {
            if (p->JJ == BB::BLOCKED && p->MM[0] > 0) {
                vector<int> retryRequest(p->MM.size(), 0);
                for (size_t i = 0; i < retryRequest.size(); ++i) {
                    if (p->MM[i] > 0) {
                        retryRequest[i] = min(1, p->MM[i]);
                    }
                }

                bool canAlloc = true;
                for (size_t i = 0; i < retryRequest.size(); ++i) {
                    if (retryRequest[i] > available[i]) {
                        canAlloc = false;
                        break;
                    }
                }

                if (canAlloc && requestResources(p, retryRequest)) {
                    p->JJ = BB::READY;
                    wokeUp.push_back(p);
                }
            }
        }

        return wokeUp;
    }

    void printAvailable() const {
        cout << "Available: [";
        for (size_t i = 0; i < available.size(); ++i) {
            cout << available[i];
            if (i < available.size() - 1) cout << ", ";
        }
        cout << "]\n";
    }
};

class TT {
private:
    vector<vector<queue<shared_ptr<CC>>>> queues;
    vector<int> timeQuantum;
    vector<int> agingThreshold;

    int getTypeIndex(AA t) const {
        return (t == AA::SYSTEM) ? 0 :
               (t == AA::INTERACTIVE) ? 1 : 2;
    }

public:
    TT(int numLevels, const vector<int>& quanta, const vector<int>& aging)
        : timeQuantum(quanta), agingThreshold(aging) {
        queues.resize(3);
        for (auto& typeQueues : queues)
            typeQueues.resize(numLevels);
    }

    void addProcess(shared_ptr<CC> p) {
        p->HH = 0;
        p->II = 0;
        int typeIdx = getTypeIndex(p->EE);
        queues[typeIdx][0].push(p);
    }

    void requeueProcess(shared_ptr<CC> p) {
        int typeIdx = getTypeIndex(p->EE);
        queues[typeIdx][p->HH].push(p);
    }

    shared_ptr<CC> getNextProcess() {
        for (int typeIdx = 0; typeIdx < 3; ++typeIdx) {
            for (size_t level = 0; level < queues[typeIdx].size(); ++level) {
                auto& q = queues[typeIdx][level];
                if (!q.empty()) {
                    auto p = q.front();
                    q.pop();
                    return p;
                }
            }
        }
        return nullptr;
    }

    void aging() {
        for (int typeIdx = 0; typeIdx < 3; ++typeIdx) {
            for (size_t level = 1; level < queues[typeIdx].size(); ++level) {
                auto& fromQueue = queues[typeIdx][level];
                auto& toQueue = queues[typeIdx][level - 1];

                size_t size = fromQueue.size();
                for (size_t i = 0; i < size; ++i) {
                    auto p = fromQueue.front();
                    fromQueue.pop();

                    p->II++;
                    if ((size_t)p->II >= agingThreshold[level - 1]) {
                        p->HH = level - 1;
                        p->II = 0;
                        toQueue.push(p);
                    } else {
                        fromQueue.push(p);
                    }
                }
            }
        }
    }

    int getQuantum(int level) const {
        if (level >= 0 && level < (int)timeQuantum.size())
            return timeQuantum[level];
        return timeQuantum.back();
    }
};

int main() {
    cout << "====== Resource-Aware TT (MLQ + MLFQ + Banker's) ======\n\n";

    vector<int> resources = {15, 8};
    NN rm(resources);

    TT zz(3, {2, 4, 6}, {3, 6});

    auto p1 = make_shared<CC>(1, AA::SYSTEM, 10, vector<int>{5, 2});
    auto p2 = make_shared<CC>(2, AA::INTERACTIVE, 8, vector<int>{3, 1});
    auto p3 = make_shared<CC>(3, AA::BATCH, 12, vector<int>{4, 2});

    rm.addProcess(p1);
    rm.addProcess(p2);
    rm.addProcess(p3);

    zz.addProcess(p1);
    zz.addProcess(p2);
    zz.addProcess(p3);

    int timeStep = 0;
    int maxTime = 60;

    while (timeStep < maxTime) {
        cout << "\n--- Time Step " << timeStep << " ---\n";

        zz.aging();
        auto wokeUp = rm.tryUnblock();
        for (auto p : wokeUp) {
            cout << "CC " << p->DD << " unblocked\n";
            zz.requeueProcess(p);
        }

        auto current = zz.getNextProcess();

        if (!current) {
            cout << "No ready processes.\n";
            rm.printAvailable();
            timeStep++;
            continue;
        }

        current->JJ = BB::RUNNING;
        int quantum = zz.getQuantum(current->HH);

        string typeStr = (current->EE == AA::SYSTEM) ? "SYSTEM" :
                         (current->EE == AA::INTERACTIVE) ? "INTERACTIVE" : "BATCH";

        cout << "Running CC " << current->DD
             << " (" << typeStr << ", Level " << current->HH
             << ") for " << quantum << " time units\n";

        current->GG -= quantum;
        if (current->GG < 0)
            current->GG = 0;

        if (current->GG > 0 && (rand() % 100) < 30) {
            vector<int> request = {1, 0};
            cout << "Request resources: [1, 0]\n";

            if (!rm.requestResources(current, request)) {
                cout << " Request BLOCKED\n";
                timeStep++;
                continue;
            }
            cout << " Request GRANTED\n";
        }

        if (current->GG == 0) {
            current->JJ = BB::TERMINATED;
            rm.releaseResources(current);
            cout << "CC " << current->DD << " TERMINATED\n";
        } else {
            current->JJ = BB::READY;
            zz.requeueProcess(current);
        }

        rm.printAvailable();

        bool alldone = (p1->JJ == BB::TERMINATED) && (p2->JJ == BB::TERMINATED) && (p3->JJ == BB::TERMINATED);

        if (alldone) {
            cout << "All processes terminated. Ending simulation.\n";
            break;
        }

        timeStep++;
    }
    cout << "\n=== Simulation Finished ===\n";
    return 0;
}
