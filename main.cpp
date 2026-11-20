#include <cctype>
#include <pwd.h>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

void print_pids(const std::filesystem::path &path) {
  // edge cases
  if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
    return;
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

  for (const auto &path : status_paths) {
    std::ifstream file(path);
    if (!file.is_open()) {
      continue;
    }
  }
}

int main() {
  const std::filesystem::path path{"/proc/"};
  print_pids(path);

  return 0;
}
