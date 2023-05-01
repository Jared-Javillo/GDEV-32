#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform float time;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    vec2 uv = texCoords;
    float distortion = 0.01f;
    float noise = 0.01f;
    float distortionSpeed = 1.0f;
    float noiseSpeed = 1.0f;

    // Add some noise
    float noiseValue = rand(uv + time * noiseSpeed) * noise - noise / 2.0;
    vec3 color = texture(screenTexture, uv).rgb + vec3(noiseValue);

    // Add some distortion
    float distortionValue = rand(uv + time * distortionSpeed) * distortion - distortion / 2.0;
    vec2 distortionCoords = uv + vec2(distortionValue, distortionValue);
    color = texture(screenTexture, distortionCoords).rgb;

    FragColor = vec4(color, 1.0);
}