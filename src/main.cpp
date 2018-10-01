#include <DisplayManager.h>
#include <iostream>

int main() {
  std::cout << "Hello from main" << std::endl;
  std::cout << nrg::DisplayManager::screenWidth;
  std::cout << nrg::DisplayManager::screenHeight;
  return 0;
}