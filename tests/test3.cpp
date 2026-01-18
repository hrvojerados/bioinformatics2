#include "../src/HMM.cpp"

int main() {
  ifstream file("../src/data/Test_set.txt");
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
