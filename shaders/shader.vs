#version 330 core
in vec3 position;
in vec2 aTexCoords;

out vec2 TexCoords;
uniform mat4 transformationMatrix;

void main()
{
    gl_Position = transformationMatrix * vec4(position, 1.0);
    TexCoords = aTexCoords;
}