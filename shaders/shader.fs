#version 330 core
in vec2 TexCoords;

out vec4 FragColor;
uniform sampler2D textureSampler;

uniform vec3 lightColor;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

uniform float shineDamper;
uniform float reflectivity;

void main()
{
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitLightVector = normalize(toLightVector);

    float nDot1 = dot(unitNormal, unitLightVector);
    float brightness = max(nDot1, 0.0);
    vec3 diffuse = brightness * lightColor;

    vec3 unitVectorToCamera = normalize(toCameraVector);
    vec3 lightDirection = -unitLightVector;
    vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

    float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor = pow(specularFactor, shineDamper);
    vec3 finalSpecular = dampedFactor * reflectivity * lightColor;

    FragColor = vec4(diffuse, 1.0) * texture(textureSampler, TexCoords) + vec4(finalSpecular, 1.0);
}