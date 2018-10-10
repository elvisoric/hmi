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

uniform float useAmbientLight;
uniform float useDiffuseLight;
uniform float useSpecularLight;
uniform float useLight;

void main()
{
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitLightVector = normalize(toLightVector);

    float ambientFactor = 0.0;
    vec3 diffuse;
    if(useAmbientLight >0.5){
        ambientFactor = 0.2;
    }
    float brightness = ambientFactor;
    if(useDiffuseLight > 0.5){
        float nDot1 = dot(unitNormal, unitLightVector);
        brightness = max(nDot1, ambientFactor);
    }
    diffuse = brightness * lightColor;

    vec3 unitVectorToCamera = normalize(toCameraVector);
    vec3 lightDirection = -unitLightVector;
    vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

    float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor = pow(specularFactor, shineDamper);
    float useSpecularFactor = 0.0;
    if(useSpecularLight >0.5) useSpecularFactor= 1.0;
    vec3 finalSpecular = useSpecularFactor * dampedFactor * reflectivity * lightColor;

    if(useLight > 0.5){
        FragColor = vec4(diffuse, 1.0) * texture(textureSampler, TexCoords) + vec4(finalSpecular, 1.0);
    }else{
        FragColor = texture(textureSampler, TexCoords);
    }
}