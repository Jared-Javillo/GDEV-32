/******************************************************************************
 * This demo is a modification of demo5.cpp to implement shadow mapping.
 *
 * Make sure the support files of demo5 are in the same folder before you
 * attempt to run this demo!
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>
#include "../camera.h"

// change this to your desired window attributes
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 360
#define WINDOW_TITLE  "Hello Shadows (use WASDQE keys for camera, IKJLUO keys for light)"
GLFWwindow *pWindow;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

float LegHeightTop = -1.0f;
float LegHeightBot = -2.0f;
float Zmin = 1.0f;
float Zmax = Zmin - 0.25f;
float Xmin = -1.0f;
float Xmax = Xmin + 0.25f;
float chairXmin = 2.00f;
float chairXmax = chairXmin + 1.00f;
float chairZmin = -0.50f;
float chairZmax = chairZmin + 1.00f;

// model
float vertices[] =
{
    // position (x, y, z)    normal (x, y, z)     texture coordinates (s, t)

    // ground plane
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  4.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  0.0f, 4.0f,

    // cube top
    -1.00f,  -0.50f,  1.00f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     1.00f,  -0.50f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     1.00f,  -0.50f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    -1.00f,  -0.50f,  1.00f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     1.00f,  -0.50f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    -1.00f,  -0.50f, -1.00f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    -1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    -1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    -1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    -1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     1.00f,  -0.50f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     1.00f,  -0.50f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -1.00f,  -0.50f,  1.00f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    // cube back
     1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    -1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    -1.00f,  -0.50f, -1.00f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    -1.00f,  -0.50f, -1.00f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     1.00f,  -0.50f, -1.00f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube right
     1.00f, -1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f, -1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     1.00f,  -0.50f, -1.00f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     1.00f, -1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f,  -0.50f, -1.00f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     1.00f,  -0.50f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube left
    -1.00f, -1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -1.00f, -1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -1.00f,  -0.50f,  1.00f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f, -1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -1.00f,  -0.50f,  1.00f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f,  -0.50f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube top
    Xmin,  LegHeightTop,  Zmin,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax,  LegHeightTop,  Zmin,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     Xmax,  LegHeightTop, Zmax,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin,  LegHeightTop,  Zmin,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax,  LegHeightTop, Zmax,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin,  LegHeightTop, Zmax,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    Xmin, LegHeightBot, Zmax,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     Xmax, LegHeightBot, Zmax,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     Xmax, LegHeightBot,  Zmin,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin, LegHeightBot, Zmax,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    Xmax, LegHeightBot,  Zmin,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin, LegHeightBot,  Zmin,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    Xmin, LegHeightBot,  Zmin,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     Xmax, LegHeightBot,  Zmin,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     Xmax,  LegHeightTop,  Zmin,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin, LegHeightBot,  Zmin,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    Xmax,  LegHeightTop,  Zmin,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin,  LegHeightTop,  Zmin,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    // cube back
     Xmax, LegHeightBot, Zmax,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     Xmin, LegHeightBot, Zmax,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     Xmin,  LegHeightTop, Zmax,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax, LegHeightBot, Zmax,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    Xmin,  LegHeightTop, Zmax,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax,  LegHeightTop, Zmax,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube right
    Xmax, LegHeightBot,  Zmin,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax, LegHeightBot, Zmax,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    Xmax,  LegHeightTop, Zmax,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax, LegHeightBot,  Zmin,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax,  LegHeightTop, Zmax,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax,  LegHeightTop,  Zmin,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube left
   Xmin, LegHeightBot, Zmax, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin, LegHeightBot, Zmin, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   Xmin,  LegHeightTop, Zmin, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin, LegHeightBot, Zmax, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin,  LegHeightTop,  Zmin, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin,  LegHeightTop, Zmax, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

   // cube top
    Xmin+1.75f,  LegHeightTop,  Zmin,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+1.75f,  LegHeightTop,  Zmin,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     Xmax+1.75f,  LegHeightTop, Zmax,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+1.75f,  LegHeightTop,  Zmin,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+1.75f,  LegHeightTop, Zmax,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+1.75f,  LegHeightTop, Zmax,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    Xmin+1.75f, LegHeightBot, Zmax,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+1.75f, LegHeightBot, Zmax,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     Xmax+1.75f, LegHeightBot,  Zmin,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+1.75f, LegHeightBot, Zmax,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    Xmax+1.75f, LegHeightBot,  Zmin,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+1.75f, LegHeightBot,  Zmin,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    Xmin+1.75f, LegHeightBot,  Zmin,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     Xmax+1.75f, LegHeightBot,  Zmin,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     Xmax+1.75f,  LegHeightTop,  Zmin,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+1.75f, LegHeightBot,  Zmin,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    Xmax+1.75f,  LegHeightTop,  Zmin,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+1.75f,  LegHeightTop,  Zmin,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    // cube back
     Xmax+1.75f, LegHeightBot, Zmax,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     Xmin+1.75f, LegHeightBot, Zmax,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     Xmin+1.75f,  LegHeightTop, Zmax,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax+1.75f, LegHeightBot, Zmax,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    Xmin+1.75f,  LegHeightTop, Zmax,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax+1.75f,  LegHeightTop, Zmax,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube right
    Xmax+1.75f, LegHeightBot,  Zmin,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax+1.75f, LegHeightBot, Zmax,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    Xmax+1.75f,  LegHeightTop, Zmax,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax+1.75f, LegHeightBot,  Zmin,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax+1.75f,  LegHeightTop, Zmax,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax+1.75f,  LegHeightTop,  Zmin,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube left
   Xmin+1.75f, LegHeightBot, Zmax, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+1.75f, LegHeightBot, Zmin, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   Xmin+1.75f,  LegHeightTop, Zmin, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+1.75f, LegHeightBot, Zmax, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+1.75f,  LegHeightTop,  Zmin, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+1.75f,  LegHeightTop, Zmax, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

   // cube top
    Xmin,  LegHeightTop,  Zmin-1.75f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax,  LegHeightTop,  Zmin-1.75f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     Xmax,  LegHeightTop, Zmax-1.75f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin,  LegHeightTop,  Zmin-1.75f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax,  LegHeightTop, Zmax-1.75f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin,  LegHeightTop, Zmax-1.75f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    Xmin, LegHeightBot, Zmax-1.75f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     Xmax, LegHeightBot, Zmax-1.75f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     Xmax, LegHeightBot,  Zmin-1.75f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin, LegHeightBot, Zmax-1.75f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    Xmax, LegHeightBot,  Zmin-1.75f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin, LegHeightBot,  Zmin-1.75f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    Xmin, LegHeightBot,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     Xmax, LegHeightBot,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     Xmax,  LegHeightTop,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin, LegHeightBot,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    Xmax,  LegHeightTop,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin,  LegHeightTop,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    // cube back
     Xmax, LegHeightBot, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     Xmin, LegHeightBot, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     Xmin,  LegHeightTop, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax, LegHeightBot, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    Xmin,  LegHeightTop, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax,  LegHeightTop, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube right
    Xmax, LegHeightBot,  Zmin-1.75f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax, LegHeightBot, Zmax-1.75f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    Xmax,  LegHeightTop, Zmax-1.75f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax, LegHeightBot,  Zmin-1.75f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax,  LegHeightTop, Zmax-1.75f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax,  LegHeightTop,  Zmin-1.75f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube left
   Xmin, LegHeightBot, Zmax-1.75f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin, LegHeightBot, Zmin-1.75f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   Xmin,  LegHeightTop, Zmin-1.75f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin, LegHeightBot, Zmax-1.75f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin,  LegHeightTop,  Zmin-1.75f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin,  LegHeightTop, Zmax-1.75f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

   // cube top
    Xmin+1.75f,  LegHeightTop,  Zmin-1.75f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+1.75f,  LegHeightTop,  Zmin-1.75f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     Xmax+1.75f,  LegHeightTop, Zmax-1.75f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+1.75f,  LegHeightTop,  Zmin-1.75f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+1.75f,  LegHeightTop, Zmax-1.75f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+1.75f,  LegHeightTop, Zmax-1.75f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    Xmin+1.75f, LegHeightBot, Zmax-1.75f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+1.75f, LegHeightBot, Zmax-1.75f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     Xmax+1.75f, LegHeightBot,  Zmin-1.75f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+1.75f, LegHeightBot, Zmax-1.75f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    Xmax+1.75f, LegHeightBot,  Zmin-1.75f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+1.75f, LegHeightBot,  Zmin-1.75f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    Xmin+1.75f, LegHeightBot,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     Xmax+1.75f, LegHeightBot,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     Xmax+1.75f,  LegHeightTop,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+1.75f, LegHeightBot,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    Xmax+1.75f,  LegHeightTop,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+1.75f,  LegHeightTop,  Zmin-1.75f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    // cube back
     Xmax+1.75f, LegHeightBot, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     Xmin+1.75f, LegHeightBot, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     Xmin+1.75f,  LegHeightTop, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax+1.75f, LegHeightBot, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    Xmin+1.75f,  LegHeightTop, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax+1.75f,  LegHeightTop, Zmax-1.75f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube right
    Xmax+1.75f, LegHeightBot,  Zmin-1.75f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax+1.75f, LegHeightBot, Zmax-1.75f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    Xmax+1.75f,  LegHeightTop, Zmax-1.75f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax+1.75f, LegHeightBot,  Zmin-1.75f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax+1.75f,  LegHeightTop, Zmax-1.75f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax+1.75f,  LegHeightTop,  Zmin-1.75f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube left
   Xmin+1.75f, LegHeightBot, Zmax-1.75f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+1.75f, LegHeightBot, Zmin-1.75f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   Xmin+1.75f,  LegHeightTop, Zmin-1.75f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+1.75f, LegHeightBot, Zmax-1.75f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+1.75f,  LegHeightTop,  Zmin-1.75f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+1.75f,  LegHeightTop, Zmax-1.75f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

//CHAIR
   // cube top
    Xmin+3.00f,  LegHeightTop,  Zmin-0.75f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.50f,  LegHeightTop,  Zmin-0.75f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     Xmax+3.50f,  LegHeightTop, Zmax-1.25f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f,  LegHeightTop,  Zmin-0.75f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.50f,  LegHeightTop, Zmax-1.25f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f,  LegHeightTop, Zmax-1.25f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    Xmin+3.00f, LegHeightBot+0.75f, Zmax-1.25f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.50f, LegHeightBot+0.75f, Zmax-1.25f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     Xmax+3.50f, LegHeightBot+0.75f,  Zmin-0.75f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f, LegHeightBot+0.75f, Zmax-1.25f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    Xmax+3.50f, LegHeightBot+0.75f,  Zmin-0.75f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f, LegHeightBot+0.75f,  Zmin-0.75f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    Xmin+3.00f, LegHeightBot+0.75f,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     Xmax+3.50f, LegHeightBot+0.75f,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     Xmax+3.50f,  LegHeightTop,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+3.00f, LegHeightBot+0.75f,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    Xmax+3.50f,  LegHeightTop,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+3.00f,  LegHeightTop,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    // cube back
     Xmax+3.50f, LegHeightBot+0.75f, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     Xmin+3.00f, LegHeightBot+0.75f, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     Xmin+3.00f,  LegHeightTop, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax+3.50f, LegHeightBot+0.75f, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    Xmin+3.00f,  LegHeightTop, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax+3.50f,  LegHeightTop, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube right
    Xmax+3.50f, LegHeightBot+0.75f,  Zmin-0.75f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax+3.50f, LegHeightBot+0.75f, Zmax-1.25f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    Xmax+3.50f,  LegHeightTop, Zmax-1.25f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax+3.50f, LegHeightBot+0.75f,  Zmin-0.75f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax+3.50f,  LegHeightTop, Zmax-1.25f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax+3.50f,  LegHeightTop,  Zmin-0.75f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube left
   Xmin+3.00f, LegHeightBot+0.75f, Zmax-1.25f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+3.00f, LegHeightBot+0.75f, Zmin-0.75f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   Xmin+3.00f,  LegHeightTop, Zmin-0.75f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+3.00f, LegHeightBot+0.75f, Zmax-1.25f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+3.00f,  LegHeightTop,  Zmin-0.75f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+3.00f,  LegHeightTop, Zmax-1.25f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//
   // cube top
    Xmin+3.00f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.00f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     Xmax+3.00f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.00f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    Xmin+3.00f, LegHeightBot, Zmax-1.25f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.00f, LegHeightBot, Zmax-1.25f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     Xmax+3.00f, LegHeightBot,  Zmin-1.25f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f, LegHeightBot, Zmax-1.25f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    Xmax+3.00f, LegHeightBot,  Zmin-1.25f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f, LegHeightBot,  Zmin-1.25f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    Xmin+3.00f, LegHeightBot,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     Xmax+3.00f, LegHeightBot,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     Xmax+3.00f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+3.00f, LegHeightBot,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    Xmax+3.00f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+3.00f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    // cube back
     Xmax+3.00f, LegHeightBot, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     Xmin+3.00f, LegHeightBot, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     Xmin+3.00f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax+3.00f, LegHeightBot, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    Xmin+3.00f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax+3.00f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube right
    Xmax+3.00f, LegHeightBot,  Zmin-1.25f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax+3.00f, LegHeightBot, Zmax-1.25f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    Xmax+3.00f,  LegHeightTop-0.25f, Zmax-1.25f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax+3.00f, LegHeightBot,  Zmin-1.25f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax+3.00f,  LegHeightTop-0.25f, Zmax-1.25f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax+3.00f,  LegHeightTop-0.25f,  Zmin-1.25f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube left
   Xmin+3.00f, LegHeightBot, Zmax-1.25f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+3.00f, LegHeightBot, Zmin-1.25f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   Xmin+3.00f,  LegHeightTop-0.25f, Zmin-1.25f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+3.00f, LegHeightBot, Zmax-1.25f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+3.00f,  LegHeightTop-0.25f,  Zmin-1.25f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+3.00f,  LegHeightTop-0.25f, Zmax-1.25f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//
    // cube top
    Xmin+3.50f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
      Xmax+3.50f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
      Xmax+3.50f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.50f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
      Xmax+3.50f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.50f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    Xmin+3.50f, LegHeightBot, Zmax-1.25f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
      Xmax+3.50f, LegHeightBot, Zmax-1.25f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
      Xmax+3.50f, LegHeightBot,  Zmin-1.25f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.50f, LegHeightBot, Zmax-1.25f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.50f, LegHeightBot,  Zmin-1.25f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.50f, LegHeightBot,  Zmin-1.25f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    Xmin+3.50f, LegHeightBot,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
      Xmax+3.50f, LegHeightBot,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
      Xmax+3.50f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+3.50f, LegHeightBot,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     Xmax+3.50f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+3.50f,  LegHeightTop-0.25f,  Zmin-1.25f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    // cube back
      Xmax+3.50f, LegHeightBot, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     Xmin+3.50f, LegHeightBot, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     Xmin+3.50f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     Xmax+3.50f, LegHeightBot, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    Xmin+3.50f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     Xmax+3.50f,  LegHeightTop-0.25f, Zmax-1.25f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube right
     Xmax+3.50f, LegHeightBot,  Zmin-1.25f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.50f, LegHeightBot, Zmax-1.25f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     Xmax+3.50f,  LegHeightTop-0.25f, Zmax-1.25f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     Xmax+3.50f, LegHeightBot,  Zmin-1.25f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.50f,  LegHeightTop-0.25f, Zmax-1.25f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     Xmax+3.50f,  LegHeightTop-0.25f,  Zmin-1.25f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube left
   Xmin+3.50f, LegHeightBot, Zmax-1.25f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+3.50f, LegHeightBot, Zmin-1.25f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   Xmin+3.50f,  LegHeightTop-0.25f, Zmin-1.25f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+3.50f, LegHeightBot, Zmax-1.25f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+3.50f,  LegHeightTop-0.25f,  Zmin-1.25f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+3.50f,  LegHeightTop-0.25f, Zmax-1.25f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   //
   // cube top
    Xmin+3.50f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
      Xmax+3.50f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
      Xmax+3.50f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.50f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
      Xmax+3.50f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.50f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    Xmin+3.50f, LegHeightBot, Zmax-0.75f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
      Xmax+3.50f, LegHeightBot, Zmax-0.75f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
      Xmax+3.50f, LegHeightBot,  Zmin-0.75f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.50f, LegHeightBot, Zmax-0.75f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.50f, LegHeightBot,  Zmin-0.75f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.50f, LegHeightBot,  Zmin-0.75f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    Xmin+3.50f, LegHeightBot,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
      Xmax+3.50f, LegHeightBot,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
      Xmax+3.50f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+3.50f, LegHeightBot,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     Xmax+3.50f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+3.50f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    // cube back
      Xmax+3.50f, LegHeightBot, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     Xmin+3.50f, LegHeightBot, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     Xmin+3.50f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     Xmax+3.50f, LegHeightBot, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    Xmin+3.50f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     Xmax+3.50f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube right
     Xmax+3.50f, LegHeightBot,  Zmin-0.75f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.50f, LegHeightBot, Zmax-0.75f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     Xmax+3.50f,  LegHeightTop-0.25f, Zmax-0.75f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     Xmax+3.50f, LegHeightBot,  Zmin-0.75f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.50f,  LegHeightTop-0.25f, Zmax-0.75f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     Xmax+3.50f,  LegHeightTop-0.25f,  Zmin-0.75f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube left
   Xmin+3.50f, LegHeightBot, Zmax-0.75f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+3.50f, LegHeightBot, Zmin-0.75f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   Xmin+3.50f,  LegHeightTop-0.25f, Zmin-0.75f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+3.50f, LegHeightBot, Zmax-0.75f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+3.50f,  LegHeightTop-0.25f,  Zmin-0.75f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+3.50f,  LegHeightTop-0.25f, Zmax-0.75f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   //
   // cube top
    Xmin+3.00f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.00f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     Xmax+3.00f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.00f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    Xmin+3.00f, LegHeightBot, Zmax-0.75f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     Xmax+3.00f, LegHeightBot, Zmax-0.75f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     Xmax+3.00f, LegHeightBot,  Zmin-0.75f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f, LegHeightBot, Zmax-0.75f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    Xmax+3.00f, LegHeightBot,  Zmin-0.75f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    Xmin+3.00f, LegHeightBot,  Zmin-0.75f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    Xmin+3.00f, LegHeightBot,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     Xmax+3.00f, LegHeightBot,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     Xmax+3.00f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+3.00f, LegHeightBot,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    Xmax+3.00f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    Xmin+3.00f,  LegHeightTop-0.25f,  Zmin-0.75f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    // cube back
     Xmax+3.00f, LegHeightBot, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     Xmin+3.00f, LegHeightBot, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     Xmin+3.00f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax+3.00f, LegHeightBot, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    Xmin+3.00f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    Xmax+3.00f,  LegHeightTop-0.25f, Zmax-0.75f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube right
    Xmax+3.00f, LegHeightBot,  Zmin-0.75f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax+3.00f, LegHeightBot, Zmax-0.75f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    Xmax+3.00f,  LegHeightTop-0.25f, Zmax-0.75f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax+3.00f, LegHeightBot,  Zmin-0.75f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    Xmax+3.00f,  LegHeightTop-0.25f, Zmax-0.75f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    Xmax+3.00f,  LegHeightTop-0.25f,  Zmin-0.75f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube left
   Xmin+3.00f, LegHeightBot, Zmax-0.75f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+3.00f, LegHeightBot, Zmin-0.75f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   Xmin+3.00f,  LegHeightTop-0.25f, Zmin-0.75f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+3.00f, LegHeightBot, Zmax-0.75f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   Xmin+3.00f,  LegHeightTop-0.25f,  Zmin-0.75f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   Xmin+3.00f,  LegHeightTop-0.25f, Zmax-0.75f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
};

// OpenGL object IDs
GLuint vao;
GLuint vbo;
GLuint shader;
GLuint texture;

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

glm::vec3 lightPosition = glm::vec3(-5.0f, 3.0f, 5.0f);
double previousTime = 0.0;

///////////////////////////////////////////////////////////////////////////////
// SHADOW MAPPING CODE

#define SHADOW_SIZE 1024
GLuint shadowMapFbo;      // shadow map framebuffer object
GLuint shadowMapTexture;  // shadow map texture
GLuint shadowMapRbo;
GLuint shadowMapShader;   // shadow map shader

bool setupShadowMap()
{
    // create the FBO for rendering shadows
    glGenFramebuffers(1, &shadowMapFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFbo);

    // attach a texture object to the framebuffer
    glGenTextures(1, &shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);

    // check if we did everything right
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Could not create custom framebuffer.\n";
        return false;
    }

    // load the shader program for drawing the shadow map
    shadowMapShader = gdevLoadShader("finals_s.vs", "finals_s.fs");
    if (! shadowMapShader)
        return false;

    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

glm::mat4 renderShadowMap(glm::vec3 lightPos)
{
    // use the shadow framebuffer for drawing the shadow map
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFbo);

    // the viewport should be the size of the shadow map
    glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);

    // clear the shadow map
    // (we don't have a color buffer attachment, so no need to clear that)
    glClear(GL_DEPTH_BUFFER_BIT);

    // using the shadow map shader...
    glUseProgram(shadowMapShader);

    // ... set up the light space matrix...
    // (note that if you use a spot light, the FOV and the center position
    // vector should be set to your spotlight's outer radius and focus point,
    // respectively)
    glm::mat4 lightTransform;
    std::cout << lightPos.x << " : " << lightPos.x << " : " << lightPos.z << std::endl;
    lightTransform = glm::perspective(glm::radians(90.0f),       // fov
                                      1.0f,                      // aspect ratio
                                      0.1f,                      // near plane
                                      100.0f);                   // far plane
    lightTransform *= glm::lookAt(lightPos,                 // eye position
                                  glm::vec3(0.0f, 0.0f, 0.0f),   // center position
                                  glm::vec3(0.0f, 1.0f, 0.0f));  // up vector
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "lightTransform"),
                       1, GL_FALSE, glm::value_ptr(lightTransform));

    // ... set up the model matrix... (just identity for this demo)
    glm::mat4 modelTransform = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (8 * sizeof(float)));

    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // before drawing the final scene, we need to set drawing to the whole window
    int width, height;
    glfwGetFramebufferSize(pWindow, &width, &height);
    glViewport(0, 0, width, height);

    // we will need the light transformation matrix again in the main rendering code
    return lightTransform;
}

// SHADOW MAPPING CODE
///////////////////////////////////////////////////////////////////////////////

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // load our shader program
    shader = gdevLoadShader("finals.vs", "finals.fs");
    if (! shader)
        return false;

    // load our texture
    texture = gdevLoadTexture("finals.png", GL_REPEAT, true, true);
    if (! texture)
        return false;

    // enable z-buffer depth testing and face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    ///////////////////////////////////////////////////////////////////////////
    // setup shadow rendering
    if (! setupShadowMap())
        return false;
    ///////////////////////////////////////////////////////////////////////////

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // find the elapsed time since the last frame
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(pWindow);

    glm::vec3 lightPosition = camera.Position + camera.Front * 1.0f;
    ///////////////////////////////////////////////////////////////////////////
    // draw the shadow map
    glm::mat4 lightTransform = renderShadowMap(lightPosition);
    ///////////////////////////////////////////////////////////////////////////

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
    viewTransform = camera.GetViewMatrix();  // up vector
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewTransform"),
                       1, GL_FALSE, glm::value_ptr(viewTransform));

    // ... set up the model matrix... (just identity for this demo)
    glm::mat4 modelTransform = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set up the light position...
    glUniform3fv(glGetUniformLocation(shader, "lightPosition"),
                 1, glm::value_ptr(lightPosition));

    ///////////////////////////////////////////////////////////////////////////
    // ... set up the light transformation (for looking up the shadow map)...
    glUniformMatrix4fv(glGetUniformLocation(shader, "lightTransform"),
                       1, GL_FALSE, glm::value_ptr(lightTransform));

    // ... set the active texture...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"),  1);
    ///////////////////////////////////////////////////////////////////////////

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (8 * sizeof(float)));
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
    glfwSetCursorPosCallback(pWindow, mouse_callback);
    glfwSetScrollCallback(pWindow, scroll_callback);

    // Mouse Input
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // initialize GLAD, which acts as a library loader for the current OS's native OpenGL library
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // if our initial setup is successful...
    if (setup())
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
