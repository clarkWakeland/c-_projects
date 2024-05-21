#ifndef TUR
#define TUR

#include <iostream>
using namespace std;

class turing{
    private:
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

};
