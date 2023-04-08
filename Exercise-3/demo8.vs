/******************************************************************************
 * This vertex shader is a modification of demo5.vs, adding a calculation for
 * the position of the fragment in "light space" so that we can compare that
 * fragment's depth with the depth stored in the shadow map (later in the
 * fragment shader).
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexTangent;
layout (location = 3) in vec2 vertexTexCoord;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;

uniform vec3 lightPosition;
uniform vec3 lightDirection;

out vec3 shaderPosition;
out mat3 shaderTBN;
out vec2 shaderTexCoord;

out vec3 shaderLightPosition;
out vec3 shaderLightDirection;
out float shaderLightConeAngle;
out float shaderLightOuterConeAngle;

///////////////////////////////////////////////////////////////////////////////
// added for shadow mapping
uniform mat4 lightTransform;
out vec4 shaderLightSpacePosition;
///////////////////////////////////////////////////////////////////////////////

void main()
{
    // combine the model and view transforms to get the camera space transform
    mat4 modelViewTransform = viewTransform * modelTransform;

    // compute the vertex's attributes in camera space
    shaderPosition = vec3(modelViewTransform * vec4(vertexPosition, 1.0f));
    shaderTexCoord = vertexTexCoord;

    // compute the normal transform as the transpose of the inverse of the camera transform,
    // then compute a TBN matrix using this transform
    mat3 normalTransform = mat3(transpose(inverse(modelViewTransform)));
    vec3 normal = normalize(normalTransform * vertexNormal);
    vec3 tangent = normalize(normalTransform * vertexTangent);
    vec3 bitangent = cross(normal, tangent);
    shaderTBN = mat3(tangent, bitangent, normal);

    // also compute the light position in camera space
    // (we want all lighting calculations to be done in camera space to avoid losing precision)
    shaderLightPosition = vec3(modelTransform * vec4(lightPosition, 1.0f));
    shaderLightDirection = normalize(vec3(modelTransform * vec4(lightDirection, 0.0f)));

    // we still need OpenGL to compute the final vertex position in projection space
    // to correctly determine where the fragments of the triangle actually go on the screen
    gl_Position = projectionTransform * vec4(shaderPosition, 1.0f);

    ///////////////////////////////////////////////////////////////////////////
    // also compute this fragment position from the light's point of view
    shaderLightSpacePosition = lightTransform * modelTransform * vec4(vertexPosition, 1.0f);
    ///////////////////////////////////////////////////////////////////////////
}
