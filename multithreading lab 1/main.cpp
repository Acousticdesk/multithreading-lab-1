#include <thread>
#include <iostream>
#include <map>
#include <vector>
#include <chrono>
#include <mutex>

using namespace std;

int x;
int y;
int z;

int n = 0;

int MAX_ITERATIONS = 100;

bool finished = false;

mutex m;

typedef vector<string> Edges;

class Vertex {
    string value;
    vector<string> edges;
    
public:
    Vertex(string aValue) {
        value = aValue;
    }
    
    vector<string> getEdges() {
        Edges& edgs = this->edges;
        return edgs;
    }
    
    void setEdges(vector<string> aEdges) {
        for (string edge: aEdges) {
            this->edges.push_back(edge);
        }
    }
    string getValue() {
        return this->value;
    }
    int findEdge(string aEdge) {
        auto it = find_if(this->edges.begin(), this->edges.end(), [=](string edge){ return edge == aEdge; });
        
        if (it != this->edges.end()) {
            return distance(this->edges.begin(), it);
        }
        
        return -1;
    }
    void setValue(string v) {
        this->value = v;
    }
};

class Graph {
    vector<Vertex> vertecies;

public:
    void addVertex(string value) {
        Vertex v(value);
        
        this->vertecies.push_back(v);
    }
    int findVertex(string value) {
        auto isSubjectVertex = [=](Vertex v){ return v.getValue() == value; };
        auto it = find_if(this->vertecies.begin(), this->vertecies.end(), isSubjectVertex);
        
        if (it != this->vertecies.end()) {
            // element found
            return distance(this->vertecies.begin(), it);
        }
        
        return -1;
    }
    vector<Vertex>& getVertecies() {
        return this->vertecies;
    }
};

class StateMachine {
    Graph state;
    
public:
    void addState(string x) {
        m.lock();
        if (this->state.findVertex(x) == -1) {
            this->state.addVertex(x);
        }
        m.unlock();
    }
    
    void addTransition(string x, string transition) {
        m.lock();
        int foundAtIndex = this->state.findVertex(x);
        if (foundAtIndex != -1) {
            // state found
            vector<Vertex>& vtcs = this->state.getVertecies();
            Vertex& v = vtcs.at(foundAtIndex);
            if (v.findEdge(transition) == -1) {
                vector<string> transitionEdge{ transition };
                v.setEdges(transitionEdge);
            }
        }
        m.unlock();
    }
    
    Graph& getState() {
        return this->state;
    }
};

StateMachine sm;

void thread1() {
    string x1 = to_string(x);
    sm.addState(x1);
    
    x=0;
    
    sm.addTransition(x1, to_string(x));
    string x2 = to_string(x);
    sm.addState(x2);
    
    y=0;
    
    sm.addTransition(x2, to_string(x));
    string x3 = to_string(x);
    sm.addState(x3);
    
    z=0;
    
    sm.addTransition(x3, to_string(x));

    while (n < MAX_ITERATIONS) {
        string x4 = to_string(x);
        sm.addState(x4);
        
        x=1;
        
        sm.addTransition(x4, to_string(x));
        string x5 = to_string(x);
        sm.addState(x5);
        
        x=2;
        
        sm.addTransition(x5, to_string(x));
        string x6 = to_string(x);
        sm.addState(x6);
        
        x=z;
        
        sm.addTransition(x6, to_string(x));
        n++;
    }

    finished = true;
}

void thread2() {
    string x1 = to_string(x);
    sm.addState(x1);
    
    x=1;
    
    sm.addTransition(x1, to_string(x));
    string x2 = to_string(x);
    sm.addState(x2);
    
    y=1;
    
    sm.addTransition(x2, to_string(x));
    string x3 = to_string(x);
    sm.addState(x3);
    
    z=1;
    
    sm.addTransition(x3, to_string(x));

    while (n < MAX_ITERATIONS) {
        string x4 = to_string(x);
        sm.addState(x4);
        
        y=x;
        
        sm.addTransition(x4, to_string(x));
        string x5 = to_string(x);
        sm.addState(x5);
        
        y=3;
        
        sm.addTransition(x5, to_string(x));
        string x6 = to_string(x);
        sm.addState(x6);
        
        z=1;
        
        sm.addTransition(x6, to_string(x));
        n++;
    }

    finished = true;
}

void thread3() {
    string x1 = to_string(x);
    sm.addState(x1);
    
    x=2;
    
    sm.addTransition(x1, to_string(x));
    string x2 = to_string(x);
    sm.addState(x2);
    
    y=2;
    
    sm.addTransition(x2, to_string(x));
    string x3 = to_string(x);
    sm.addState(x3);
    
    z=2;
    
    sm.addTransition(x3, to_string(x));

    while (n < MAX_ITERATIONS) {
        string x4 = to_string(x);
        sm.addState(x4);
        
        z=2;
        
        sm.addTransition(x4, to_string(x));
        string x5 = to_string(x);
        sm.addState(x5);
        
        z=y;
        
        sm.addTransition(x5, to_string(x));
        string x6 = to_string(x);
        sm.addState(x6);
        
        x=1;
        
        sm.addTransition(x6, to_string(x));
        n++;
    }

    finished = true;
}

void thread4() {
    string x1 = to_string(x);
    sm.addState(x1);
    
    x=0;
    
    sm.addTransition(x1, to_string(x));
    string x2 = to_string(x);
    sm.addState(x2);
    
    y=1;
    
    sm.addTransition(x2, to_string(x));
    string x3 = to_string(x);
    sm.addState(x3);
    
    z=2;
    
    sm.addTransition(x3, to_string(x));

    while (n < MAX_ITERATIONS) {
        string x4 = to_string(x);
        sm.addState(x4);
        
        y=3;
        
        sm.addTransition(x4, to_string(x));
        string x5 = to_string(x);
        sm.addState(x5);
        
        y=5;
        
        sm.addTransition(x5, to_string(x));
        string x6 = to_string(x);
        sm.addState(x6);
        
        x=7;
        
        sm.addTransition(x6, to_string(x));
        n++;
    }

    finished = true;
}

void log() {
    while (!finished) {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    Graph& state = sm.getState();
    
    for (Vertex& v : state.getVertecies()) {
        cout << "Vertex: " << endl;
        cout << v.getValue() << endl;
        
        cout << "Edges: " << endl;
        for (string e : v.getEdges()) {
            cout << e << endl;
        }
        
        cout << "----------------" << endl;
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
