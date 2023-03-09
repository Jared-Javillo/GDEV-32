#version 330 core

in vec3 shaderPosition;
in mat3 shaderTBN;
in vec2 shaderTexCoord;
in vec3 shaderLightPosition;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform float lightInnerAngle;
uniform float lightOuterAngle;
uniform vec3 lightColor;
uniform float ambientIntensity;
uniform float specularIntensity;
uniform float specularPower;
out vec4 fragmentColor;

void main()
{
    // look up the normal from the normal map, then reorient it with the current model transform via the TBN matrix
    vec3 textureNormal = vec3(texture(normalMap, shaderTexCoord));
    textureNormal = normalize(textureNormal * 2.0f - 1.0f);  // convert range from [0, 1] to [-1, 1]
    vec3 normalDir = normalize(shaderTBN * textureNormal);

    // calculate ambient
    vec3 lightAmbient = lightColor * ambientIntensity;

    
    vec3 lightDir = normalize(shaderLightPosition - shaderPosition);
    float cosAngle = dot(lightDir, normalize(-lightDirection));

    // calculate diffuse
    float spotlightIntensity = smoothstep(cos(lightOuterAngle), cos(lightInnerAngle), cosAngle);
    vec3 lightDiffuse = max(dot(normalDir, lightDir), 0.0f) * spotlightIntensity * lightColor;

    // calculate specular
    vec3 viewDir = normalize(-shaderPosition);
    vec3 reflectDir = reflect(-lightDir, normalDir);
    vec3 lightSpecular = pow(max(dot(reflectDir, viewDir), 0), specularPower) * spotlightIntensity * lightColor * specularIntensity;

    // compute final fragment color
    fragmentColor = vec4((lightAmbient + lightDiffuse + lightSpecular), 1.0f) * texture(diffuseMap, shaderTexCoord);
}