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

nrg::TexturedModel loadTexturedModel(const std::string& objpath,
                                     const std::string& texturpath,
                                     nrg::Loader& loader) {
  auto obj = nrg::load(objpath);
  auto rawModel =
      loader.loadVAO(obj.vertices, obj.textures, obj.normals, obj.indices);
  auto modelTexture = loader.loadTexture(texturpath);
  nrg::TexturedModel model{rawModel, modelTexture};
  return model;
}

int main() {
  // stbi_set_flip_vertically_on_load(true);

  nrg::Display display = nrg::createDisplay(1024.0f, 768.0f);
  nrg::Loader loader;

  auto sphereModel =
      loadTexturedModel("res/sphere.obj", "res/sphere.png", loader);
  sphereModel.texture().reflectivity(1);
  sphereModel.texture().shineDamper(32);
  nrg::Entity sphere{
      sphereModel, glm::vec3(0.0f, 0.0f, -5.0f), 0.0f, 0.0f, 0.0f, 1.0f};

  auto cubeModel = loadTexturedModel("res/cube.obj", "res/grid.png", loader);
  cubeModel.texture().reflectivity(1);
  cubeModel.texture().shineDamper(256);
  nrg::Entity cube{cubeModel, glm::vec3(3.0f, 2.0f, -8.0f), 0.0f, 0.0f, 0.0f,
                   1.0f};

  auto monkeyModel =
      loadTexturedModel("res/monkey.obj", "res/monkey.png", loader);
  monkeyModel.texture().reflectivity(1);
  monkeyModel.texture().shineDamper(32);
  nrg::Entity monkey{
      monkeyModel, glm::vec3(-1.0f, -2.0f, -3.0f), 0.0f, 0.0f, 0.0f, 1.0f};

  auto barrelModel =
      loadTexturedModel("res/barrel.obj", "res/barrel.png", loader);
  barrelModel.texture().reflectivity(1);
  barrelModel.texture().shineDamper(32);
  nrg::Entity barrel{
      barrelModel, glm::vec3(2.0f, -3.0f, -4.0f), 0.0f, 0.0f, 0.0f, 0.2f};

  nrg::StaticShader shader;
  shader.useAmbient(true);
  shader.useDiffuse(true);
  shader.useSpecular(true);
  shader.useLight(true);
  nrg::Renderer renderer{display};
  nrg::Camera camera;
  nrg::Light light{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)};

  glEnable(GL_DEPTH_TEST);

  while (!display.shouldClose()) {
    camera.move(display.window());
    shader.processInput(display.window());
    sphere.increaseRotation(1.0f, 1.0f, 0.0f);
    cube.increaseRotation(1.0f, 0.45f, 0.1f);
    monkey.increaseRotation(1.0f, 0.3f, 0.7f);
    barrel.increaseRotation(0.0f, 1.0f, 0.0f);
    renderer.prepare();
    shader.start();
    shader.loadView(camera);
    shader.loadLight(light);
    renderer.render(sphere, shader);
    renderer.render(cube, shader);
    renderer.render(monkey, shader);
    renderer.render(barrel, shader);
    shader.stop();
    display.update();
  }
  return 0;
}