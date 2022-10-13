#include <thread>
#include <iostream>
#include <map>
#include <vector>
#include <chrono>
#include <mutex>

using namespace std;

// the current states of the variables
int x;
int y;
int z;

// Ids of the currently processed operation
int p1;
int p2;
int p3;
int p4;

// total number of operations taken (is being used to track how many more operations we need to take before exiting)
int n = 0;

const int MAX_ITERATIONS = 10;

// whether or not we stopped processing the threads
bool finished = false;

mutex m;

class StateMachine {
    vector<StateMachine> mState;
    
    int mX;
    int mY;
    int mZ;
    
    int mP1;
    int mP2;
    int mP3;
    int mP4;
    
public:
    StateMachine(int aX, int aY, int aZ, int aP1, int aP2, int aP3, int aP4) {
        this->mX = aX;
        this->mY = aY;
        this->mZ = aZ;
        
        this->mP1 = aP1;
        this->mP2 = aP2;
        this->mP3 = aP3;
        this->mP4 = aP4;
    }
    
    void makeNextState(int aX, int aY, int aZ, int aP1, int aP2, int aP3, int aP4) {
        m.lock();
        StateMachine s(aX, aY, aZ, aP1, aP2, aP3, aP4);
        this->mState.push_back(s);
        m.unlock();
    }
    
    vector<StateMachine> &getState() {
        return this->mState;
    }
    
    string toString() {
        return to_string(this->mX) + "|" + to_string(this->mY) + "|" + to_string(this->mZ) + "|" + to_string(this->mP1) + "|" + to_string(this->mP2) + "|" + to_string(this->mP3) + "|" + to_string(this->mP4);
    }
};

// Privisioning the statem machine with the inital state
StateMachine sm(0, 0, 0, 0, 0, 0, 0);

void thread1() {
    x=0;
    p1 = 11;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);
    
    y=0;
    p1 = 12;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);
    
    z=0;
    p1 = 13;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);
    
    while (n < MAX_ITERATIONS) {
        x=1;
        p1 = 14;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        x=2;
        p1 = 15;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        x=z;
        p1 = 16;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        n++;
    }

    finished = true;
}

void thread2() {
    x=1;
    p2 = 21;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);
    
    y=1;
    p2 = 22;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);
    
    z=1;
    p2 = 23;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);
    
    while (n < MAX_ITERATIONS) {
        y=x;
        p2 = 24;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        y=3;
        p2 = 25;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        z=1;
        p3 = 26;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        n++;
    }

    finished = true;
}

void thread3() {
    x=2;
    p3 = 31;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);
    
    y=2;
    p3 = 32;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);
    
    z=2;
    p3 = 33;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);

    while (n < MAX_ITERATIONS) {
        z=2;
        p3 = 34;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        z=y;
        p3 = 35;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        x=1;
        p3 = 36;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        n++;
    }

    finished = true;
}

void thread4() {
    x=0;
    p4 = 41;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);
    
    y=1;
    p4 = 42;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);
    
    z=2;
    p4 = 43;
    sm.makeNextState(x, y, z, p1, p2, p3, p4);
    
    while (n < MAX_ITERATIONS) {
        y=3;
        p4 = 44;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        y=5;
        p4 = 45;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        x=7;
        p4 = 46;
        sm.makeNextState(x, y, z, p1, p2, p3, p4);
        
        n++;
    }

    finished = true;
}

void log() {
    while (!finished) {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    
    vector<StateMachine> &s = sm.getState();
    
    cout << "All the captured states:" << endl;
    for (StateMachine smn : s) {
        cout << smn.toString() << endl;
    }
}


int main() {
    thread t_thread1(thread1);
    thread t_thread2(thread2);
    thread t_thread3(thread3);
    thread t_thread4(thread4);
    thread logThread(log);

    t_thread1.detach();
    t_thread2.detach();
    t_thread3.detach();
    t_thread4.detach();
    logThread.detach();

    cin.get();
}
