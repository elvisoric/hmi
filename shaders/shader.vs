#version 330 core
in vec3 position;
in vec2 aTexCoords;

out vec2 TexCoords;
uniform mat4 transformationMatrix;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection*view*transformationMatrix * vec4(position, 1.0);
    TexCoords = aTexCoords;
}