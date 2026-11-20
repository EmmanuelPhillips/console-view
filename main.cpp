#include <cctype>
#include <pwd.h>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

std::optional<std::vector<std::filesystem::path>>
get_pids(const std::filesystem::path &path) {
  // edge cases
  if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
    return std::nullopt;
  }

  std::vector<std::filesystem::path> status_paths{};

  // iterate over all files in is_directory
  // append to vector of paths if its a valid pid
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    bool is_pid{true};
    for (const auto &ch : entry.path().filename().string()) {
      if (!std::isdigit(static_cast<unsigned char>(ch))) {
        is_pid = false;
        break;
      }
    }
    if (is_pid) {
      auto status_path{entry.path() / "status"};
      status_paths.emplace_back(status_path);
    }
  }
  return status_paths;
}

void print_pids(const std::vector<std::filesystem::path> &pids) {
  for (const auto &pid : pids) {
    std::ifstream file(pid);
    if (!file.is_open()) {
      continue;
    }
    std::string search_value{"Name:"};
    std::string line;
    std::string pid_name;
    while (std::getline(file, line)) {
      if (std::string_view{line}.starts_with(search_value)) {
        pid_name = line.substr(search_value.size());
      }
    }
    if (!pid_name.empty()) {
      std::cout << pid_name << '\n';
    }
  }
}

int main() {
  const std::filesystem::path path{"/proc/"};
  auto pids{get_pids(path)};
  if (pids) {
    print_pids(*pids);
  }

  return 0;
}
