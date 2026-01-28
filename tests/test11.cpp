#include "../src/HMM.cpp"
#include <iostream>

int main() {
  HMM* model = new HMM();
  string path = "../src/params/params2.txt";
  model->readParameters(path);
  string x = "ATGCTACGTTGACCTGAACTGCTAGCTGAA";
  string y = "ATGCTACGTTGAACTGCTGAA";
  auto [alignedX, alignedY] = model->align(x, y);
  NW(x, y);
  cout << "\n\n\n";
  cout << alignedX << "\n";
  cout << alignedY << "\n";
}
