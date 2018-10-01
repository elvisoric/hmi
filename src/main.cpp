#include <Display.h>
#include <Loader.h>
#include <Renderer.h>
#include <StaticShader.h>
#include <iostream>
#include <vector>

int main() {
  nrg::Display display = nrg::createDisplay(1024.0f, 768.0f);
  std::vector<float> vertices = {// positions
                                 0.5f,  0.5f,  0.0f, 0.5f,  -0.5f, 0.0f,
                                 -0.5f, -0.5f, 0.0f, -0.5f, 0.5f,  0.0f};
  std::vector<unsigned int> indices = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };
  nrg::Loader loader;
  auto model = loader.loadVAO(vertices, indices);
  nrg::StaticShader shader;
  nrg::Renderer renderer;

  glEnable(GL_DEPTH_TEST);

  while (!display.shouldClose()) {
    renderer.prepare();
    shader.start();
    renderer.render(model);
    shader.stop();
    display.update();
  }
  return 0;
}