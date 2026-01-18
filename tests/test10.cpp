#include "../src/HMM.cpp"

int main() {
  ifstream file("../src/data/Test_set.txt");
  string line;
  string x;
  string y;
  // ld time = 0;
  // ld val = 0;
  // int cnt = 0;
  // ld maxVal;
  while (getline(file, line)) {
    if (line[0] == '>') {
      x = "";
      y = "";
      continue;
    } else if (x == ""){
      x = line;
      // maxVal = x.size(); 
    } else {
      y = line;
      auto start = std::chrono::high_resolution_clock::now();
      NW(x, y);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = end - start;
      cout << elapsed.count() << "\n";
      break;
    }
  }
}
