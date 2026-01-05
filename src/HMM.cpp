#include<bits/stdc++.h>
#include <map>
#include <vector>

using namespace std;
using ld = long double;

// A[i][j]
class HMM {
public: 
  HMM() {
    int n_state = 5;
    initStateP[0] = 1;
    for (int i = 1; i < n_state; i++) {
      initStateP[i] = 0;  
    }
    ld delta = 0.3;
    ld epsilon = 0.4;
    ld tau = 0.1;
    transitionP = {
      {0, 1 - 2*delta - tau, delta, delta, tau},
      { 0, 1 - 2 * delta - tau, delta, delta, tau},
      { 0, 1 - epsilon - tau, epsilon, 0, tau},
      { 0, 1 - epsilon - tau, 0, epsilon, tau},
      { 0, 0, 0, 0, 1},
    };
    emitP = {
    {
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 1
    },
    {
      1.0 / 16, 1.0 / 16, 1.0 / 16, 1.0 / 16, 0,
      1.0 / 16, 1.0 / 16, 1.0 / 16, 1.0 / 16, 0,
      1.0 / 16, 1.0 / 16, 1.0 / 16, 1.0 / 16, 0,
      1.0 / 16, 1.0 / 16, 1.0 / 16, 1.0 / 16, 0,
      0, 0, 0, 0, 0
    },
    {
      0, 0, 0, 0, 0.25,
      0, 0, 0, 0, 0.25,
      0, 0, 0, 0, 0.25,
      0, 0, 0, 0, 0.25,
      0, 0, 0, 0, 0
    },
    {
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0.25, 0.25, 0.25, 0.25, 0
    },
    {
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 1
    }};
  }
  void train(
    int maxIterations,
    vector<vector<pair<char, char>>>& data 
  ){}

  pair<string, string> align() {
    return {"jelena", "hrvoje"};
  }

  void print() {
    //used for debugging
    //prints Markov chain matricies
    //prints alpha matrix
    //prints beta matrix
    //prints gamma matrix
    //prints xi matrix
    cout << "PI:\n[";
    for (int i = 0; i < initStateP.size(); i++) {
      cout << initStateP[i];
      if (i != initStateP.size() - 1)
        cout << " ";
    } 
    cout << "]\n";
    cout << "A:\n[\n";
    for (int i = 0; i < transitionP.size(); i++) {
      cout << "[";
      for (int j = 0; j < initStateP.size(); j++) {
        cout << initStateP[j];
        if (j != initStateP.size() - 1)
          cout << " ";
      } 
      cout << "]\n";
    }
    cout << "\n]";

  }

private:
  vector<ld> initStateP;
  vector<vector<ld>> transitionP;
  vector<vector<ld>> emitP;
  unordered_map<char, int> symMap = {
    {'A', 0},
    {'C', 1},
    {'G', 2},
    {'T', 3},
    {'-', 4}};
  unordered_map<int, char> numMap = {
    {0, 'A'},
    {1, 'C'},
    {2, 'G'},
    {3, 'T'},
    {4, '-'}};

  void readData(
    string path,
    vector<vector<pair<char, char>>>& data) {

  }

    //A C G T - 
    //AA AC AG AT A-
    //CA CC CG CT C-
    //GA GC GG GT G-
    //TA TC TG TT T-
    //-A -C -G -T --
  inline int symsToNum(pair<char, char> syms) {
    return symMap[syms.first] * 5 + symMap[syms.second];
  }

  inline pair<char, char> numToSyms(int num) {
    return {numMap[num / 5], numMap[num % 5]};
  }

  ld calcAlpha(
    vector<vector<ld>>& alpha, 
    int t,
    int i,
    vector<int>& Y) {
    int n_state = initStateP.size();
    ld alp = 0;
    if (alpha[t][i] != -1)
      return alpha[t][i];
    if (t == 0) {
      return alpha[t][i] = initStateP[i] * emitP[i][Y[t]];
    }
    for (int j = 0; j < n_state; j++) {
      alp += calcAlpha(alpha, t - 1, j, Y) * transitionP[j][i];
    }
    alp *= emitP[i][Y[t]];
    return alpha[t][i] = alp;
  }

  ld calcBeta(
    vector<vector<ld>>& beta, 
    int t,
    int i,
    vector<int>& Y) {
    return 0;
  }

  void calcGamma(
    vector<vector<ld>>& alpha,
    vector<vector<ld>>& beta,
    vector<vector<ld>>& gamma) {
    
  }

  void calcXi(
    vector<vector<ld>>& alpha,
    vector<vector<ld>>& beta,
    vector<vector<ld>>& xi) {
  }
};

int main() {
  HMM();
}
