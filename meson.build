project('nrg', 'cpp')

incdir = include_directories('include')
glfw_dep = dependency('glfw3')

executable('nrg', ['src/main.cpp','src/glad.cpp'], include_directories:incdir, dependencies:[glfw_dep])