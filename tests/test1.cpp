#include "../src/HMM.cpp"

int main() {
  HMM* model = new HMM();
  string path = "data/Train_set.txt";
  vector<pair<string, string>> data;
  readData(path, data);
  model->readParameters("../src/params/params2.txt");
  model->print();
}
