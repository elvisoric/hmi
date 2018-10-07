#include <Display.h>
#include <Entity.h>
#include <Loader.h>
#include <Maths.h>
#include <ObjLoader.h>
#include <Renderer.h>
#include <StaticShader.h>
#include <TexturedModel.h>
#include <stb_image.h>
#include <iostream>
#include <vector>

int main() {
  stbi_set_flip_vertically_on_load(true);

  nrg::Display display = nrg::createDisplay(1024.0f, 768.0f);
  nrg::Loader loader;
  auto obj = nrg::load("res/monkey.obj");
  auto rawModel = loader.loadVAO(obj.vertices, obj.textures, obj.indices);
  auto modelTexture = loader.loadTexture("res/white.png");
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