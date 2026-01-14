#include<bits/stdc++.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
using ld = long double;
using ll = long long;

#define eps 1e-10
#define EPS 20
#define numOfIterations 50

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
    for (int ind = 0; ind < data.size(); ind++) {
      // this->print();
      if (data[ind].first.size() != data[ind].second.size()) {
        cerr << "given data is surely not aligned properly\n";
        return;
      }
      vector<int> Y;
      Y.push_back(24);
      for (int j = 0; j < data[ind].first.size(); j++) {
        Y.push_back(symsToNum(data[ind].first[j], data[ind].second[j]));
      }
      Y.push_back(24);
      for (int iteration = 0; iteration < maxIterations; iteration++) {
        // this->print();
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
        vector<vector<vector<ld>>> xi =
          vector<vector<vector<ld>>>
          (Y.size(), vector<vector<ld>>
          (5, vector<ld>
            (5, -1)));
        this->calcAlpha(alpha, Y);
        this->calcBeta(beta, Y);
        this->calcGamma(alpha, beta, gamma, Y);
        this->calcXi(alpha, beta, xi, Y);

        for (int i = 0; i < this->initStateP.size(); i++) {
          this->initStateP[i] = gamma[0][i];
        }

        for (int i = 0; i < this->transitionP.size() - 1; i++) {
          ld denominator = 0;
          for (int t = 0; t < Y.size() - 1; t++) {
            denominator += gamma[t][i]; 
          }
          for (int j = 1; j < this->transitionP.size(); j++) {
            ld numerator = 0;
            for (int t = 0; t < Y.size() - 1; t++) {
              numerator += xi[t][i][j]; 
            }
            if (i == 0 || i == 1) {
              this->transitionP[i][j] = (numerator + eps) / (denominator + 4 * eps);
            } else if (i == 2) {
              if (j!=3) {
                this->transitionP[i][j] = (numerator + eps) / (denominator + 3 * eps);
              }
            } else if (i == 3){
              if (j != 2) {
                this->transitionP[i][j] = (numerator + eps) / (denominator + 3 * eps);
              }
            }
            else {
              cerr << "huh?\n";
            }
          }
        }

        for (int i = 1; i < this->emitP.size() - 1; i++) {
          ld denominator = 0;
          for (int t = 0; t < Y.size() - 1; t++) {
            denominator += gamma[t][i]; 
          }
          for (int k = 0; k < this->emitP[i].size(); k++) {
            ld numerator = 0;
            for (int t = 0; t < Y.size(); t++) {
              if (Y[t] == k)
                numerator += gamma[t][i]; 
            }
            if (i == 1) {
              if (k % 5 != 4 && k < 20) 
                this->emitP[i][k] = (numerator + eps) / (denominator + 16 * eps);
            } else if (i == 2) {
              if (k % 5 == 4 && k < 20) 
                this->emitP[i][k] = (numerator + EPS) / (denominator + 4 * EPS);
            } else if (i == 3) {
              if (k % 5 != 4 && k >= 20) 
                this->emitP[i][k] = (numerator + EPS) / (denominator + 4 * EPS);
            } else {
              cerr << "huh?\n";
            }
          }
        }
      }
    }
  }

  void readParameters(string path) {
    ifstream file(path);
    if (!file.is_open()) {
      cerr << "Couldn't open the file\n";
    }
    int phase = 0;
    string line;
    int i = 0;
    ld x;
    while(getline(file, line)) {
      if (line[0] == '>')
        continue;
      stringstream ss(line);
      if (phase == 0) {
        for (int j = 0; j < 5; j++) {
          ss >> x;
          this->transitionP[i][j] = x;
        } 
        i++;
        if (i == 5){
          i = 0;
          phase++;
        } 
      } else if (phase == 1){
        for (int j = 0; j < 25; j++) {
          ss >> x;
          this->emitP[i][j] = x;
        } 
        i++;
        if (i == 5) {
          phase++;
        } 
      } 
    }
    return;
  }

  void writeParameters(string fileName) {
    ofstream file("params/" + fileName + ".txt");
    for (int i = 0; i < transitionP.size(); i++) {
      for (int j = 0; j < transitionP[i].size(); j++) {
        file << transitionP[i][j];
        if (j != transitionP.size() - 1)
          file << " ";
      } 
      file << "\n";
    }
    file << ">\n";
    for (int i = 0; i < emitP.size(); i++) {
      for (int j = 0; j < 25; j++) {
        file << emitP[i][j] << " ";
      } 
      file << "\n>\n";
    }
  }

  pair<string, string> align(string& X, string& Y) {
    vector<vector<ld>> Vm =
      vector<vector<ld>>
        (X.length(), vector<ld>
          (Y.length()));
    for (int j = 0; j < Y.length(); j++) {
      Vm[0][j] = log(0);
    }
    for (int i = 0; i < X.length(); i++) {
      Vm[i][0] = log(0);
    }
    vector<vector<ld>> Vix =
      vector<vector<ld>>
        (X.length(), vector<ld>
          (Y.length()));
    for (int j = 0; j < Y.length(); j++) {
      Vix[0][j] = log(0);
    }
    for (int i = 0; i < X.length(); i++) {
      Vix[i][0] = log(0);
    }
    vector<vector<ld>> Viy =
      vector<vector<ld>>
        (X.length(),vector<ld>
          (Y.length()));
    for (int j = 0; j < Y.length(); j++) {
      Viy[0][j] = log(0);
    }
    for (int i = 0; i < X.length(); i++) {
      Viy[i][0] = log(0);
    }
    vector<vector<int>> MPrev =
      vector<vector<int>>
        (X.length(), vector<int>
          (Y.length(), 0));
    vector<vector<int>> IxPrev =
      vector<vector<int>>
        (X.length(), vector<int>
          (Y.length(), 0));
    vector<vector<int>> IyPrev =
      vector<vector<int>>
        (X.length(), vector<int>
          (Y.length(), 0));
    
    Vm[0][0] = 0;
    int emission;
    for (int i = 1; i < X.length(); i++) {
      for (int j = 1; j < Y.length(); j++) {
        emission = symsToNum(X[i],Y[j]);
        ld tmp1 = Vm[i-1][j-1] + log(transitionP[1][1]);
        ld tmp2 = Vix[i-1][j-1] + log(transitionP[2][1]);
        ld tmp3 = Viy[i-1][j-1] + log(transitionP[3][1]);
        Vm[i][j] = 
          log(emitP[1][emission]) +
            max({tmp1, tmp2, tmp3});
        if (tmp1 >= tmp2 && tmp1 >= tmp3) {
          MPrev[i][j] = 1;
        } else if (tmp2 >= tmp3) {
          MPrev[i][j] = 2;
        } else {
          MPrev[i][j] = 3;
        }

        emission = symsToNum(X[i],'-');
        tmp1 = Vm[i-1][j] + log(transitionP[1][2]);
        tmp2 = Vix[i-1][j] + log(transitionP[2][2]); 
        Vix[i][j] = log(emitP[2][emission]) + max(tmp1, tmp2);
        if (tmp1 >= tmp2){
          IxPrev[i][j] = 1;
        } else {
          IxPrev[i][j] = 2;
        }
        emission = symsToNum('-',Y[j]);
        tmp1 = Vm[i][j-1] + log(transitionP[1][3]);
        tmp2 = Viy[i][j-1] + log(transitionP[3][3]);
        Viy[i][j] = log(emitP[3][emission]) + max(tmp1, tmp2);
        if (tmp1 >= tmp2) {  
          IyPrev[i][j] = 1;
        } else {
          IyPrev[i][j] = 3;
        }
      }
    }

    string seq1 = "-";
    string seq2 = "-";
    int x = X.size() - 1;
    int y = Y.size() - 1;
    int curState;
    int nxtState;
    while (x != 0 && y != 0) {
      if (
        Vm[x][y] > Vix[x][y] && 
        Vm[x][y] > Viy[x][y]) {
        curState = 1; 
        nxtState = Vm[x][y];
        seq1 = X[x] + seq1;
        seq2 = Y[y] + seq2;
        x--;
        y--;
      } else if (
        Vix[x][y] > Viy[x][y]) {
        curState = 2; 
        nxtState = Vix[x][y];
        seq1 = X[x] + seq1;
        seq2 = '-' + seq2;
        x--;
      } else {
        curState = 3; 
        nxtState = Viy[x][y];
        seq1 = '-' + seq1;
        seq2 = Y[y] + seq2;
        y--;
      } 
    }
    seq1 = '-' + seq1;
    seq2 = '-' + seq2;
    return {seq1, seq2};
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
  inline int symsToNum(char sym1, char sym2) {
    return symMap[sym1] * 5 + symMap[sym2];
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
          xi[t][i][j] = (alpha[t][i] * transitionP[i][j] * beta[t + 1][j] * emitP[j][Y[t + 1]]) / den;
        }
      }
    }
  }
};

void print2DVector(const std::vector<std::vector<ll>>& mat)
{
    for (const auto& row : mat)
    {
        for (ll val : row)
        {
            std::cout << std::setw(4) << val << " ";
        }
        std::cout << '\n';
    }
}

ll NW(string x, string y) {
  vector<vector<ll>> dp =
    vector<vector<ll>>
      (x.size() + 1, vector<ll>
        (y.size() + 1)); 
  for (int j = 0; j < y.size() + 1; j++) {
    dp[0][j] = j * (-2);
  }
  for (int i = 0; i < x.size() + 1; i++) {
    dp[i][0] = i * (-2);
  }
  for (int i = 1; i < x.size() + 1; i++) {
    for (int j = 1; j < y.size() + 1; j++) {
      dp[i][j] = 
        max({
          dp[i - 1][j] - 2,
          dp[i][j - 1] - 2,
          dp[i - 1][j - 1] + 1 * (x[i - 1] == y[j - 1]) + (-1) * (x[i - 1] != y[j - 1]) 
        });
    }
  }
  return dp[x.size()][y.size()];
}

ll evaluateAlignment(string x, string y) {
  ll val = 0;
  for (int i = 0; i < x.size(); i++) {
    if (x[i] == y[i])
      val++;
    else if (x[i] == '-' || y[i] == '-') {
      val -= 2;
    } else {
      val--;
    }
  }
  return val;
}

void test1() {
  HMM* model = new HMM();
  string path = "data/Train_set.txt";
  vector<pair<string, string>> data;
  model->readData(path, data);
  cout << data.size() << "\n";
  model->train(numOfIterations, data);
  model->print();
}

void test2() {
  HMM* model = new HMM();
  string path = "params/newParameters.txt";
  vector<pair<string, string>> data;
  model->print();
  model->writeParameters("newParameters");
}

void test3() {
  ifstream file("data/Test_set.txt");
  string line;
  string x;
  string y;
  while (getline(file, line)) {
    if (line[0] == '>') {
      x = "";
      y = "";
      continue;
    } else if (x == ""){
      x = line;
    } else {
      y = line;
      cout << NW(x, y) << "\n";
    }
  }
}

void test4() {
  HMM* model = new HMM();
  string path = "params/params.txt";
  model->readParameters(path);
  ifstream file("data/Test_set.txt");
  string line;
  string x;
  string y;
  while (getline(file, line)) {
    if (line[0] == '>') {
      x = "";
      y = "";
      continue;
    } else if (x == ""){
      x = line;
    } else {
      y = line;
      auto [alignedX, alignedY] =
        model->align(x, y);
      cout << evaluateAlignment(alignedX, alignedY) << "\n";
    }
  }
}

int main() {
  test4();
}
