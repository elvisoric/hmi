project('nrg', 'cpp', default_options:['cpp_std=c++14'])

incdir = include_directories(['include', 'include/imgui'])
glfw_dep = dependency('glfw3')

srcs = [
    'src/main.cpp',
    'src/glad.cpp',
    'src/Display.cpp',
    'src/Loader.cpp',
    'src/Renderer.cpp',
    'src/ShaderProgram.cpp',
    'src/stb_image.cpp',
    'src/Maths.cpp',
    'src/imgui.cpp',
    'src/imgui_impl_glfw.cpp',
    'src/imgui_widgets.cpp',
    'src/imgui_draw.cpp',
    'src/imgui_demo.cpp',
    'src/imgui_impl_opengl3.cpp']

executable('nrg', srcs, include_directories:incdir, dependencies:[glfw_dep])