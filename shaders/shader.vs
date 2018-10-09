#version 330 core
in vec3 position;
in vec2 aTexCoords;
in vec3 normal;

out vec2 TexCoords;
uniform mat4 transformationMatrix;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 lightPosition;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;


void main()
{
    vec4 worldPosition = transformationMatrix * vec4(position, 1.0);

    gl_Position = projection*view* worldPosition;
    TexCoords = aTexCoords;

    surfaceNormal = (transformationMatrix * vec4(normal, 0.0)).xyz;
    toLightVector =  lightPosition - worldPosition.xyz;
    toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
}