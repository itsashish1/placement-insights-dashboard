#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
struct Row {
  int offers;
  double ctc;
  std::string branch;
};
int main() {
  std::ifstream file("../dataset/sample_placements.csv");
  if (!file.is_open()) {
    std::cerr << "Could not open dataset\n";
    return 1;
  }
  std::string line;
  std::getline(file, line);
  std::vector<Row> rows;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> cols;
    while (std::getline(ss, token, ',')) cols.push_back(token);
    if (cols.size() == 8) {
      rows.push_back({std::stoi(cols[6]), std::stod(cols[7]), cols[1]});
    }
  }
  double offersCount = 0;
  double ctcSum = 0;
  for (const auto& r : rows) {
    if (r.offers > 0) offersCount += 1;
    ctcSum += r.ctc;
  }
  std::cout << "Students: " << rows.size() << "\n";
  std::cout << "Offer Ratio: " << (rows.empty() ? 0.0 : (offersCount / rows.size()) * 100.0) << "%\n";
  std::cout << "Avg Highest CTC: " << (rows.empty() ? 0.0 : ctcSum / rows.size()) << " LPA\n";
  return 0;
}
