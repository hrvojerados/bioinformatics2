#include "../src/HMM.cpp"

int main() {
  HMM* model = new HMM();
  string path = "../src/data/Train_set.txt";
  vector<pair<string, string>> data;
  readData(path, data);

  auto start = std::chrono::high_resolution_clock::now();
  model->train(50, data);
  auto end = std::chrono::high_resolution_clock::now();

  model->writeParameters("../src/params/parameters3.txt");
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Training time: " << elapsed.count() << " seconds\n";

  model->print();
  delete model;
}
