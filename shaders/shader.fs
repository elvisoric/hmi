#version 330 core
in vec2 TexCoords;

out vec4 FragColor;
uniform sampler2D textureSampler;

uniform vec3 lightColor;
in vec3 surfaceNormal;
in vec3 toLightVector;

void main()
{
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitLightVector = normalize(toLightVector);

    float nDot1 = dot(unitNormal, unitLightVector);
    float brightness = max(nDot1, 0.0);
    vec3 diffuse = brightness * lightColor;
    FragColor = vec4(diffuse, 1.0) * texture(textureSampler, TexCoords);
}