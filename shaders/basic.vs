#version 330 core
in vec3 position;

uniform mat4 transformationMatrix;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec4 worldPosition = transformationMatrix * vec4(position, 1.0);
    gl_Position = projection*view* worldPosition;
}