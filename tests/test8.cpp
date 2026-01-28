#include "../src/HMM.cpp"

int main() {
  HMM* model = new HMM();
  string path = "../src/params/params2.txt";
  model->readParameters(path);
  ifstream file("../src/data/Test_set.txt");
  string line;
  string x;
  string y;
  ld time = 0;
  ld val = 0;
  int cnt = 0;
  ld maxVal;
  while (getline(file, line)) {
    if (line[0] == '>') {
      x = "";
      y = "";
      continue;
    } else if (x == ""){
      x = line;
      maxVal = x.size(); 
    } else {
      y = line;
      auto start = std::chrono::high_resolution_clock::now();
      auto [alignedX, alignedY] =
        model->align(x, y);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = end - start;
      // ld delta = abs(evaluateAlignmentWeDontLikeInsert(alignedX, alignedY) - NWWeDontLikeInsert(x, y));
      cout << evaluateAlignmentWeDontLikeInsert(alignedX, alignedY) << " ";
      cout << NWWeDontLikeInsert(x, y) << "\n";
      // val += delta;
      // cout << delta << "\n";
      time += elapsed.count();
      // cnt++;
    }
  }
  // cout << val / (cnt * maxVal) << "  " << time / cnt << "\n";
}
