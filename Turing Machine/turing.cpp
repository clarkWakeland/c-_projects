#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <unistd.h>
#include <vector>

using namespace std;

struct DFA {
    string alphabet;
    string tapeAlphabet;
    string bigAlph;
    string * states;
    string initial;
    string accept;
    string reject;
    string * final;
    string * transititions;
    string * writeStates;
    string * moveStates;
    string tape = "E";
    int tapeHeadIndex = 1;
};

int findStateIndex(const DFA& dfa, const string& state, int size) {
    
    for (int i = 0; i < size; i++){
        if (dfa.states[i] == state){
            return i;
        }
    }
    
    return -1;
}

string removeSpaces(string s) {
    string r;

    for (char c: s) {
        if (c!= ' ') {
            r += c;
        }
    }
    return r;
}

int findNumStates(string s){

    int size = 0;
    bool startString = true;

    for (char c: s) {
        if (c== ' '){
            startString = true;
        } else if (startString){
            size++;
            startString = false;
        }
    }

    return size;
}

string * createStatesArray(string s, int size) {
    

    string * states = new string[size];

    int i = 0;
    bool startString = true;
    for (char c: s) {
        if (c== ' '){
            if (!startString){
                i++;
            }
            startString = true;
        } else {
            states[i] += c;
            startString = false;
        }
    }

    return states;

} 



int createDFA(DFA& dfa, string file){

    ifstream f(file);

    if (!f.is_open()) {
        cerr << "Error opening file" << endl;
        return -1;
    }

    int stateSize = -1;
    string line;
    string alph = "alphabet";
    string dfAlp;
    int count = 0;
    
    //Get alphabet
    getline(f, line);
    if (line == "-- alphabet") {
            getline(f, line);
            string c = removeSpaces(line);
            dfa.alphabet = c;
    } else{
        cout << "Invalid file.txt";
        f.close();
        return -1;
    }

    //Get tape alphabet
    getline(f, line);
    if (line == "-- tape alphabet") {
            getline(f, line);
            string tc = removeSpaces(line);
            dfa.tapeAlphabet = tc;
    } else{
        cout << "Invalid file.txt";
        f.close();
        return -1;
    }

    getline(f,line);

    //Get array of states
    if (line == "-- states"){
        getline(f,line);
        stateSize = findNumStates(line);
        dfa.states = createStatesArray(line,stateSize);
        
    } else{
        cout << "Invalid file.txt";
        f.close();
        return -1;
    }

    getline(f,line);

    //Get initial state
    if (line == "-- initial"){
        getline(f,line);
        dfa.initial = line;
    } else{
        cout << "Invalid file.txt";
        f.close();
        return -1;
    }

    getline(f,line);

    //Get Accept state
    if (line == "-- accept"){
        getline(f,line);
        dfa.accept = line;
    } else{
        cout << "Invalid file.txt";
        f.close();
        return -1;
    }


    getline(f,line);

    //Get Reject state
    if (line == "-- reject"){
        getline(f,line);
        dfa.reject = line;
    } else{
        cout << "Invalid file.txt";
        f.close();
        return -1;
    }

    getline(f,line);


    if (line == "-- transitions"){
        dfa.bigAlph = dfa.alphabet + dfa.tapeAlphabet;
        int bigAlphSize = dfa.bigAlph.size();
        string * tArray = new string[stateSize * bigAlphSize];
        string * wArray = new string[stateSize * bigAlphSize];
        string * hArray = new string[stateSize * bigAlphSize];
        
        for (int a = 0; a < bigAlphSize*stateSize; a++) {
            getline(f,line);

            bool startString = true;
            string * tstates = new string[5];
            int i = 0;
            
            for (char c: line) {
                if (c== ' ' || c== '-' || c=='>' || c==','){
                    if (!startString){
                        i++;
                    }
                    startString = true;
                } else {
                    tstates[i] += c;
                    startString = false;
                }
            }

            string stSt = tstates[0];
            string w = tstates[1];
            string writeChar = tstates[2];
            string moveHead = tstates[3];
            string nSt = tstates[4];

            int stId = findStateIndex(dfa, stSt, stateSize);

            int wId = dfa.bigAlph.find(w);

            int nId = findStateIndex(dfa, nSt, stateSize);

            int tId1 = dfa.tapeAlphabet.find(writeChar);
            int tId2 = dfa.alphabet.find(writeChar);

            if (tId1 == -1 && tId2 == -1){
                cout << "Error: Letter to write to tape was not found in tape alphabet";
                f.close();
                return -1;
            }
            
            if (stId == -1 || nId == -1 || wId == -1){
                cout << "Error: Transition State or letter not found in state space or alphabet";
                f.close();
                return -1;
            } 

            tArray[stId*bigAlphSize + wId] = nSt;
            wArray[stId*bigAlphSize + wId] = writeChar;
            hArray[stId*bigAlphSize + wId] = moveHead;
                    
        }
        dfa.transititions = tArray;
        dfa.writeStates = wArray;
        dfa.moveStates = hArray;
        //cout << '\n';

        /*for (int q = 0; q < stateSize * dfa.alphabet.size(); q++){
            cout << dfa.transititions[q] << '\n';
        } */

    } else{
        cout << "Invalid file.txt";
        f.close();
        return -1;
    }
    f.close();
    return stateSize;
    

}
void deleteDFAMemory(DFA &dfa){

    delete [] dfa.states;
    delete [] dfa.transititions;

    dfa.states = nullptr;
    dfa.transititions = nullptr;
}
string automaton(string file, string input) {

    DFA dfa;
    dfa.tape += input;
    int stateSize = createDFA(dfa,file);

    
    if (stateSize < 0) {
        return "REJECT";
    }

    string curState = dfa.initial;
    if (curState == dfa.accept){
            return dfa.tape;
        }
    if (curState == dfa.reject) {
        return "REJECT";
    }

    std::vector<char> vec(2, ' ');
    char arrow = '^';

    while (true){
        char sym = dfa.tape[dfa.tapeHeadIndex];
        int stateInd = findStateIndex(dfa, curState, stateSize);
        int symInd = dfa.bigAlph.find(sym);

        if (stateInd == -1 || symInd == -1){
            // character in input string is not found
            return "REJECT";
        }
        
        int indOfInterest = stateInd*dfa.bigAlph.size() + symInd;
        string nextState = dfa.transititions[indOfInterest];
        string toWrite = dfa.writeStates[indOfInterest];
        string dir = dfa.moveStates[indOfInterest];
        if (dfa.tapeHeadIndex == vec.size()){
            vec.push_back(' ');    
        }

        vec[dfa.tapeHeadIndex] = arrow;
        /*Debugging        
        cout << "\n" << curState << "\n";
        cout << "Head Index: " << dfa.tapeHeadIndex << "\n"; 
        cout << dfa.tape;
        */
        cout << "\033[A\r" << dfa.tape << std::flush << '\n';
        cout << '\r';

        for (char c: vec){
            cout << c << std::flush;
        }
        vec[dfa.tapeHeadIndex] = ' ';

        usleep(100000);
        dfa.tape[dfa.tapeHeadIndex] = toWrite[0];

        if (dir == "R") {
            dfa.tapeHeadIndex += 1;
        } else{
            dfa.tapeHeadIndex -= 1;
        }

        if (dfa.tapeHeadIndex >= dfa.tape.size()){
            dfa.tape += "w";
        }

        
        
        
        curState = nextState;
        if (curState == dfa.accept){
            deleteDFAMemory(dfa);
            return dfa.tape;
        }
        if (curState == dfa.reject){
            deleteDFAMemory(dfa);
            return "REJECT";
        }
        // cout << "Current State is: " << curState << "\n";

    }

    //TODO: Edit for Turing state


    if(dfa.reject==curState){
            deleteDFAMemory(dfa);
            return "REJECT";
        }
    if (dfa.accept==curState){
        deleteDFAMemory(dfa);
        return dfa.tape;
    }
    deleteDFAMemory(dfa);

    return "REJECT";

    
}

int main() {

    cout << "###################### Testing turing machine ######################\n";
    const char * path = "square.dat";
    const char * iTape = "aaa";

    cout << "Input tape:" << iTape << "\n\n\n";

    string t = automaton(path,iTape);
    if ( t == "REJECT") {
        cout << "REJECT\n";
        //return 1;
    } else{
        cout << "\n" << t;
        cout << "\npass\n";
    }

    /*cout << "Test 1 a should be on tape (Should pass)";
    
    t = automaton("C:\\Users\\clark\\Personal Repos\\aero552\\hw3\\square.dat","a");
    if ( t == "REJECT") {
        cout << "REJECT\n";
        //return 1;
    } else{
        cout << "\n" << t;
        cout << "\npass\n";
    }

    cout << "Test 36 a's should be on tape (Should pass)";
    
    t = automaton("C:\\Users\\clark\\Personal Repos\\aero552\\hw3\\square.dat","aaaaaa");
    if ( t == "REJECT") {
        cout << "REJECT\n";
        //return 1;
    } else{
        cout << "\n" << t;
        cout << "\npass\n";
    }

    cout << "Test string contains reject character (Reject)";

    t = automaton("C:\\Users\\clark\\Personal Repos\\aero552\\hw3\\square.dat","aaba");
    if ( t == "REJECT") {
        cout << "REJECT\n";
        //return 1;
    } else{
        cout << "\npass\n";
    }

    cout << "Test no input string (Reject)";

    t = automaton("C:\\Users\\clark\\Personal Repos\\aero552\\hw3\\square.dat","");
    if (t == "REJECT") {
        cout << "REJECT\n";
        //return 1;
    } else{
        cout << "\npass\n";
    }

    cout << "Test invalid input string (Reject)";

    t = automaton("C:\\Users\\clark\\Personal Repos\\aero552\\hw3\\square.dat","a1q34251aba");
    if (t == "REJECT") {
        cout << "REJECT\n";
        //return 1;
    } else{
        cout << "\npass\n";
    }

    cout << "###################### Testing other turing machines ######################\n";

    cout << "Test read only b's, writes c on them, reject if any a is found (pass)";

    t = automaton("C:\\Users\\clark\\Personal Repos\\aero552\\hw3\\test1.dat","bbbb");
    if (t == "REJECT") {
        cout << "REJECT\n";
        //return 1;
    } else{
        cout << "\n" << t;
        cout << "\npass\n";
    }

    cout << "Test read only b's, writes c on them, reject if any a is found (Reject)";

    t = automaton("C:\\Users\\clark\\Personal Repos\\aero552\\hw3\\test1.dat","bbabb");
    if (t == "REJECT") {
        cout << "REJECT\n";
        //return 1;
    } else{
        cout << "\n" << t;
        cout << "\npass\n";
    }
*/

    return 0;
    
}
