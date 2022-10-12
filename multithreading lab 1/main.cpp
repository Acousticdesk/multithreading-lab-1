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

int MAX_ITERATIONS = 1000000;

bool finished = false;

mutex m;

class Vertex {
    string value;
    vector<string> edges;
    
public:
    Vertex(string aValue) {
        value = aValue;
    }
    
    vector<string> getEdges() {
        return this->edges;
    }
    
    void setEdges(vector<string> aEdges) {
        for (string edge: aEdges) {
            this->edges.push_back(edge);
        }
    }
    string getValue() {
        return this->value;
    }
};

// State Machine as a Graph representation
class Graph {
    // map<int, vector<int>> state;
    vector<Vertex> vertecies;

public:
    Vertex addVertex(string value) {
        Vertex v(value);
        
        vertecies.push_back(v);
        
        return v;
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
};

void thread1() {
    x=0; y=0; z=0;

    while (n < MAX_ITERATIONS) {
        x=1;
        x=2;
        x=z;
        n++;
    }

    finished = true;
}

void thread2() {
    x=1; y=1; z=1;

    while (n < MAX_ITERATIONS) {
        y=x;
        y=3;
        z=1;
        n++;
    }

    finished = true;
}

void thread3() {
    x=2; y=2; z=2;
    while (n < MAX_ITERATIONS) {
        z=2;
        z=y;
        x=1;
        n++;
    }

    finished = true;
}

void thread4() {
    x=0; y=1; z=2;

    while (n < MAX_ITERATIONS) {
        y=3;
        y=5;
        x=7;
        n++;
    }

    finished = true;
}

//void log() {
//    while (!finished) {
//        this_thread::sleep_for(chrono::milliseconds(1000));
//    }
//
//    this_thread::sleep_for(chrono::milliseconds(1000));
//
//    map<int, vector<int>> state = g.getState();
//
//    for(auto const& key: state) {
//        cout << key.first << ": " << endl;
//        for (int i = 0; i < state[key.first].size(); i++) {
//            cout << state[key.first][i] << endl;
//        }
//    }
//}


int main() {
    thread t_thread1(thread1);
    thread t_thread2(thread2);
    thread t_thread3(thread3);
    thread t_thread4(thread4);
//    thread logThread(log);

    t_thread1.detach();
    t_thread2.detach();
    t_thread3.detach();
    t_thread4.detach();
//    logThread.detach();
    
//    Graph g;
//
//    Vertex v = g.addVertex("1");
//
//    vector<string> edges{"1"};
//    v.setEdges(edges);
//
//    cout << g.findVertex("2") << endl;
        
    cin.get();
}
