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

class Models {
 public:
  void add(const std::string& key, const nrg::Entity& entity) {
    ids_.push_back(key);
    entities_.emplace(key, entity);
  }
  inline const std::vector<std::string>& ids() const { return ids_; }
  inline nrg::Entity entity(const std::string& key) { return entities_[key]; }

 private:
  std::unordered_map<std::string, nrg::Entity> entities_;
  std::vector<std::string> ids_;
};

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

namespace ImGui {
static auto vector_getter = [](void* vec, int idx, const char** out_text) {
  auto& vector = *static_cast<std::vector<std::string>*>(vec);
  if (idx < 0 || idx >= static_cast<int>(vector.size())) {
    return false;
  }
  *out_text = vector.at(idx).c_str();
  return true;
};

bool Combo(const char* label, int* currIndex,
           std::vector<std::string>& values) {
  if (values.empty()) {
    return false;
  }
  return Combo(label, currIndex, vector_getter, static_cast<void*>(&values),
               values.size());
}

bool ListBox(const char* label, int* currIndex,
             std::vector<std::string>& values) {
  if (values.empty()) {
    return false;
  }
  return ListBox(label, currIndex, vector_getter, static_cast<void*>(&values),
                 values.size());
}

}  // namespace ImGui

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

struct EntityManager {
  std::unordered_map<std::string, nrg::Entity> entities;
  std::vector<std::string> ids() const {
    std::vector<std::string> result;
    for (auto& e : entities) {
      result.push_back(e.first);
    }
    return result;
  }
  static nrg::Entity specialCase;
  nrg::Entity* current = &specialCase;
};
nrg::Entity EntityManager::specialCase = nrg::Entity{};

void modelsWindow(Models& models, EntityManager& manager) {
  ImGui::Begin("Models");
  auto listbox_items = models.ids();
  static int listbox_item_current = 0;
  ImGui::ListBox("Models", &listbox_item_current, listbox_items);
  static char str0[128] = "Hello, world!";
  ImGui::InputText("Entity Name:", str0, IM_ARRAYSIZE(str0));
  if (ImGui::Button("Add Model")) {
    std::string name{str0};
    manager.entities[name] =
        models.entity(listbox_items.at(listbox_item_current));
  }
  ImGui::End();
}

void entitiesWindow(EntityManager& manager) {
  ImGui::Begin("Entities");
  auto listbox_items = manager.ids();
  static int listbox_item_current = 0;
  ImGui::ListBox("Enities", &listbox_item_current, listbox_items);
  if (ImGui::Button("Use selected") && !listbox_items.empty()) {
    std::string name = listbox_items.at(listbox_item_current);
    std::cout << name << std::endl;
    manager.current = &(manager.entities.at(name));
  }
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
  Models models;

  auto cubeModel =
      loadTexturedModel("res/cube.obj", "res/container.png", loader);
  auto cubeSpecular = loader.loadTexture("res/container_specular.png");
  cubeModel.texture().reflectivity(1);
  cubeModel.texture().shineDamper(64);
  cubeModel.texture().specularMap(cubeSpecular);
  nrg::Entity cube{cubeModel, glm::vec3(0.0f), 0.0f, 0.0f, 0.0f, 1.0f};

  models.add("cube", cube);
  auto sphereModel =
      loadTexturedModel("res/sphere.obj", "res/sphere.png", loader);
  sphereModel.texture().reflectivity(1);
  sphereModel.texture().shineDamper(32);
  nrg::Entity sphere{sphereModel, glm::vec3(0.0f), 0.0f, 0.0f, 0.0f, 1.0f};

  models.add("sphere", sphere);
  auto monkeyModel =
      loadTexturedModel("res/monkey.obj", "res/monkey.png", loader);
  monkeyModel.texture().reflectivity(1);
  monkeyModel.texture().shineDamper(32);
  nrg::Entity monkey{monkeyModel, glm::vec3(0.0f), 0.0f, 0.0f, 0.0f, 1.0f};

  models.add("monkey", monkey);
  auto barrelModel =
      loadTexturedModel("res/barrel.obj", "res/barrel.png", loader);
  auto barrelSpecularMap = loader.loadTexture("res/barrelS.png");
  barrelModel.texture().reflectivity(0.5f);
  barrelModel.texture().shineDamper(10);
  barrelModel.texture().specularMap(barrelSpecularMap);
  nrg::Entity barrel{barrelModel, glm::vec3(0.0), 0.0f, 0.0f, 0.0f, 1.0f};

  models.add("barrel", barrel);

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
  EntityManager manager;

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

    for (auto& entity : manager.entities)
      renderer.render(entity.second, shader);

    shader.stop();

    basicShader.start();
    basicShader.color(gridColor);
    basicShader.loadView(cameraHolder.camera());
    renderer.render(grid, basicShader);
    basicShader.stop();

    transformationsWindow(manager.current->position(),
                          manager.current->rotation(),
                          manager.current->scaling());
    environmentWindow(gridColor);
    modelsWindow(models, manager);
    entitiesWindow(manager);
    renderImgui();
    display.update();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  return 0;
}