#include <thread>
#include <iostream>
#include <map>
#include <vector>
#include <chrono>

using namespace std;

int x;
int y;
int z;

int n = 0;

int MAX_ITERATIONS = 1000000;

class Graph {
    map<int, vector<int>> state;

public:
    void addEdge(int currentX, int nextX)
    {
        // there is no such x yet
        if (!this->state.count(currentX))
        {
            vector<int> v;
            this->state[currentX] = v;
        }
        
        if (!count(this->state[currentX].begin(), this->state[currentX].end(), nextX))
        {
            this->state[currentX].push_back(nextX);
        }
    }
    
    map<int, vector<int>> getState() {
        return this->state;
    }
};

Graph g;

void thread1() {
    g.addEdge(x, 0);
    x=0; y=0; z=0;
    
    while (n < MAX_ITERATIONS) {
        g.addEdge(x, 1);
        x=1;
        g.addEdge(x, 2);
        x=2;
        g.addEdge(x, z);
        x=z;
        n++;
        
    }
}

void thread2() {
    g.addEdge(x, 1);
    x=1; y=1; z=1;
    
    while (n < MAX_ITERATIONS) {
        y=x;
        y=3;
        z=1;
        n++;
    }
}

void thread3() {
    g.addEdge(x, 2);
    x=2; y=2; z=2;
    while (n < MAX_ITERATIONS) {
        z=2;
        z=y;
        g.addEdge(x, 0);
        x=1;
        n++;
    }
}

void thread4() {
    x=0; y=1; z=2;
    
    while (n < MAX_ITERATIONS) {
        y=3;
        y=5;
        g.addEdge(x, 7);
        x=7;
        n++;
    }
}

void log() {
    while (n < MAX_ITERATIONS) {
        
        this_thread::sleep_for(chrono::milliseconds(1000));
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
        
    return 0;
}
