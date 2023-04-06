#version 330 core //fragment shader

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
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float ambientIntensity;
uniform float specularIntensity;
uniform float specularPower;
out vec4 fragmentColor;
uniform float constant;
uniform float linear;
uniform float quadratic;
uniform float cutOff;
uniform float outerCutOff;

void main()
{
    // look up the normal from the normal map, then reorient it with the current model transform via the TBN matrix
    vec3 textureNormal = vec3(texture(normalMap, shaderTexCoord));
    textureNormal = normalize(textureNormal * 2.0f - 1.0f);  // convert range from [0, 1] to [-1, 1]
    vec3 normalDir = normalize(shaderTBN * textureNormal);

    // calculate ambient
    vec3 lightAmbient = lightColor * ambient * ambientIntensity;

    
    vec3 lightDir = normalize(shaderLightPosition - shaderPosition);
    float cosAngle = dot(lightDir, normalize(-lightDirection));

    // calculate diffuse
    //float spotlightIntensity = smoothstep(cos(lightOuterAngle), cos(lightInnerAngle), cosAngle);
    vec3 lightDiffuse = diffuse * max(dot(normalDir, lightDir), 0.0f) * lightColor;

    // calculate specular
    vec3 viewDir = normalize(-shaderPosition);
    vec3 reflectDir = reflect(-lightDir, normalDir);
    vec3 lightSpecular = specular * pow(max(dot(reflectDir, viewDir), 0), specularPower) * lightColor * specularIntensity;

    //soft edges
    float theta = dot(lightDir, normalize(-lightDirection));
    float epsilon = (cutOff - outerCutOff);
    float intensity = clamp((theta - outerCutOff)/epsilon, 0.0f, 1.0f);
    lightDiffuse *= intensity;
    lightSpecular *= intensity;

    //calculate attenuation
    float distance = length(shaderLightPosition - shaderPosition);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    lightAmbient *= attenuation;
    lightDiffuse *= attenuation;
    lightSpecular *= attenuation;
    
    // compute final fragment color
    fragmentColor = vec4((lightAmbient + lightDiffuse + lightSpecular), 1.0f) * texture(diffuseMap, shaderTexCoord);
}