#version 330 core
in vec3 aPos;
out vec3 color;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    color = vec3(aPos.x +0.5, 1.0, aPos.y+0.5);
}