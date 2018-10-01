#include <Display.h>
#include <iostream>

int main() {
  nrg::Display display = nrg::createDisplay(1024.0f, 768.0f);

  while (!display.shouldClose()) {
    display.update();
  }
  return 0;
}