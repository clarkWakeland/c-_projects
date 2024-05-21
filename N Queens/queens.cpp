#include <iostream>
#include <vector>

using namespace std;
// Checks if the position is safe for a queen.
bool isSafe(int row, int col, vector<int>& position) {
    for (int i = 0; i < col; i++) {
        if (position[i] == row || (abs(position[i] - row) == abs(i - col))) {
            return false;
        }
    }
    return true;
}

// Prints board configuration
// . (period) represent empty space
// Q represents space occupied by Queen
void printSolution(vector<int>& position) {
    int n = position.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (position[i] == j) {
                cout << "Q ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Recursive utility function to solve the problem.
bool NQUtil(int col, int depth, int n, int depthLimit, vector<int>& position, bool& cont, bool& foundSol) {
    
    if (col == n) {
        printSolution(position);
        foundSol = true; // Useful for output message
        return true;
    }

    if (depth == depthLimit || !cont) {
        return false;
    }

    for (int i = 0; i < n; i++) {
        if (isSafe(i, col, position)) {
            
            position[col] = i;
            if (NQUtil(col + 1, depth + 1, n, depthLimit,  position, cont, foundSol)) {
                char choice;

                cout << "Find Another Solution? (Y/N): ";
                cin >> choice;
                if (choice != 'Y' && choice != 'y') {
                    cont = false;
                    break;
                }
            }
            position[col] = -1; // backtrack
        }
    }

    return false;
}

// Solves N Queens using iterative deepening
// Loops through depth limit and searches states up to depth limit
bool solveNQueens(int n) {
    vector<int> position(n, -1);
    bool cont = true;
    bool foundSol = false;
    for (int depthLimit = 1; depthLimit <= n; depthLimit++) {
        
        if (NQUtil(0, 0, n, depthLimit, position, cont, foundSol)) {
            cout << "Solution at level ";
        }
    
    }
    if (foundSol && !cont){
        cout << "No more solutions will be displayed\n";
        return true;
    } else if(foundSol && cont){
        cout << "There are no more solutions\n";
        return true;
    }
    return false;
}

int main() {
    int n = 2; // Should have no solutions
    cout << "Finding configs for " << n << " queens \n";
    if (!solveNQueens(n)) {
        cout << "Solution does not exist for " << n << " queens\n";
    }
    n = 3; // Should have no solutions
    cout << "Finding configs for " << n << " queens \n";
    if (!solveNQueens(n)) {
        cout << "Solution does not exist for " << n << " queens\n";
    }
    n = 4; // Should have 2 solutions
    cout << "Finding configs for " << n << " queens \n";
    if (!solveNQueens(n)) {
        cout << "Solution does not exist for " << n << " queens\n";
    }
    n = 5; // Should have 10 solutions
    cout << "Finding configs for " << n << " queens \n";
    if (!solveNQueens(n)) {
        cout << "Solution does not exist for " << n << " queens\n";
    }
    n = 6; // Should have 4 solutions
    cout << "Finding configs for " << n << " queens \n";
    if (!solveNQueens(n)) {
        cout << "Solution does not exist for " << n << " queens\n";
    }
    n = 8; // Should have 92 solutions
    cout << "Finding configs for " << n << " queens \n";
    if (!solveNQueens(n)) {
        cout << "Solution does not exist for " << n << " queens\n";
    }

    return 0;
}
