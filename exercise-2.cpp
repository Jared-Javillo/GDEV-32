/******************************************************************************
 Keys 1-9
 Light Direction
 Ambient Intensity
 Specular Intensity
 Shininess
 Keys Z,X,C,V
 Spotlight Radius
 Spotlight Outer Radius
 Keys U,O
 Light Distance (to show attenuation)
 *****************************************************************************/
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>
#include <bits/stdc++.h>
#include <vector>
#include <cmath>

// change this to your desired window attributes
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 360
#define PI 3.14
#define WINDOW_TITLE  "Hello Lighting (use WASDQE keys for camera, IKJLUO keys for light)"
GLFWwindow *pWindow;


void printMat4(const glm::mat4& matrix)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// model
float vertices[] =
{
    // position (x, y, z)    normal (x, y, z)     tangent (x, y, z)    texture coordinates (s, t)

    // ground plane
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 4.0f,

    // cube top
    -1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    -1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    -1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube back
     1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube right
     1.00f, -1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     1.00f, -1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     1.00f,  1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     1.00f, -1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     1.00f,  1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     1.00f,  1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube left
    -1.00f, -1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    -1.00f, -1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
    -1.00f,  1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -1.00f, -1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    -1.00f,  1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -1.00f,  1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f
};

// Sphere Variables
std::vector<float> verticesSphere;
std::vector<int> indicesSphere;

float radius = 2.0f;
float sectorCount = 72;                         // Column Count
float stackCount = 24;                          // Row Count
float xSphere, ySphere, zSphere, xySphere;      // vertex position
float nx, ny ,nz ,r, g, b, lengthInv = 1.0f / radius;    // vertex normals and color
float tx, ty, tz, px, py, pz, dotProdTemp, mag;
float s, t;                                     // vertex tex

float sectorStep = 2 * PI / sectorCount;    //Angles for each sector count
float stackStep = PI / stackCount;          //Angles for each stack count
float sectorAngle, stackAngle;

// clear memory of prev arrays
void SetupSphereData()
{
    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xySphere = radius * cosf(stackAngle);             // r * cos(u)
        zSphere = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            xSphere = xySphere * cosf(sectorAngle);             // r * cos(u) * cos(v)
            ySphere = xySphere * sinf(sectorAngle);             // r * cos(u) * sin(v)
            verticesSphere.push_back(xSphere);
            verticesSphere.push_back(ySphere);
            verticesSphere.push_back(zSphere);

            // normalized vertex normal (nx, ny, nz)
            nx = xSphere * lengthInv;
            ny = ySphere * lengthInv;
            nz = zSphere * lengthInv;
            verticesSphere.push_back(nx);
            verticesSphere.push_back(ny);
            verticesSphere.push_back(nz);

            // normalized vertex tangent (tx, ty, tz)
            mag = sqrt(xSphere * xSphere + ySphere * ySphere + zSphere * zSphere);
            px = xSphere/mag * radius;
            py = ySphere/mag * radius;
            pz = zSphere/mag * radius;

            tx = -py;
            ty = px;
            tz = 0.0;

            mag = sqrt(tx*tx + ty*ty + tz*tz);

            verticesSphere.push_back(tx/mag);
            verticesSphere.push_back(ty/mag);
            verticesSphere.push_back(tz/mag);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            verticesSphere.push_back(s);
            verticesSphere.push_back(t);
        }
    }

    //// Vertices Indices
    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indicesSphere.push_back(k1);
                indicesSphere.push_back(k2);
                indicesSphere.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                indicesSphere.push_back(k1 + 1);
                indicesSphere.push_back(k2);
                indicesSphere.push_back(k2 + 1);
            }
        }
    }
}

// OpenGL object IDs
GLuint vao;
GLuint vbo;
GLuint shader;

GLuint vaoSphere;         // vertex array object (stores the render state for our vertex array)
GLuint vboSphere;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint eboSphere;          // element buffer object (stores the indices of the vertices to form triangles)


GLuint texture[2];

// helper struct for defining spherical polar coordinates
struct polar
{
    float radius      =   8.0f;   // distance from the origin
    float inclination = -20.0f;   // angle on the YZ vertical plane
    float azimuth     =  45.0f;   // angle on the XZ horizontal plane

    // sanity ranges to prevent strange behavior like flipping axes, etc.
    // (you can change these as you see fit)
    static constexpr float minRadius      =   0.1f;
    static constexpr float maxRadius      =  20.0f;
    static constexpr float minInclination = -89.0f;
    static constexpr float maxInclination =  89.0f;

    // restricts the coordinates to sanity ranges
    void clamp()
    {
        if (radius < minRadius)
            radius = minRadius;
        if (radius > maxRadius)
            radius = maxRadius;
        if (inclination < minInclination)
            inclination = minInclination;
        if (inclination > maxInclination)
            inclination = maxInclination;
    }

    // converts the spherical polar coordinates to a vec3 in Cartesian coordinates
    glm::vec3 toCartesian()
    {
        glm::mat4 mat = glm::mat4(1.0f);  // set to identity first
        mat = glm::rotate(mat, glm::radians(azimuth), glm::vec3(0.0f, 1.0f, 0.0f));
        mat = glm::rotate(mat, glm::radians(inclination), glm::vec3(1.0f, 0.0f, 0.0f));
        return mat * glm::vec4(0.0f, 0.0f, radius, 1.0f);
    }
};

// variables for tracking camera and light position
polar camera;
glm::vec3 lightPosition = glm::vec3(0.0f, 10.0f, 0.0f);
glm::vec3 lightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
float lightInnerAngle = 0.174533f;
float lightOuterAngle = 0.2f;
float cutOff = glm::cos(glm::radians(12.5f));
float outerCutOff = glm::cos(glm::radians(17.5f));

glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
float ambientIntensity = 1.0f;
float specularIntensity = 1.0f;
float specularPower = 1.0f;
double previousTime = 0.0;
float constant = 1.0f;
float linear = 0.07f;
float quadratic = 0.017f;

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // upload the model to the GPU (explanations omitted for brevity)
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (9 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // load our shader program
    shader = gdevLoadShader("demo5n.vs", "demo5n.fs");
    if (! shader)
        return false;

    // since we now use multiple textures, we need to set the texture channel for each texture
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(shader, "normalMap"),  1);

    // load our textures
    texture[0] = gdevLoadTexture("texture1.png", GL_REPEAT, true, true);
    texture[1] = gdevLoadTexture("normal1.png", GL_REPEAT, true, true);
    if (! texture[0] || ! texture[1])
        return false;

    // enable z-buffer depth testing and face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

bool setupSphere()
{
    int n = verticesSphere.size();
    int m = indicesSphere.size();

    int indicesSphereArr[m];
    float verticesSphereArr[n];

    std::copy(verticesSphere.begin(),verticesSphere.end(),verticesSphereArr);
    std::copy(indicesSphere.begin(),indicesSphere.end(),indicesSphereArr);

    std::cout << "Vertices: " <<verticesSphere.size() << "\n";
    std::cout << "Indices: "<< indicesSphere.size() << "\n";

    std::cout << "Vertices: " << sizeof(verticesSphereArr) << "\n";
    std::cout << "Indices: " << sizeof(indicesSphereArr) << "\n";
   
    glGenVertexArrays(1, &vaoSphere);
    glGenBuffers(1, &vboSphere);
    glGenBuffers(1, &eboSphere);

    glBindVertexArray(vaoSphere);

    glBindBuffer(GL_ARRAY_BUFFER, vboSphere);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSphereArr), verticesSphereArr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboSphere);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSphereArr), indicesSphereArr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,11 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (9 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // find the elapsed time since the last frame
    double currentTime = glfwGetTime();
    double elapsedTime = (currentTime - previousTime);
    previousTime = currentTime;

    // define how much to rotate and translate according to time
    float rotationAmount = 100.0f * elapsedTime;
    float translationAmount = 10.0f * elapsedTime;

    // handle key events for camera
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
        camera.radius -= translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
        camera.radius += translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
        camera.azimuth -= rotationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
        camera.azimuth += rotationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS)
        camera.inclination += rotationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)
        camera.inclination -= rotationAmount;
    camera.clamp();

    // get the Cartesian coordinates of the camera
    glm::vec3 cameraPosition = camera.toCartesian();

    // get a "forward" vector for controlling the light position
    glm::vec3 lightForward = glm::normalize(glm::vec3(-cameraPosition.x, 0.0f, -cameraPosition.z));

    if (glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_PRESS)
        lightPosition += lightForward * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_K) == GLFW_PRESS)
        lightPosition -= lightForward * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_J) == GLFW_PRESS)
        lightPosition -= glm::cross(lightForward, glm::vec3(0.0f, 1.0f, 0.0f)) * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_L) == GLFW_PRESS)
        lightPosition += glm::cross(lightForward, glm::vec3(0.0f, 1.0f, 0.0f)) * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_PRESS)
        lightPosition -= glm::vec3(0.0f, 1.0f, 0.0f) * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS)
        lightPosition += glm::vec3(0.0f, 1.0f, 0.0f) * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_PRESS)
        lightDirection.x += 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_2) == GLFW_PRESS)
        lightDirection.x -= 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_3) == GLFW_PRESS)
        lightDirection.z += 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_4) == GLFW_PRESS)
        lightDirection.z -= 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_5) == GLFW_PRESS)
        ambientIntensity += 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_6) == GLFW_PRESS)
        ambientIntensity -= 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_7) == GLFW_PRESS)
        specularIntensity += 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_8) == GLFW_PRESS)
        specularIntensity -= 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_9) == GLFW_PRESS)
        specularPower += 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_0) == GLFW_PRESS)
        specularPower -= 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_Z) == GLFW_PRESS)
        cutOff += 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_X) == GLFW_PRESS)
        cutOff -= 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_C) == GLFW_PRESS)
        outerCutOff += 0.5f * elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_V) == GLFW_PRESS)
        outerCutOff -= 0.5f * elapsedTime;

    // clear the whole frame
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using our shader program...
    glUseProgram(shader);

    // ... set up the projection matrix...
    glm::mat4 projectionTransform;
    projectionTransform = glm::perspective(glm::radians(45.0f),                   // fov
                                           (float) WINDOW_WIDTH / WINDOW_HEIGHT,  // aspect ratio
                                           0.1f,                                  // near plane
                                           100.0f);                               // far plane
    glUniformMatrix4fv(glGetUniformLocation(shader, "projectionTransform"),
                       1, GL_FALSE, glm::value_ptr(projectionTransform));

    // ... set up the view matrix...
    glm::mat4 viewTransform;
    viewTransform = glm::lookAt(cameraPosition,                // eye position
                                glm::vec3(0.0f, 0.0f, 0.0f),   // center position
                                glm::vec3(0.0f, 1.0f, 0.0f));  // up vector
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewTransform"),
                       1, GL_FALSE, glm::value_ptr(viewTransform));

    // ... set up the model matrix... (just identity for this demo)
    glm::mat4 modelTransform = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    glm::mat4 lightTransform = glm::translate(glm::mat4(1.0f), lightPosition);
    glUniformMatrix4fv(glGetUniformLocation(shader, "lightTransform"),
                       1, GL_FALSE, glm::value_ptr(lightTransform));
    printMat4(lightTransform);

    // ... set up the spotlight attributes
    glUniform3fv(glGetUniformLocation(shader, "lightPosition"),
                 1, glm::value_ptr(lightPosition));
    glUniform3fv(glGetUniformLocation(shader, "lightDirection"),
                 1, glm::value_ptr(lightDirection));
    glUniform1f(glGetUniformLocation(shader, "lightInnerAngle"),
                 lightInnerAngle);
    glUniform1f(glGetUniformLocation(shader, "lightOuterAngle"),
                 lightOuterAngle);
    glUniform1f(glGetUniformLocation(shader, "cutOff"),
                 cutOff);
    glUniform1f(glGetUniformLocation(shader, "outerCutOff"),
                 outerCutOff);

    glUniform3fv(glGetUniformLocation(shader, "lightColor"),
                 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(shader, "ambient"),
                 1, glm::value_ptr(ambient));
    glUniform3fv(glGetUniformLocation(shader, "diffuse"),
                 1, glm::value_ptr(diffuse));
    glUniform3fv(glGetUniformLocation(shader, "specular"),
                 1, glm::value_ptr(specular));
    glUniform1f(glGetUniformLocation(shader, "ambientIntensity"),
                 ambientIntensity);
    glUniform1f(glGetUniformLocation(shader, "specularIntensity"),
                 specularIntensity);
    glUniform1f(glGetUniformLocation(shader, "specularPower"),
                 specularPower);
    glUniform1f(glGetUniformLocation(shader, "constant"),
                 constant);
    glUniform1f(glGetUniformLocation(shader, "linear"),
                 linear);
    glUniform1f(glGetUniformLocation(shader, "quadratic"),
                 quadratic);

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (11 * sizeof(float)));

    glBindVertexArray(vaoSphere);
    // ... set up the model matrix...
    //modelTransform = glm::rotate(modelTransform, float(glfwGetTime()) * 1, glm::vec3(0.0f, 1.0f, 0.0f));
    //modelTransform = glm::translate(modelTransform, glm::vec3( 0.0f, 0.0f, 20.0f));  
    //modelTransform = glm::mat4(1.0f);

    glDrawElements(GL_TRIANGLES, 39744, GL_UNSIGNED_INT, (void*)0);
}

/*****************************************************************************/

// handler called by GLFW when there is a keyboard event
void handleKeys(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    // pressing Esc closes the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
}

// handler called by GLFW when the window is resized
void handleResize(GLFWwindow* pWindow, int width, int height)
{
    // tell OpenGL to do its drawing within the entire "client area" (area within the borders) of the window
    glViewport(0, 0, width, height);
}

// main function
int main(int argc, char** argv)
{
    // initialize GLFW and ask for OpenGL 3.3 core
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    SetupSphereData();

    // create a GLFW window with the specified width, height, and title
    pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (! pWindow)
    {
        // gracefully terminate if we cannot create the window
        std::cout << "Cannot create the GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    // enable vertical sync and aspect-ratio correction on the GLFW window,
    // and make the window the current context of subsequent OpenGL commands
    glfwSwapInterval(1);
    glfwSetWindowAspectRatio(pWindow, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwMakeContextCurrent(pWindow);

    // set up callback functions to handle window system events
    glfwSetKeyCallback(pWindow, handleKeys);
    glfwSetFramebufferSizeCallback(pWindow, handleResize);

    // don't miss any momentary keypresses
    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

    // initialize GLAD, which acts as a library loader for the current OS's native OpenGL library
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // if our initial setup is successful...
    if (setup() && setupSphere())
    {
        // do rendering in a loop until the user closes the window
        while (! glfwWindowShouldClose(pWindow))
        {
            // render our next frame
            // (by default, GLFW uses double-buffering with a front and back buffer;
            // all drawing goes to the back buffer, so the frame does not get shown yet)
            render();
            
            // swap the GLFW front and back buffers to show the next frame
            glfwSwapBuffers(pWindow);

            // process any window events (such as moving, resizing, keyboard presses, etc.)
            glfwPollEvents();
        }
    }

    // gracefully terminate the program
    glfwTerminate();
    return 0;
}
