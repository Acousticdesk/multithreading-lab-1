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

const int MAX_ITERATIONS = 100;

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
    
    string mInitiator;
        
public:
    StateMachine(int aX, int aY, int aZ, int aP1, int aP2, int aP3, int aP4, string aInitiator) {
        this->mX = aX;
        this->mY = aY;
        this->mZ = aZ;
        
        this->mP1 = aP1;
        this->mP2 = aP2;
        this->mP3 = aP3;
        this->mP4 = aP4;
        
        this->mInitiator = aInitiator;
    }
    
    void makeNextState(int aX, int aY, int aZ, int aP1, int aP2, int aP3, int aP4, string aInitiator) {
        m.lock();
        StateMachine s(aX, aY, aZ, aP1, aP2, aP3, aP4, aInitiator);
        this->mState.push_back(s);
        m.unlock();
    }
    
    vector<StateMachine> &getState() {
        return this->mState;
    }
    
    string toString() {
        return to_string(this->mX) + "|" + to_string(this->mY) + "|" + to_string(this->mZ) + "|" + to_string(this->mP1) + "|" + to_string(this->mP2) + "|" + to_string(this->mP3) + "|" + to_string(this->mP4);
    }
    
    int getX() {
        return this->mX;
    }
    
    void clearStatesWhereXDidNotChange() {
        for (int i = 0; i < this->mState.size(); i ++) {
            if (i == 0) {
                continue;
            }
            
            int xCurrent = this->mState.at(i).getX();
            int xPrevious = this->mState.at(i - 1).getX();
            
            if (xCurrent == xPrevious) {
                this->mState.erase(this->mState.begin() + i);
                // because we removed an element at i and shifted all the further elements positions
                // meaning that the next item is i -= 1
                i-=1;
            }
        }
    }
};

map<string, StateMachine> allStatesLookup;
map<string, vector<StateMachine>> stateTransitions;
map<string, vector<StateMachine>> stateTransitionsNoDuplicates;
//map<int, bool> singularStatesAdded;
map<string, bool> stateTransitionsAcknowledged;

// Privisioning the state machine with the inital state
StateMachine sm(0, 0, 0, 0, 0, 0, 0, "");

void thread1() {
    x=0;
    p1 = 11;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p11");
    
    y=0;
    p1 = 12;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p12");
    
    z=0;
    p1 = 13;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p13");
    
    while (n < MAX_ITERATIONS) {
        x=1;
        p1 = 14;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p14");
        
        x=2;
        p1 = 15;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p15");
        
        x=z;
        p1 = 16;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p16");
        
        n++;
    }

    finished = true;
}

void thread2() {
    x=1;
    p2 = 21;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p21");
    
    y=1;
    p2 = 22;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p22");
    
    z=1;
    p2 = 23;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p23");
    
    while (n < MAX_ITERATIONS) {
        y=x;
        p2 = 24;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p24");
        
        y=3;
        p2 = 25;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p25");
        
        z=1;
        p3 = 26;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p26");
        
        n++;
    }

    finished = true;
}

void thread3() {
    x=2;
    p3 = 31;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p31");
    
    y=2;
    p3 = 32;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p32");
    
    z=2;
    p3 = 33;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p33");

    while (n < MAX_ITERATIONS) {
        z=2;
        p3 = 34;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p34");
        
        z=y;
        p3 = 35;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p35");
        
        x=1;
        p3 = 36;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p36");
        
        n++;
    }

    finished = true;
}

void thread4() {
    x=0;
    p4 = 41;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p41");
    
    y=1;
    p4 = 42;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p42");
    
    z=2;
    p4 = 43;
    sm.makeNextState(x, y, z, p1, p2, p3, p4, "p43");
    
    while (n < MAX_ITERATIONS) {
        y=3;
        p4 = 44;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p44");
        
        y=5;
        p4 = 45;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p45");
        
        x=7;
        p4 = 46;
        sm.makeNextState(x, y, z, p1, p2, p3, p4, "p46");
        
        n++;
    }

    finished = true;
}

void finalize() {
    while (!finished) {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    
    // create state transition map (keep track of all the next states for a specific X)
    // [{}, {}, ..., {}] => { [operation_id]: [{}, {}] }
    vector<StateMachine> &s = sm.getState();
    sm.clearStatesWhereXDidNotChange();
    
    for (int i = 0; i < s.size(); i++) {
        // we are not interested in the last item
        if (i == s.size() - 1) {
            continue;
        }

        string stateKey = s.at(i).toString();

        // not found
        if (stateTransitions.find(stateKey) == stateTransitions.end()) {
            vector<StateMachine> sv;
            stateTransitions[stateKey] = sv;
        }

        // compact possible next states (remove duplicates)
        if (find_if(stateTransitions[stateKey].begin(), stateTransitions[stateKey].end(), [&](StateMachine smn) { return smn.getX() == s.at(i + 1).getX(); }) == stateTransitions[stateKey].end())
        {
            // Element not in vector.
            stateTransitions[stateKey].push_back(s.at(i + 1));
        }
    }
    
    // create states lookup (we will need it to properly remove the regex groups duplicates)
    // also clear repetitive operations as we don't need repetitions
    for (int i = 0; i < s.size(); i++) {
        if (allStatesLookup.find(s[i].toString()) != allStatesLookup.end()) {
            // item already exists
            s.erase(s.begin() + i);
            // because we removed an element at i and shifted all the further elements positions
            // meaning that the next item is i -= 1
            i-=1;
            continue;
        }
        allStatesLookup.insert(pair(s[i].toString(), s[i]));
    }
    
    // remove all the regex group duplicates
    for (auto it = stateTransitions.begin(); it != stateTransitions.end(); it++)
    {
        string currentStateKey = it->first;
        auto itAllStates = allStatesLookup.find(currentStateKey);
        int currentStateX = -1;
        
        if (itAllStates != allStatesLookup.end()) {
            currentStateX = itAllStates->second.getX();
        }
        
        if (currentStateX == -1) {
            continue;
        }
        
        int numXTransitionOccurrences = 0;
        
        for (int i = 0; i < s.size(); i++) {
            if (s[i].getX() == currentStateX) {
                numXTransitionOccurrences += 1;
            }
        }
        
        // the only transition from this X
        if (numXTransitionOccurrences == 1) {
            stateTransitionsNoDuplicates[currentStateKey] = stateTransitions[currentStateKey];
            continue;
        }
        
        bool hasDuplicates = false;
        
        // 2. okay, we found the X, let's see if there are elements that transition into the exact same values
        vector<StateMachine> nextTransitionsFromCurrentState = it->second;
        map<string, bool> stateTransitionsWithTheSameX;
        
        // filter stateTransitions and pick the ones with the correct X
        for (int i = 0; i < s.size(); i++) {
            if (s[i].getX() == currentStateX && s[i].toString() != currentStateKey) {
                stateTransitionsWithTheSameX.insert(pair<string, bool>(s[i].toString(), true));
            }
        }
        
        for (auto it2 = stateTransitions.begin(); it2 != stateTransitions.end(); it2++) {
            string otherStateKey = it2->first;
            
            if (!stateTransitionsWithTheSameX[otherStateKey]) {
                continue;
            }
            
            // this is the same state we are comparing with, continue
            if (otherStateKey == currentStateKey) {
                continue;
            }
            
            vector<StateMachine> &nextTransitionsFromOtherState = it2->second;
            
            // 3. the other state contains different values transition, add it to the list
            if (nextTransitionsFromOtherState.size() != nextTransitionsFromOtherState.size()) {
                continue;
            }
            
            vector<StateMachine> nextTransitionsFromCurrentStateCopy = nextTransitionsFromCurrentState;
            vector<StateMachine> nextTransitionsFromOtherStateCopy = nextTransitionsFromOtherState;
            
            string nextTransitionsFromCurrentStateString = "";
            string nextTransitionsFromOtherStateString = "";
            
            sort(nextTransitionsFromCurrentStateCopy.begin(), nextTransitionsFromCurrentStateCopy.end(), [&](StateMachine sm1, StateMachine sm2) { return sm1.getX() < sm2.getX(); });
            sort(nextTransitionsFromOtherStateCopy.begin(), nextTransitionsFromOtherStateCopy.end(), [&](StateMachine sm1, StateMachine sm2) { return sm1.getX() < sm2.getX(); });
            
            for (int j = 0; j < nextTransitionsFromCurrentStateCopy.size(); j ++) {
                nextTransitionsFromCurrentStateString += to_string(nextTransitionsFromCurrentStateCopy[j].getX());
            }
            
            for (int j = 0; j < nextTransitionsFromOtherStateCopy.size(); j ++) {
                nextTransitionsFromOtherStateString += to_string(nextTransitionsFromOtherStateCopy[j].getX());
            }
            
            string stateAcknowledgementKey = to_string(currentStateX) + "_" + nextTransitionsFromCurrentStateString;
            bool isTransitionAcknowledgedAlready = stateTransitionsAcknowledged[stateAcknowledgementKey];
            cout << "is Acknowledged already: " << endl;
            cout << to_string(isTransitionAcknowledgedAlready) << endl;
            stateTransitionsAcknowledged[stateAcknowledgementKey] = true;
            
            if (!isTransitionAcknowledgedAlready) {
                stateTransitionsNoDuplicates[currentStateKey] = stateTransitions[currentStateKey];
                break;
            }
            
            // 4. the states are different, add the current state to the list
            if (nextTransitionsFromCurrentStateString == nextTransitionsFromOtherStateString) {
                hasDuplicates = true;
                break;
            }
            
            cout << "is not marked as duplicate" << endl;
        }
        
        if (!hasDuplicates) {
            cout << "added" << endl;
            stateTransitionsNoDuplicates[currentStateKey] = stateTransitions[currentStateKey];
        }
    }
    
    string regEx = "^";
    
    // create a regular expression
    for (int j = 0; j < s.size(); j++) {
        StateMachine smn = s.at(j);
        
        // all the possible next states
        if (stateTransitionsNoDuplicates.find(smn.toString()) != stateTransitionsNoDuplicates.end()) {
            cout << "Adding an item to the regex:" << endl;
            cout << smn.toString() << endl;
            cout << "x: " << smn.getX() << endl;
            cout << "Which transfers to:" << endl;
            string debugString2 = "";
            for (int k = 0; k < stateTransitionsNoDuplicates[smn.toString()].size(); k++) {
                debugString2 += to_string(stateTransitionsNoDuplicates[smn.toString()][k].getX());
                debugString2 += " ";
            }
            cout << debugString2 << endl;
            cout << "--------------------------------" << endl;
            // move reason: previously was out of scope of this if, but we need to check if it is not a duplicated item
            regEx += "(";
            regEx += to_string(smn.getX());
            regEx += "(";
            
            for (int i = 0; i < stateTransitionsNoDuplicates[smn.toString()].size(); i++) {
                regEx += to_string(stateTransitionsNoDuplicates[smn.toString()].at(i).getX());
                if (i != stateTransitionsNoDuplicates[smn.toString()].size() - 1) {
                    regEx += "|";
                }
            }
            
            regEx += ")";
            regEx += ")";
            
            if (j != s.size() - 1) {
                regEx += "|";
            }
        } else {
            // if the last item should not be added - remove the last pipe in the regex
            if (j == s.size() - 1) {
                regEx = regEx.substr(0, regEx.length() - 2);
            }
        }
    }
    
    regEx += "$";
    
    // write the result into the console
    cout << "The regex: " << regEx << endl;
}


int main() {
    thread t_thread1(thread1);
    thread t_thread2(thread2);
    thread t_thread3(thread3);
    thread t_thread4(thread4);
    thread finalizeThread(finalize);

    t_thread1.detach();
    t_thread2.detach();
    t_thread3.detach();
    t_thread4.detach();
    finalizeThread.detach();

    cin.get();
}
