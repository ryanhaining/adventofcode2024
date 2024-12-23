#include <iostream>

namespace {

constexpr unsigned long PRUNE_VAL = 16777216;
constexpr int STEPS = 2000;

unsigned long step(unsigned long n) {
  n = (n ^ (n * 64)) % PRUNE_VAL;
  n = (n ^ (n / 32)) % PRUNE_VAL;
  n = (n ^ (n * 2048)) % PRUNE_VAL;
  return n;
}

}  // namespace

int main() {
  unsigned long secret{};
  unsigned long total{};
  while (std::cin >> secret) {
    for (int i = 0; i < STEPS; ++i) {
      secret = step(secret);
    }
    total += secret;
  }
  std::cout << total << '\n';
}
