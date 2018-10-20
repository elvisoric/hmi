#include <Display.h>

namespace {
GLFWwindow* createWindow(float width, float height, const std::string& title) {
  if (!glfwInit()) {
    throw "Failed to initialize glfw";
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window =
      glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window) {
    glfwTerminate();
    throw "Failed to create glfw window ";
  }
  glfwMakeContextCurrent(window);

  // initialize glad before any opengl call
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw "Failed to initialize GLAD";
  }
  return window;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  nrg::FramebufferChangeSubject::instance().notify(width, height);
}

void scrollCallback(GLFWwindow* window, double dx, double dy) {
  nrg::ScrollSubject::instance().notify(dx, dy);
}
void mouseCallback(GLFWwindow* window, double x, double y) {
  nrg::MouseInputSubject::instance().notify(x, y);
}

}  // anonymous namespace

namespace nrg {
Display::Display(GLFWwindow* window, float width, float height)
    : window_{window}, width_{width}, height_{height} {
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  auto f = [this](int width, int height) {
    width_ = width;
    height_ = height;
  };
  FramebufferChangeSubject::instance().subscribe(f);
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
Display::~Display() { glfwDestroyWindow(window_); }

Display createDisplay(float width, float height) {
  GLFWwindow* window =
      createWindow(width, height, "Interakcija Covjek Racunar");
  return Display{window, width, height};
}

void Display::processInput() {
  if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window_, true);
}
}  // namespace nrg