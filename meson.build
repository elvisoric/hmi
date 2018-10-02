project('nrg', 'cpp', default_options:['cpp_std=c++14'])

incdir = include_directories('include')
glfw_dep = dependency('glfw3')

srcs = ['src/main.cpp','src/glad.cpp','src/Display.cpp','src/Loader.cpp', 'src/Renderer.cpp', 'src/ShaderProgram.cpp','src/stb_image.cpp']

executable('nrg', srcs, include_directories:incdir, dependencies:[glfw_dep])