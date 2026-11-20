#include <cctype>
#include <pwd.h>

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>

void print(const std::filesystem::path &path) {
  if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
    return;
  }
  for (const auto &f : std::filesystem::directory_iterator(path)) {
    std::string item_name{f.path().filename().string()};
    bool is_pid{true};
    for (const auto c : item_name) {
      if (!std::isdigit(static_cast<unsigned char>(c))) {
        is_pid = false;
      }
    }
    if (is_pid) {
      std::cout << item_name << '\n';
    }
  }
}

int main() {
  const std::filesystem::path path{"/proc/"};
  print(path);

  return 0;
}
