#include<bits/stdc++.h>
#include <map>
#include <vector>

using namespace std;
using ld = long double;

#define eps 1e-10
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
    vector<pair<string, string>>& data 
  ){
    int cnt = 0;
    for (auto [seq1, seq2] : data) {
      this->print();
      if (seq1.size() != seq2.size()) {
        cerr << "given data is surely not aligned properly\n";
        return;
      }
      vector<int> Y;
      Y.push_back(24);
      for (int i = 0; i < seq1.size(); i++) {
        Y.push_back(symsToNum({seq1[i], seq2[i]}));
        // cout << symsToNum({seq1[i], seq2[i]}) << " ";
      }
      Y.push_back(24);
      // cout << "\n";
      for (int iteration = 0; iteration < maxIterations; iteration++) {
        this->print();
        vector<vector<ld>> alpha = 
          vector<vector<ld>>
          (Y.size(),vector<ld>
           (5, -1));
        vector<vector<ld>> beta =
          vector<vector<ld>>
          (Y.size(), vector<ld>
           (5, -1));
        vector<vector<ld>> gamma =
          vector<vector<ld>>
          (Y.size(), vector<ld>
           (5, -1));
        //idk how to format this
        vector<vector<vector<ld>>> xi =
          vector<vector<vector<ld>>>
          (Y.size(), vector<vector<ld>>
          (5, vector<ld>
            (5, -1)));
        this->calcAlpha(alpha, Y);
        this->calcBeta(beta, Y);
        this->calcGamma(alpha, beta, gamma, Y);
        this->calcXi(alpha, beta, xi, Y);

        // printAlphaBetaGammaXi(alpha, beta, gamma, xi, Y);
        for (int i = 0; i < this->initStateP.size(); i++) {
          this->initStateP[i] = gamma[0][i];
        }

        for (int i = 0; i < this->transitionP.size() - 1; i++) {
          ld denominator = 0;
          for (int t = 0; t < Y.size() - 1; t++) {
            denominator += gamma[t][i]; 
          }
          if (denominator == 0)
            continue;
          for (int j = 0; j < this->transitionP.size(); j++) {
            ld numerator = 0;
            for (int t = 0; t < Y.size() - 1; t++) {
              numerator += xi[t][i][j]; 
            }
            this->transitionP[i][j] = numerator / denominator;
          }
        }

        for (int i = 1; i < this->emitP.size() - 1; i++) {
          ld denominator = 0;
          for (int t = 0; t < Y.size() - 1; t++) {
            denominator += gamma[t][i]; 
          }
          if (denominator == 0)
            continue;
          for (int k = 0; k < this->emitP[i].size(); k++) {
            ld numerator = 0;
            for (int t = 0; t < Y.size(); t++) {
              if (Y[t] == k)
                numerator += gamma[t][i]; 
            }
            this->emitP[i][k] = (numerator + eps) / (denominator + 25 * eps);
          }
        }
      }
    }
  }

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
    vector<string> stateNames = {
      "Begin",
      "Match",
      "InsertX",
      "InsertY",
      "End"};
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

  void printAlphaBetaGammaXi(
    vector<vector<ld>>& alpha,
    vector<vector<ld>>& beta,
    vector<vector<ld>>& gamma,
    vector<vector<vector<ld>>>& xi,
    vector<int>& Y) {
    //prints alpha matrix
    //prints beta matrix
    //prints gamma matrix
    //prints xi matrix
    cout << "alpha:\n";
    for (int t = 0; t < Y.size(); t++) {
      for (int i = 0; i < initStateP.size(); i++) {
        cout << alpha[t][i] << " "; 
      }
      cout << "\n";
    }
    cout << "beta:\n";
    for (int t = 0; t < Y.size(); t++) {
      for (int i = 0; i < initStateP.size(); i++) {
        cout << beta[t][i] << " "; 
      }
      cout << "\n";
    }
    cout << "gamma:\n";
    for (int t = 0; t < Y.size(); t++) {
      for (int i = 0; i < initStateP.size(); i++) {
        cout << gamma[t][i] << " "; 
      }
      cout << "\n";
    }
    cout << "xi:\n";
    for (int t = 0; t < Y.size(); t++) {
      cout << "t = " << t << "\n";
      for (int i = 0; i < initStateP.size(); i++) {
        for (int j = 0; j < initStateP.size(); j++) {
          cout << xi[t][i][j] << " "; 
        }
        cout << "\n";
      }
    }
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
  void test1() {
    vector<int> Y = {24, 4, 5, 24};
    vector<vector<ld>> alpha = vector<vector<ld>>(4, vector<ld>(5, -1));
    vector<vector<ld>> beta = vector<vector<ld>>(4, vector<ld>(5, -1));
    vector<vector<ld>> gamma = vector<vector<ld>>(4, vector<ld>(5, -1));
    //idk how to format this
    vector<vector<vector<ld>>> xi =
      vector<vector<vector<ld>>>
      (4, vector<vector<ld>>
       (5, vector<ld>
        (5, -1)));
  
    this->calcAlpha(alpha, Y);
    this->calcBeta(beta, Y);
    this->calcGamma(alpha, beta, gamma, Y);
    this->calcXi(alpha, beta, xi, Y);
    this->print();
    this->printAlphaBetaGammaXi(alpha,
                                beta,
                                gamma,
                                xi,
                                Y);
  }
  void test2() {
    string path = "data/Train_set.txt";
    vector<pair<string, string>> data;
    readData(path, data);
    this->train(10, data);
    this->print();
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

  ld calcAlphaR(
    vector<vector<ld>>& alpha, 
    int t,
    int i,
    vector<int>& Y) {
    int n_state = initStateP.size();
    if (alpha[t][i] != -1)
      return alpha[t][i];
    if (t == 0) {
      return alpha[t][i] = initStateP[i] * emitP[i][Y[t]];
    }
    ld scale = 0;
    for (int j = 0; j < n_state; j++) { 
      scale += alpha[t - 1][j];
    }
    ld alp = 0;
    for (int j = 0; j < n_state; j++) {
      alp += calcAlphaR(alpha, t - 1, j, Y) * transitionP[j][i];
    }
    alp *= emitP[i][Y[t]];
    return alpha[t][i] = alp / scale;
  }

  void calcAlpha(
    vector<vector<ld>>& alpha, 
    vector<int>& Y) {
    int T = alpha.size() - 1;
    for (int i = 0; i < alpha[0].size(); i++) {
      calcAlphaR(alpha, T, i, Y);
    }
  }

  ld calcBetaR(
    vector<vector<ld>>& beta, 
    int t,
    int i,
    vector<int>& Y) {
    int n_state = initStateP.size();
    if (beta[t][i] != -1)
      return beta[t][i];
    int T = Y.size() - 1;
    if (t == T)
      return beta[t][i] = 1;
    ld bet = 0;
    for (int j = 0; j < n_state; j++) {
      bet += calcBetaR(beta, t + 1, j, Y) * transitionP[i][j] * emitP[j][Y[t + 1]];
    }
    ld scale = 0;
    for (int j = 0; j < n_state; j++) { 
      scale += beta[t + 1][j];
    }
    return beta[t][i] = bet / scale;
  }

  void calcBeta(
    vector<vector<ld>>& beta, 
    vector<int>& Y) {
    int T = beta.size() - 1;
    for (int i = 0; i < beta[0].size(); i++) {
      calcBetaR(beta, 0, i, Y);
    }
  }

  void calcGamma(
    vector<vector<ld>>& alpha,
    vector<vector<ld>>& beta,
    vector<vector<ld>>& gamma,
    vector<int>& Y) {
    int n_state = initStateP.size();
    for (int t = 0; t < Y.size(); t++) {
      for (int i = 0; i < n_state; i++) {
        ld den = 0;
        for (int j = 0; j < n_state; j++) {
          den += alpha[t][j] * beta[t][j];
        }
        gamma[t][i] = (alpha[t][i] * beta[t][i]) / den;
      }
    }
  }

  void calcXi(
    vector<vector<ld>>& alpha,
    vector<vector<ld>>& beta,
    vector<vector<vector<ld>>>& xi,
    vector<int>& Y) {
    int n_state = initStateP.size();
    for (int t = 0; t < Y.size() - 1; t++) {
      for (int i = 0; i < n_state; i++) {
        for (int j = 0; j < n_state; j++) {
          ld den = 0;
          for (int k = 0; k < n_state; k++) {
            for (int l = 0; l < n_state; l++) {
              den += alpha[t][k] * transitionP[k][l] * beta[t + 1][l] * emitP[l][Y[t + 1]];
            }
          }
          // ld den = 0;
          // for (int k = 0; k < n_state; k++) {
          //   den += alpha[t][k] * beta[t][k];
          // }
          xi[t][i][j] = (alpha[t][i] * transitionP[i][j] * beta[t + 1][j] * emitP[j][Y[t + 1]]) / den;
        }
      }
    }
  }
};

int main() {
  HMM* model = new HMM();
  model->test2();
}
