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
    initStateP = vector<ld>(n_state);
    transitionP = vector<vector<ld>>(n_state, vector<ld>(n_state));
    emitP = vector<vector<ld>>(n_state, vector<ld>(25));
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
    pair<string, string>& data 
  ){}

  pair<string, string> align() {
    return {"jelena", "hrvoje"};
  }

  void print() {
    //used for debugging
    //prints Markov chain matricies
    cout << "PI:\n[";
    for (int i = 0; i < initStateP.size(); i++) {
      cout << initStateP[i];
      if (i != initStateP.size() - 1)
        cout << " ";
    } 
    cout << "]\n\n";
    cout << "A:\n";
    for (int i = 0; i < transitionP.size(); i++) {
      for (int j = 0; j < transitionP[i].size(); j++) {
        cout << transitionP[i][j];
        if (j != transitionP.size() - 1)
          cout << " ";
      } 
      cout << "\n";
    }
    cout << "\n";
    cout << "E:\n";
    vector<string> stateNames = {"Begin", "Match", "InsertX", "InsertY", "End"};
    for (int i = 0; i < emitP.size(); i++) {
      cout << "state: " << stateNames[i] << "\n";
      for (int j = 0; j < 5; j++) {
        for (int k = 0; k < 5; k++) {
          cout << emitP[i][j * 5 + k];
          if (k != 4)
            cout << " ";
        } 
        cout << "\n";
      }
      cout << "\n";
    }
  }

  void printAlphaBetaGammaXi() {
    //prints alpha matrix
    //prints beta matrix
    //prints gamma matrix
    //prints xi matrix
  }

  void readData(
    string path,
    vector<pair<string, string>>& data) {
    ifstream file(path);
    if (!file.is_open()) {
      cerr << "Couldn't open the file\n";
    }
    string line;
    string seq1 = "";
    string seq2 = "";
    while(getline(file, line)) {
      if (line[0] == '>') {
        seq1 = "";
        seq2 = "";
      }
      else if (seq1 == "") {
        seq1 = line;
      } 
      else if (seq2 == ""){
        seq2 = line;
        data.emplace_back(seq1, seq2);
      }
    }
    return;
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
  string path = "data/Test_set.txt";
  HMM* model = new HMM();
  model->print();
  vector<pair<string, string>> data;
  model->readData(path, data);
  for (int i = 0; i < data.size(); i++) {
    cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>\n";
    cout << data[i].first << "\n";
    cout << data[i].second << "\n";
    cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>\n";
  }
}
