#include <Display.h>
#include <Entity.h>
#include <Loader.h>
#include <Maths.h>
#include <Renderer.h>
#include <StaticShader.h>
#include <TexturedModel.h>
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
  std::vector<float> textureCoords = {0.0f, 0.0f, 0.0f, 1.0f,
                                      1.0f, 1.0f, 1.0f, 0.0f};
  nrg::Loader loader;
  auto rawModel = loader.loadVAO(vertices, textureCoords, indices);
  auto modelTexture = loader.loadTexture("res/texture.jpg");
  nrg::TexturedModel model{rawModel, modelTexture};
  nrg::Entity entity{model, glm::vec3(0.0f, 0.0f, -10.0f), 0.0f, 0.0f, 0.0f,
                     1.0f};
  nrg::StaticShader shader;
  nrg::Renderer renderer{display};
  nrg::Camera camera;

  glEnable(GL_DEPTH_TEST);

  while (!display.shouldClose()) {
    camera.move(display.window());
    entity.increaseRotation(0.0f, 1.0f, 0.0f);
    renderer.prepare();
    shader.start();
    shader.loadView(camera);
    renderer.render(entity, shader);
    shader.stop();
    display.update();
  }
  return 0;
}