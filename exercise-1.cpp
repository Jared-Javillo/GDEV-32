/******************************************************************************
 * This demo draws a textured quadrilateral on screen, plus the user can change
 * its position, rotation, and scaling using the WASD and arrow keys.
 *
 * The main differences from the previous demo are:
 * - Transformation matrices are now passed to the vertex shader (as uniforms).
 * - The OpenGL GL_BLEND feature is enabled to allow for drawing transparent
 *   texels using the alpha channel.
 *
 * Happy hacking! - eric
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
#define WINDOW_TITLE  "Hello Transform (use WASD and arrow keys)"
GLFWwindow *pWindow;



// define a vertex array to hold our vertices
float verticesCube[] =
{
    // position (x, y, z)    color (r, g, b)    texture coordinates (s, t)
    -0.50f, -0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
     0.50f, -0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
    -0.50f,  0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
     0.50f,  0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
     
     -0.50f, 0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
     0.50f, 0.50f,  0.00f,   1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    -0.50f,  0.50f,  -1.00f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
     0.50f,  0.50f,  -1.00f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,

    -0.50f, -0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
     0.50f, -0.50f,  0.00f,   1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
    -0.50f,  -0.50f,  -1.00f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
     0.50f,  -0.50f,  -1.00f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,

    -0.50f, -0.50f,  -1.00f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
     0.50f, -0.50f,  -1.00f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
    -0.50f,  0.50f,  -1.00f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
     0.50f,  0.50f,  -1.00f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,

    -0.50f,  -0.50f, -1.00f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
    -0.50f,  -0.50f, 0.00f,  1.0f, 1.0f, 1.0f,  -1.0f, 0.0f,
    -0.50f,  0.50f, -1.00f,  1.0f, 1.0f, 1.0f,  0.0f, -1.0f,
    -0.50f,  0.50f, 0.00f,  1.0f, 1.0f, 1.0f,  -1.0f, -1.0f,

    0.50f,  -0.50f, -1.00f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
    0.50f,  -0.50f, 0.00f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
    0.50f,  0.50f, -1.00f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    0.50f,  0.50f, 0.00f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
};

// define a vertex array to hold our vertices
float verticesPlane[] =
{
    // position (x, y, z)    color (r, g, b)    texture coordinates (s, t)
    -0.50f, -0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
     0.50f, -0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
    -0.50f,  0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
     0.50f,  0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
     
};


// define the triangles as triplets of indices into the vertex array
GLuint indicesCube[] =
{
    0, 1, 2,
    1, 3, 2,

    4, 5, 6,
    5, 7, 6,

    8, 9, 10,
    9, 11, 10,

    12, 13, 14,
    13, 15, 14,

    16, 17, 18,
    17, 19, 18,

    20, 21, 22,
    21, 23, 22,
};

GLuint indicesPlane[] =
{
    0, 1, 2,
    1, 3, 2,
};



// define different positions for the shape
glm::vec3 shapePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  2.0f, 0.0f),
    glm::vec3(-2.0f, -2.0f, 0.0f),
    glm::vec3(2.0f, -2.0f, 0.0f),
    glm::vec3( -2.0f, 2.0f, 0.0f)
};

// Sphere Variables
std::vector<float> verticesSphere;
std::vector<int> indicesSphere;

float radius = 2.0f;
float sectorCount = 72;                         // Column Count
float stackCount = 24;                          // Row Count
float xSphere, ySphere, zSphere, xySphere;      // vertex position
float r, g, b, lengthInv = 1.0f / radius;    // vertex Color
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

            // Color (r, g, b)
            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
            verticesSphere.push_back(r);
            verticesSphere.push_back(g);
            verticesSphere.push_back(b);

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





// define OpenGL object IDs to represent the vertex array, shader program, and texture in the GPU
GLuint vaoCube;         // vertex array object (stores the render state for our vertex array)
GLuint vboCube;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint eboCube;

GLuint vaoPlane;         // vertex array object (stores the render state for our vertex array)
GLuint vboPlane;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint eboPlane;         // element buffer object (stores the indices of the vertices to form triangles)

GLuint vaoSphere;         // vertex array object (stores the render state for our vertex array)
GLuint vboSphere;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint eboSphere;          // element buffer object (stores the indices of the vertices to form triangles)

GLuint shader;      // combined vertex and fragment shader
GLuint texture;     // texture object
GLuint texture2;    // second texture object

// variables controlling the object's position, rotation, and scaling
float  x            = 0.0f;
float  y            = 0.0f;
float  rotation     = 0.0f;
float  scaling      = 1.0f;
double previousTime = 0.0f;

//Variables controlling camera
float lastX = WINDOW_WIDTH/2, lastY = WINDOW_HEIGHT/2; //center of screen
float yaw = -90.0f;
float pitch = 0.0f;

bool firstMouse = true;

//initial camera vectors
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

//Handle Mouse controls
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //Is this the first mouse input
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    //Calculate mouse movement from last frame
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    //Mouse Sensitivity
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    //Handles yaw and pitch
    yaw   += xoffset;
    pitch += yoffset;
    
    //Pitch constraints
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    //Calculated cameraFront vector
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}  

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setupCube()
{
    // generate the VAO, VBO, and EBO objects and store their IDs in vao, vbo, and ebo, respectively
    glGenVertexArrays(1, &vaoCube);
    glGenBuffers(1, &vboCube);
    glGenBuffers(1, &eboCube);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vaoCube);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

    // upload our index array data to the newly-created EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboCube);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - the stride length of the vertex array is 8 floats (8 * sizeof(float))
    // - layout location 0 (position) is 3 floats and starts at the first float of the vertex array (offset 0)
    // - layout location 1 (color) is also 3 floats but starts at the fourth float (offset 3 * sizeof(float))
    // - layout location 2 (color) is 2 floats and starts at the seventh float (offset 6 * sizeof(float))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

    // enable the layout locations so they can be used by the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO, VBO, and EBO IDs, and follow the same process above to upload them to the GPU

    // load our shader program
    shader = gdevLoadShader("demo4.vs", "demo4.fs");
    if (! shader)
        return false;

    // load our texture
    texture = gdevLoadTexture("exercise-1.png", GL_REPEAT, true, true);
    if (! texture)
        return false;

    texture2 = gdevLoadTexture("pepe.jpg", GL_REPEAT, true, true);
    if (! texture2)
        return false;

    // enable OpenGL blending so that texels with alpha values less than one are drawn transparent
    // (you can omit these lines if you don't use alpha)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    return true;
}

bool setupPlane()
{
    // generate the VAO, VBO, and EBO objects and store their IDs in vao, vbo, and ebo, respectively
    glGenVertexArrays(1, &vaoPlane);
    glGenBuffers(1, &vboPlane);
    glGenBuffers(1, &eboPlane);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vaoPlane);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboPlane);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPlane), verticesPlane, GL_STATIC_DRAW);

    // upload our index array data to the newly-created EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboPlane);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPlane), indicesPlane, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - the stride length of the vertex array is 8 floats (8 * sizeof(float))
    // - layout location 0 (position) is 3 floats and starts at the first float of the vertex array (offset 0)
    // - layout location 1 (color) is also 3 floats but starts at the fourth float (offset 3 * sizeof(float))
    // - layout location 2 (color) is 2 floats and starts at the seventh float (offset 6 * sizeof(float))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

    // enable the layout locations so they can be used by the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

bool setupSphere()
{
    //Translate vector into native array
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
   
    // generate the VAO, VBO, and EBO objects and store their IDs in vao, vbo, and ebo, respectively
    glGenVertexArrays(1, &vaoSphere);
    glGenBuffers(1, &vboSphere);
    glGenBuffers(1, &eboSphere);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vaoSphere);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboSphere);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSphereArr), verticesSphereArr, GL_STATIC_DRAW);

    // upload our index array data to the newly-created EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboSphere);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSphereArr), indicesSphereArr, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - the stride length of the vertex array is 8 floats (8 * sizeof(float))
    // - layout location 0 (position) is 3 floats and starts at the first float of the vertex array (offset 0)
    // - layout location 1 (color) is also 3 floats but starts at the fourth float (offset 3 * sizeof(float))
    // - layout location 2 (color) is 2 floats and starts at the seventh float (offset 6 * sizeof(float))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

    // enable the layout locations so they can be used by the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

// called by the main function to do rendering per frame
void render()
{
    // find the elapsed time since the last frame
    double currentTime = glfwGetTime();
    double elapsedTime = (currentTime - previousTime);
    previousTime = currentTime;

    // handle key events
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    //Handle pitching UP
    if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS) 
    {
        pitch += 1;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }

    //Handle pitching down
    if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS) 
    {
        pitch -= 1;
        
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }

    
    glfwSetCursorPosCallback(pWindow, mouse_callback); 
    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
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
    viewTransform = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewTransform"),
                       1, GL_FALSE, glm::value_ptr(viewTransform));

    // ... then draw our triangles
    glBindVertexArray(vaoCube);
    for (unsigned int i = 0; i < 5; i++)
    {
        // ... set up the model matrix...
        glm::mat4 modelTransform = glm::mat4(1.0f);  // set to identity first
        modelTransform = glm::translate(modelTransform, shapePositions[i] - glm::vec3(0.0f,0.0f,0.0f));               // translate x and y
        modelTransform = glm::rotate(modelTransform, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 1.0f));  // rotate around z
        modelTransform = glm::scale(modelTransform, glm::vec3(scaling, scaling, 1.0f));       // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));
        
        if (i < 2){
            // ... set the active texture...
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
        } else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture2);
        }
        glDrawElements(GL_TRIANGLES, sizeof(indicesCube) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(vaoPlane);
    for (unsigned int i = 1; i < 5; i++)
    {
        // ... set up the model matrix...
        glm::mat4 modelTransform = glm::mat4(1.0f);  // set to identity first
        modelTransform = glm::translate(modelTransform, shapePositions[i] * sin(float(glfwGetTime()) * 1));               // translate x and y
        modelTransform = glm::scale(modelTransform, glm::vec3(scaling, scaling, 1.0f));       // scale x and y
        
        
        if (i < 2){
            // ... set the active texture...
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
        } else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture2);
        }
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));
        glDrawElements(GL_TRIANGLES, sizeof(indicesPlane) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(vaoSphere);
    // ... set up the model matrix...
    glm::mat4 modelTransform = glm::mat4(1.0f);  // set to identity first
    //modelTransform = glm::rotate(modelTransform, float(glfwGetTime()) * 1, glm::vec3(0.0f, 1.0f, 0.0f));
    //modelTransform = glm::translate(modelTransform, glm::vec3( 0.0f, 0.0f, 20.0f));  
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                    1, GL_FALSE, glm::value_ptr(modelTransform));
    

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glDrawElements(GL_TRIANGLE_STRIP, 39744, GL_UNSIGNED_INT, (void*)0);
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
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    // initialize GLAD, which acts as a library loader for the current OS's native OpenGL library
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // if our initial setup is successful...
    if (setupPlane() && setupCube() && setupSphere())
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
