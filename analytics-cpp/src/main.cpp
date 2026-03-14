#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
struct Row {
  std::string branch;
  int offers;
  double ctc;
};
static std::vector<Row> readCsv(const std::string& path) {
  std::ifstream file(path);
  std::vector<Row> rows;
  if (!file.is_open()) return rows;
  std::string line;
  std::getline(file, line);
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> cols;
    while (std::getline(ss, token, ',')) cols.push_back(token);
    if (cols.size() == 8) {
      Row r{cols[1], std::stoi(cols[6]), std::stod(cols[7])};
      rows.push_back(r);
    }
  }
  return rows;
}
static std::string response(const std::string& status, const std::string& body) {
  std::ostringstream os;
  os << "HTTP/1.1 " << status << "\r\n"
     << "Content-Type: application/json\r\n"
     << "Access-Control-Allow-Origin: *\r\n"
     << "Access-Control-Allow-Methods: GET,OPTIONS\r\n"
     << "Access-Control-Allow-Headers: Content-Type\r\n"
     << "Content-Length: " << body.size() << "\r\n\r\n"
     << body;
  return os.str();
}
int main() {
#ifdef _WIN32
  WSADATA wsa;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;
#endif
  int server_fd = static_cast<int>(socket(AF_INET, SOCK_STREAM, 0));
  if (server_fd < 0) return 1;
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(8082);
  int opt = 1;
#ifdef _WIN32
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));
#else
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif
  if (bind(server_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) return 1;
  if (listen(server_fd, 10) < 0) return 1;
  std::cout << "Placement API running on http://localhost:8082\n";
  while (true) {
    sockaddr_in client_addr{};
#ifdef _WIN32
    int client_len = sizeof(client_addr);
#else
    socklen_t client_len = sizeof(client_addr);
#endif
    int client = static_cast<int>(accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len));
    if (client < 0) continue;
    char buffer[8192] = {0};
#ifdef _WIN32
    int bytes = recv(client, buffer, sizeof(buffer) - 1, 0);
#else
    int bytes = static_cast<int>(read(client, buffer, sizeof(buffer) - 1));
#endif
    if (bytes <= 0) {
#ifdef _WIN32
      closesocket(client);
#else
      close(client);
#endif
      continue;
    }
    std::string req(buffer, bytes);
    std::string res;
    if (req.rfind("OPTIONS ", 0) == 0) {
      res = response("200 OK", "{}");
    } else if (req.rfind("GET /health", 0) == 0) {
      res = response("200 OK", "{\"ok\":true,\"service\":\"placement-analytics-cpp\"}");
    } else if (req.rfind("GET /kpis", 0) == 0) {
      auto rows = readCsv("../dataset/sample_placements.csv");
      int students = static_cast<int>(rows.size());
      int placed = 0;
      double ctcSum = 0.0;
      std::unordered_map<std::string, std::pair<int, int>> branch; // total, placed
      for (const auto& r : rows) {
        if (r.offers > 0) placed++;
        ctcSum += r.ctc;
        branch[r.branch].first += 1;
        if (r.offers > 0) branch[r.branch].second += 1;
      }
      double offerRatio = students ? (100.0 * placed / students) : 0.0;
      double avgCtc = students ? (ctcSum / students) : 0.0;
      std::ostringstream b;
      b << "[";
      bool first = true;
      for (const auto& kv : branch) {
        if (!first) b << ",";
        first = false;
        double ratio = kv.second.first ? (100.0 * kv.second.second / kv.second.first) : 0.0;
        b << "{\"branch\":\"" << kv.first << "\",\"offerRatio\":" << ratio << "}";
      }
      b << "]";
      std::ostringstream out;
      out << "{\"students\":" << students
          << ",\"offerRatio\":" << offerRatio
          << ",\"avgHighestCtc\":" << avgCtc
          << ",\"branchStats\":" << b.str() << "}";
      res = response("200 OK", out.str());
    } else {
      res = response("404 Not Found", "{\"message\":\"not found\"}");
    }
#ifdef _WIN32
    send(client, res.c_str(), static_cast<int>(res.size()), 0);
    closesocket(client);
#else
    write(client, res.c_str(), res.size());
    close(client);
#endif
  }
#ifdef _WIN32
  WSACleanup();
#endif
  return 0;
}
