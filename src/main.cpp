#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>  // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>  // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Action.h>
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

#include <BasicShader.h>
#include <Grid.h>

void renderImgui() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void imguiNewFrame() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void transformationsWindow(glm::vec3& position, glm::vec3& rotation,
                           float& scale) {
  ImGui::Begin("Transformations");
  if (ImGui::CollapsingHeader("Translate")) {
    ImGui::InputFloat("X", &position.x, 0.05f, 1.0f);
    ImGui::InputFloat("Y", &position.y, 0.05f, 1.0f);
    ImGui::InputFloat("Z", &position.z, 0.05f, 1.0f);
  }
  if (ImGui::CollapsingHeader("Rotate")) {
    ImGui::SliderFloat("X angle", &rotation.x, -180.0f, 180.0f, "%.2f", 2.0f);
    ImGui::SliderFloat("Y angle", &rotation.y, -180.0f, 180.0f, "%.2f", 2.0f);
    ImGui::SliderFloat("Z angle", &rotation.z, -180.0f, 180.0f, "%.2f", 2.0f);
  }
  if (ImGui::CollapsingHeader("Scale")) {
    ImGui::InputFloat("Factor", &scale, 0.05f, 1.0f);
  }
  ImGui::End();
}

void environmentWindow(glm::vec3& color) {
  ImGui::Begin("Environment");
  float col1[3] = {color.x, color.y, color.z};
  ImGui::ColorEdit3("Grid color", col1);
  color.x = col1[0];
  color.y = col1[1];
  color.z = col1[2];
  ImGui::End();
}

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
  nrg::Display display = nrg::createDisplay(1024.0f, 768.0f);
  nrg::Loader loader;

  auto cubeModel =
      loadTexturedModel("res/cube.obj", "res/container.png", loader);
  auto cubeSpecular = loader.loadTexture("res/container_specular.png");
  cubeModel.texture().reflectivity(1);
  cubeModel.texture().shineDamper(64);
  cubeModel.texture().specularMap(cubeSpecular);
  nrg::Entity cube{cubeModel, glm::vec3(0.0f), 0.0f, 0.0f, 0.0f, 1.0f};

  auto gridData = nrg::makeGrid(16, 16, 1.5f);
  auto gridModel = loader.loadVAO(gridData);
  nrg::LineEntity grid{gridModel, glm::vec3(0.0f), 0.0f, 0.0f, 0.0f, 1.0f};

  nrg::StaticShader shader;
  shader.start();
  shader.connectTextureUnits();
  shader.stop();
  shader.useAmbient(false);
  shader.useDiffuse(false);
  shader.useSpecular(false);
  shader.useLight(false);
  nrg::Renderer renderer{display};
  nrg::FpsCamera fps;
  nrg::BasicCamera basic;
  nrg::CameraHolder cameraHolder{fps, basic};
  cameraHolder.change();
  nrg::Light light{glm::vec3(0.0f, 3.0f, 4.0f), glm::vec3(1.0f)};

  nrg::BasicShader basicShader;
  glm::vec3 gridColor{1.0};
  basicShader.start();
  basicShader.color(gridColor);
  basicShader.stop();

  glEnable(GL_DEPTH_TEST);
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard
  // Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable
  // Gamepad Controls
  const char* glsl_version = "#version 330";
  ImGui_ImplGlfw_InitForOpenGL(display.window(), true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImGui::StyleColorsDark();

  bool showDemoWindow = true;
  while (!display.shouldClose()) {
    display.pollEvents();

    imguiNewFrame();

    ImGui::ShowDemoWindow(&showDemoWindow);

    display.processInput();
    nrg::ActionSubject::instance().processInput(display.window());
    cameraHolder.processInput(display.window());
    shader.processInput(display.window());

    renderer.prepare();
    shader.start();
    shader.loadView(cameraHolder.camera());
    shader.loadLight(light);

    renderer.render(cube, shader);

    shader.stop();

    basicShader.start();
    basicShader.color(gridColor);
    basicShader.loadView(cameraHolder.camera());
    renderer.render(grid, basicShader);
    basicShader.stop();

    transformationsWindow(cube.position(), cube.rotation(), cube.scaling());
    environmentWindow(gridColor);
    renderImgui();
    display.update();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  return 0;
}