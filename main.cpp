#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(std::string path);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 5.0f, 40.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(15.0f, 15.0f, -15.0f);


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shader("shaders/6.1.cubemaps.vs", "shaders/6.1.cubemaps.fs");
    Shader skyboxShader("shaders/6.1.skybox.vs", "shaders/6.1.skybox.fs");
    Shader groundShader("shaders/6.1.cubemaps.vs", "shaders/6.1.cubemaps.fs");
    Shader fortShader("shaders/6.1.cubemaps.vs", "shaders/6.1.cubemaps.fs");

    // build and compile our shader zprogram
    // ------------------------------------

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions          // texture Coords
        // belakang
        -5.0f, -1.0f, -5.0f,      0.0f,   0.0f,
        5.0f, -1.0f, -5.0f,      20.0f,  0.0f,
        0.0f,   5.0f,  0.0f,       10.0f,  20.0f,


        // depan
       -5.0f, -1.0f,  5.0f,      0.0f,   0.0f,
        5.0f, -1.0f,  5.0f,      20.0f,  0.0f,
        0.0f,   5.0f,  0.0f,       10.0f,  20.0f,

        // kiri
      -5.0f, -1.0f, -5.0f,       20.0f,  0.0f,
      -5.0f, -1.0f,  5.0f,       0.0f,   0.0f,
       0.0f,   5.0f,  0.0f,        10.0f,  20.0f,

        // kanan
       5.0f, -1.0f, -5.0f,       20.0f,  0.0f,
       5.0f, -1.0f,  5.0f,       0.0f,   0.0f,
       0.0f,  5.0f,  0.0f,         10.0f,  20.0f,

        // bawah
      -5.0f, -1.0f, -5.0f,       0.0f,   20.0f,
       5.0f, -1.0f, -5.0f,       20.0f,   20.0f,
       5.0f, -1.0f,  5.0f,       20.0f,   0.0f,
       5.0f, -1.0f,  5.0f,       20.0f,   0.0f,
      -5.0f, -1.0f,  5.0f,       0.0f,   0.0f,
      -5.0f, -1.0f, -5.0f,       0.0f,   20.0f,

        // belakang
        -13.0f, -1.0f, 4.0f,      0.0f,   0.0f,
        -7.0f, -1.0f, 4.0f,      20.0f,  0.0f,
        -10.0f,   3.0f,  7.0f,       10.0f,  20.0f,


        // depan
       -13.0f, -1.0f,  10.0f,      0.0f,   0.0f,
        -7.0f, -1.0f,  10.0f,      20.0f,  0.0f,
        -10.0f,   3.0f,  7.0f,       10.0f,  20.0f,

        // kiri
      -13.0f, -1.0f, 4.0f,       20.0f,  0.0f,
      -13.0f, -1.0f,  10.0f,       0.0f,   0.0f,
       -10.0f,   3.0f,  7.0f,        10.0f,  20.0f,

        // kanan
       -7.0f, -1.0f, 4.0f,       20.0f,  0.0f,
       -7.0f, -1.0f,  10.0f,       0.0f,   0.0f,
       -10.0f,  3.0f,  7.0f,         10.0f,  20.0f,

        // bawah
      -13.0f, -1.0f, 4.0f,       0.0f,   20.0f,
       -7.0f, -1.0f, 4.0f,       20.0f,   20.0f,
       -7.0f, -1.0f,  10.0f,       20.0f,   0.0f,
       -7.0f, -1.0f,  10.0f,       20.0f,   0.0f,
      -13.0f, -1.0f,  10.0f,       0.0f,   0.0f,
      -13.0f, -1.0f, 4.0f,       0.0f,   20.0f,


       // belakang
        7.0f, -1.0f, -8.0f,      0.0f,   0.0f,
        13.0f, -1.0f, -8.0f,      20.0f,  0.0f,
        10.0f,   3.0f,  -5.0f,       10.0f,  20.0f,


        // depan
       7.0f, -1.0f,  -2.0f,      0.0f,   0.0f,
        13.0f, -1.0f,  -2.0f,      20.0f,  0.0f,
        10.0f,   3.0f,  -5.0f,       10.0f,  20.0f,

        // kiri
      7.0f, -1.0f, -8.0f,       20.0f,  0.0f,
      7.0f, -1.0f,  -2.0f,       0.0f,   0.0f,
       10.0f,   3.0f,  -5.0f,        10.0f,  20.0f,

        // kanan
       13.0f, -1.0f, -8.0f,       20.0f,  0.0f,
       13.0f, -1.0f,  -2.0f,       0.0f,   0.0f,
       10.0f,  3.0f,  -5.0f,         10.0f,  20.0f,

        // bawah
      7.0f, -1.0f, -8.0f,       0.0f,   20.0f,
       13.0f, -1.0f, -8.0f,       20.0f,   20.0f,
       13.0f, -1.0f,  -2.0f,       20.0f,   0.0f,
       13.0f, -1.0f,  -2.0f,       20.0f,   0.0f,
      7.0f, -1.0f,  -2.0f,       0.0f,   0.0f,
      7.0f, -1.0f, -8.0f,       0.0f,   20.0f,


    };

    // vertex untuk skybox
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // vertex untuk terrain
    float groundVertices[] = {

        // ground
        -100.0f, -1.0f, -100.0f,  0.0f, 1.0f,
         100.0f, -1.0f, -100.0f,  1.0f, 1.0f,
         100.0f, -1.0f,  100.0f,  1.0f, 0.0f,
         100.0f, -1.0f,  100.0f,  1.0f, 0.0f,
        -100.0f, -1.0f,  100.0f,  0.0f, 0.0f,
        -100.0f, -1.0f, -100.0f,  0.0f, 1.0f,

    };

    // vertex untuk benteng
    float fortVertices[] = {

        // ====== FRONT =========
        //front-right
        //sudut kiri bwah
        2.0f, -1.0f, 14.0f, 0.0f, 0.0f,
        // sudut kiri atas
        2.0f, 0.0f, 14.0f, 0.0f, 1.0f,
        //sudut kanan atas
        2.0f, 0.0f, 14.5f, 1.0f, 1.0f,

        //sudut kiri bawah
        2.0f, -1.0f, 14.0f, 0.0f, 0.0f,
        //sudut kanan atas
        2.0f, 0.0f, 14.5f, 1.0f, 1.0f,
        //sudut kanan bawah
        2.0f, -1.0f, 14.5f, 1.0f, 0.0f,

        2.0f, -1.0f, 14.5f, 0.0f, 0.0f,
        2.0f, 0.0f, 14.5f, 0.0f, 1.0f,
        14.0f, 0.0f, 14.5f, 1.0f, 1.0f,
        14.0f, 0.0f, 14.5f, 1.0f, 1.0f,
        14.0f, -1.0f, 14.5f, 1.0f, 0.0f,
        2.0f, -1.0f, 14.5f, 0.0f, 0.0f,

        2.0f, -1.0f, 14.0f, 0.0f, 0.0f,
        2.0f, 0.0f, 14.0f, 0.0f, 1.0f,
        14.0f, 0.0f, 14.0f, 1.0f, 1.0f,
        14.0f, 0.0f, 14.0f, 1.0f, 1.0f,
        14.0f, -1.0f, 14.0f, 1.0f, 0.0f,
        2.0f, -1.0f, 14.0f, 0.0f, 0.0f,

        14.0f, -1.0f, 14.0f, 0.0f, 1.0f,
        14.0f, 0.0f, 14.0f, 1.0f, 1.0f,
        14.0f, 0.0f, 14.5f, 1.0f, 0.0f,
        14.0f, -1.0f, 14.0f, 0.0f, 1.0f,
        14.0f, 0.0f, 14.5f, 0.0f, 0.0f,
        14.0f, -1.0f, 14.5f, 1.0f, 0.0f,

        14.0f, 0.0f, 14.0f, 0.0f, 1.0f,
        14.0f, 0.0f, 14.5f, 1.0f, 1.0f,
        2.0f, 0.0f, 14.0f, 0.0f, 0.0f,
        2.0f, 0.0f, 14.0f, 0.0f, 0.0f,
        14.0f, 0.0f, 14.5f, 1.0f, 1.0f,
        2.0f, 0.0f, 14.5f, 1.0f, 0.0f,

        // front-left

        //sudut kiri bwah
        -2.0f, -1.0f, 14.0f, 0.0f, 0.0f,
        // sudut kiri atas
        -2.0f, 0.0f, 14.0f, 0.0f, 1.0f,
        //sudut kanan atas
        -2.0f, 0.0f, 14.5f, 1.0f, 1.0f,

        //sudut kiri bawah
        -2.0f, -1.0f, 14.0f, 0.0f, 0.0f,
        //sudut kanan atas
        -2.0f, 0.0f, 14.5f, 1.0f, 1.0f,
        //sudut kanan bawah
        -2.0f, -1.0f, 14.5f, 1.0f, 0.0f,

        -2.0f, -1.0f, 14.5f, 0.0f, 0.0f,
        -2.0f, 0.0f, 14.5f, 0.0f, 1.0f,
        -14.0f, 0.0f, 14.5f, 1.0f, 1.0f,
        -14.0f, 0.0f, 14.5f, 1.0f, 1.0f,
        -14.0f, -1.0f, 14.5f, 1.0f, 0.0f,
        -2.0f, -1.0f, 14.5f, 0.0f, 0.0f,

        -2.0f, -1.0f, 14.0f, 0.0f, 0.0f,
        -2.0f, 0.0f, 14.0f, 0.0f, 1.0f,
        -14.0f, 0.0f, 14.0f, 1.0f, 1.0f,
        -14.0f, 0.0f, 14.0f, 1.0f, 1.0f,
        -14.0f, -1.0f, 14.0f, 1.0f, 0.0f,
        -2.0f, -1.0f, 14.0f, 0.0f, 0.0f,

        -14.0f, -1.0f, 14.0f, 0.0f, 1.0f,
        -14.0f, 0.0f, 14.0f, 1.0f, 1.0f,
        -14.0f, 0.0f, 14.5f, 1.0f, 0.0f,
        -14.0f, -1.0f, 14.0f, 0.0f, 1.0f,
        -14.0f, 0.0f, 14.5f, 0.0f, 0.0f,
        -14.0f, -1.0f, 14.5f, 1.0f, 0.0f,

        -14.0f, 0.0f, 14.0f, 0.0f, 1.0f,
        -14.0f, 0.0f, 14.5f, 1.0f, 1.0f,
        -2.0f, 0.0f, 14.0f, 0.0f, 0.0f,
        -2.0f, 0.0f, 14.0f, 0.0f, 0.0f,
        -14.0f, 0.0f, 14.5f, 1.0f, 1.0f,
        -2.0f, 0.0f, 14.5f, 1.0f, 0.0f,

        // ====== RIGHT =========
        14.0f, 0.0f, 14.0f, 0.0f, 1.0f,
        13.5f, 0.0f, 14.0f, 0.0f, 0.0f,
        14.0f, 0.0f, -14.0f, 1.0f, 1.0f,
        14.0f, 0.0f, -14.0f, 1.0f, 1.0f,
        13.5f, 0.0f, 14.0f, 0.0f, 0.0f,
        13.5f, 0.0f, -14.0f, 1.0f, 0.0f,

        13.5f, 0.0f, 14.0f, 0.0f, 1.0f,
        13.5f, -1.0f, 14.0f, 0.0f, 0.0f,
        13.5f, 0.0f, -14.0f, 1.0f, 1.0f,
        13.5f, -1.0f, 14.0f, 0.0f, 0.0f,
        13.5f, 0.0f, -14.0f, 1.0f, 1.0f,
        13.5f, -1.0f, -14.0f, 1.0f, 0.0f,

        14.0f, 0.0f, 14.0f, 0.0f, 1.0f,
        14.0f, -1.0f, 14.0f, 0.0f, 0.0f,
        14.0f, 0.0f, -14.0f, 1.0f, 1.0f,
        14.0f, -1.0f, 14.0f, 0.0f, 0.0f,
        14.0f, 0.0f, -14.0f, 1.0f, 1.0f,
        14.0f, -1.0f, -14.0f, 1.0f, 0.0f,

        // ====== LEFT =========
        -14.0f, 0.0f, 14.0f, 0.0f, 1.0f,
        -13.5f, 0.0f, 14.0f, 0.0f, 0.0f,
        -14.0f, 0.0f, -14.0f, 1.0f, 1.0f,
        -14.0f, 0.0f, -14.0f, 1.0f, 1.0f,
        -13.5f, 0.0f, 14.0f, 0.0f, 0.0f,
        -13.5f, 0.0f, -14.0f, 1.0f, 0.0f,

        -13.5f, 0.0f, 14.0f, 0.0f, 1.0f,
        -13.5f, -1.0f, 14.0f, 0.0f, 0.0f,
        -13.5f, 0.0f, -14.0f, 1.0f, 1.0f,
        -13.5f, -1.0f, 14.0f, 0.0f, 0.0f,
        -13.5f, 0.0f, -14.0f, 1.0f, 1.0f,
        -13.5f, -1.0f, -14.0f, 1.0f, 0.0f,

        -14.0f, 0.0f, 14.0f, 0.0f, 1.0f,
        -14.0f, -1.0f, 14.0f, 0.0f, 0.0f,
        -14.0f, 0.0f, -14.0f, 1.0f, 1.0f,
        -14.0f, -1.0f, 14.0f, 0.0f, 0.0f,
        -14.0f, 0.0f, -14.0f, 1.0f, 1.0f,
        -14.0f, -1.0f, -14.0f, 1.0f, 0.0f,

        // ====== BACK =========
        //sudut kiri bwah
        -14.0f, -1.0f, -14.0f, 0.0f, 0.0f,
        // sudut kiri atas
        -14.0f, 0.0f, -14.0f, 0.0f, 1.0f,
        //sudut kanan atas
        -14.0f, 0.0f, -14.5f, 1.0f, 1.0f,

        //sudut kiri bawah
        -14.0f, -1.0f, -14.0f, 0.0f, 0.0f,
        //sudut kanan atas
        -14.0f, 0.0f, -14.5f, 1.0f, 1.0f,
        //sudut kanan bawah
        -14.0f, -1.0f, -14.5f, 1.0f, 0.0f,

        -14.0f, -1.0f, -14.5f, 0.0f, 0.0f,
        -14.0f, 0.0f, -14.5f, 0.0f, 1.0f,
        14.0f, 0.0f, -14.5f, 1.0f, 1.0f,
        14.0f, 0.0f, -14.5f, 1.0f, 1.0f,
        14.0f, -1.0f, -14.5f, 1.0f, 0.0f,
        -14.0f, -1.0f, -14.5f, 0.0f, 0.0f,

        -14.0f, -1.0f, -14.0f, 0.0f, 0.0f,
        -14.0f, 0.0f, -14.0f, 0.0f, 1.0f,
        14.0f, 0.0f, -14.0f, 1.0f, 1.0f,
        14.0f, 0.0f, -14.0f, 1.0f, 1.0f,
        14.0f, -1.0f, -14.0f, 1.0f, 0.0f,
        -14.0f, -1.0f, -14.0f, 0.0f, 0.0f,

        14.0f, -1.0f, -14.0f, 0.0f, 1.0f,
        14.0f, 0.0f, -14.0f, 1.0f, 1.0f,
        14.0f, 0.0f, -14.5f, 1.0f, 0.0f,
        14.0f, -1.0f, -14.0f, 0.0f, 1.0f,
        14.0f, 0.0f, -14.5f, 0.0f, 0.0f,
        14.0f, -1.0f, -14.5f, 1.0f, 0.0f,

        14.0f, 0.0f, -14.0f, 0.0f, 1.0f,
        14.0f, 0.0f, -14.5f, 1.0f, 1.0f,
        -14.0f, 0.0f, -14.0f, 0.0f, 0.0f,
        -14.0f, 0.0f, -14.0f, 0.0f, 0.0f,
        14.0f, 0.0f, -14.5f, 1.0f, 1.0f,
        -14.0f, 0.0f, -14.5f, 1.0f, 0.0f,
    };

    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // fort VAO
    unsigned int fortVAO, fortVBO;
    glGenVertexArrays(1, &fortVAO);
    glGenBuffers(1, &fortVBO);
    glBindVertexArray(fortVAO);
    glBindBuffer(GL_ARRAY_BUFFER, fortVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fortVertices), &fortVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // ground VAO
    unsigned int groundVAO, groundVBO;
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), &groundVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    unsigned int cubeTexture = loadTexture("resources/textures/texturepyramid.jpeg");
    unsigned int groundTexture = loadTexture("resources/textures/sand.jpg");
    unsigned int fortTexture = loadTexture("resources/textures/wall2.jpg");
    vector<std::string> faces
    {
        "resources/textures/skybox/right.jpg",
        "resources/textures/skybox/left.jpg",
        "resources/textures/skybox/top.jpg",
        "resources/textures/skybox/bottom.jpg",
        "resources/textures/skybox/front.jpg",
        "resources/textures/skybox/back.jpg"

    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    groundShader.use();
    groundShader.setInt("texture2", 0);

    fortShader.use();
    fortShader.setInt("texture3", 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw scene as normal
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // render piramid
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        glDrawArrays(GL_TRIANGLES, 0, 162);
        glBindVertexArray(0);

        // render ground
        groundShader.use();

        groundShader.setMat4("model", model);
        groundShader.setMat4("view", view);
        groundShader.setMat4("projection", projection);

        glBindVertexArray(groundVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(1);

        // render wall
        fortShader.use();

        fortShader.setMat4("model", model);
        fortShader.setMat4("view", view);
        fortShader.setMat4("projection", projection);

        glBindVertexArray(fortVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fortTexture);
        glDrawArrays(GL_TRIANGLES, 0, 180);
        glBindVertexArray(1);


        // menggambar skybox
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVBO);

    glfwTerminate();
    return 0;
}

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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
    camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------

unsigned int loadTexture(std::string path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        //unsigned char *data = stbi_load("resources/textures/texturepyramid.jpeg", &width, &height, &nrChannels, 0);
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
