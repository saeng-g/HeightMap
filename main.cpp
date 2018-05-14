////////////////////////////////////////////////////////////////////
/////////////////////////// ASSIGNMENT 1 ///////////////////////////
////////////////////////////////////////////////////////////////////

/*
 
 COMP 371 - Assignment #1
 Professor Cha
 Kyungjin Kim (ID: 40066802)
 due date: Monday May 14th, 2018 (5:00PM)
 
 basic code taken from Lab 1
 modified from http://learnopengl.com/
 
 Other references used (used to study the APIs, compare my code, and understanding VAO, VBO, EBO, and mesh creation. Note: no code was copied or used except the snippet of code used to identify project directory, does not have any impact on the project itself I just really struggled with locating the directory!)
 https://open.gl/
 https://stackoverflow.com/questions/39669186/trouble-with-rendering-a-heightmap-using-opengl-and-cimg
 http://www.learnopengles.com/android-lesson-eight-an-introduction-to-index-buffer-objects-ibos/
 
 Online Documentation for GLFW, OpenGL, GLM, CImg from:
 https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/
 http://www.glfw.org/docs/latest/input_guide.html#input_mouse_button
 http://glm.g-truc.net/0.9.8/api/modules.html
 http://cimg.eu/reference/structcimg__library_1_1CImg.html#details
 
 Heightmap greyscale images obtained from google images and http://run.usc.edu/cs420-s16/assignments/assign1/assign1.html
 
 */

// GLEW, GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// CImg
#define cimg_display 0
#define cimg_use_png 1 //accepts .png type
#define cimg_use_jpeg 1 //accepts .jpeg/.jpg type
#include "CImg.h"

//GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Other standard libraries
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace cimg_library;
using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

// Define some global variables
GLenum drawtype = GL_POINTS;
int a = 0;
// Model transformation matrices control variables
glm::mat4 transf;
glm::mat4 s_mMat = glm::scale(transf, glm::vec3(1.0f/800.0f));
glm::mat4 r_mMat = glm::rotate(transf, -1.7f, glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 t_mMat = glm::translate(transf, glm::vec3(-0.5f, -0.5f, -0.5f));
// View transformation matrices control variables
glm::vec3 pos = glm::vec3(0.0f, 0.0f, 10.0f); //camera positioned here
glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f); //looks at origin
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);






////////////////////////////////////////////////////////////////////
/////////////////////////// KEY CALLBACK ///////////////////////////
////////////////////////////////////////////////////////////////////

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    //Moving the Mesh
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        r_mMat = glm::rotate(transf, 0.1f, glm::vec3(0.0f, -1.0f, 0.0f));
        transf = r_mMat * transf;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        r_mMat = glm::rotate(transf, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
        transf = r_mMat * transf;
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        r_mMat = glm::rotate(transf, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
        transf = r_mMat * transf;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        r_mMat = glm::rotate(transf, 0.1f, glm::vec3(-1.0f, 0.0f, 0.0f));
        transf = r_mMat * transf;
    }
    
    //Changing the type
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        drawtype = GL_POINTS;
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        drawtype = GL_LINE_STRIP;
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        drawtype = GL_TRIANGLE_STRIP;
}

// Is called whenever user scrolls via GLFW
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    std::cout << yoffset << std::endl;
    float change = 1.1f;
    if (yoffset < 0) change = 0.9f;
    else change = 1.1f;
    s_mMat = glm::scale(transf, glm::vec3(change));
    transf = s_mMat * transf;
}







////////////////////////////////////////////////////////////////////
/////////////////////////// MAIN FUNCTION //////////////////////////
////////////////////////////////////////////////////////////////////

// The MAIN function, from here we start the application and run the game loop
int main()
{
    
    
    
    //added to figure out which directory to include the image files and shader files
    //ref. https://stackoverflow.com/questions/7279100/c-ifstream-on-xcode-where-is-the-default-directory
    std::cout << "Project Detail:" << std::endl;
    char * dir = getcwd(NULL, 0);
    printf("Current dir: %s\n\n", dir);
    delete [] dir;
    dir = nullptr;
    
    
    
    
    
    ////////////////////////////////////////////////////////////////////
    /////////////////////////// READING IMG ////////////////////////////
    ////////////////////////////////////////////////////////////////////
    
    
    // READING IMAGE FILE
    //Loading Greyscale Image
    CImg<unsigned char> map("hm.jpg");
    GLuint map_h = map.height();
    GLuint map_w = map.width();
    std::cout << "Map Height: " << map_h << std::endl;
    std::cout << "Map Width: " << map_w << std::endl;
    
    //extract coordinates
    int numCoordinates = map_h * map_w * 3;
    GLfloat* coordinates = new GLfloat[numCoordinates];
    for (int i = 0; i < map_h; i++)
    {
        for (int j = 0; j < map_w; j++)
        {
            int index = (i * map_w * 3) + (j * 3);
            coordinates[index + 0] = (GLfloat)i / map_h;
            coordinates[index + 1] = (GLfloat)j / map_w;
            coordinates[index + 2] = ((GLfloat)map(i,j,0)) / 255.0f;
        }
    }
    std::cout << "Number of coordinates: " << numCoordinates;
    std::cout << "Size of coordinates array in bytes: " << sizeof(GLfloat) * map_h * map_w * 3 << std::endl;
    
    
    
    
    ////////////////////////////////////////////////////////////////////
    ///////////////////////// INDICIES FOR EBO /////////////////////////
    ////////////////////////////////////////////////////////////////////
    
    //determine indices for mesh creation
    int numDegen = 2 * (map_h - 1);
    int indexSize = map_h*map_w*2 + numDegen;
    std::cout << "Numer of IndiceData: " << indexSize << std::endl;
    
    //indiceData created on heap
    //Note: was getting bad memory access error on stack, stack size might not be big enough?
    GLuint* indiceData = new GLuint[indexSize];
    int offset = 0;
    for (int i = 0; i < map_h; i++)
    {
        if (i > 0)
        {
            indiceData[offset] = (GLuint) i*map_w;
            offset++;
        }
        for (int j = 0; j < map_h; j++)
        {
            indiceData[offset] = (GLuint) i*map_w + j;
            offset++;
            indiceData[offset] = (GLuint) (i+1)*map_w + j;
            offset++;
        }
        if (i < map_h - 1)
        {
            indiceData[offset] = (GLuint) (i+1)*map_w + map_h - 1;
            offset++;
        }
    }
    
    
    
    
    ////////////////////////////////////////////////////////////////////
    ///////////////////////// GLFW WINDOW SETUP ////////////////////////
    ////////////////////////////////////////////////////////////////////
    
    // SET UP GLFW WINDOW
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //required for Mac. Ignored by Windows.

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triangle", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    
    
    ////////////////////////////////////////////////////////////////////
    //////////////////////// SETTING UP SHADERS ////////////////////////
    ////////////////////////////////////////////////////////////////////
    
    // Build and compile our shader program
    
    // Read the Vertex Shader code from the file
    string vertex_shader_path = "vertex.shader";
    string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_shader_path, ios::in);
    
    if (VertexShaderStream.is_open())
    {
        string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else
    {
        printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
        getchar();
        exit(-1);
    }
    
    // Read the Fragment Shader code from the file
    string fragment_shader_path = "fragment.shader";
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
    
    if (FragmentShaderStream.is_open())
    {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    else
    {
        printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
        getchar();
        exit(-1);
    }
    
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader); //free up memory
    glDeleteShader(fragmentShader);
    
    glUseProgram(shaderProgram);
    
    
    
    
    
    ////////////////////////////////////////////////////////////////////
    ////////////////////////// VAO, VBO, EBOs //////////////////////////
    ////////////////////////////////////////////////////////////////////
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * map_h * map_w * 3, coordinates, GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexSize, indiceData, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
    
    
    
    
    ////////////////////////////////////////////////////////////////////
    ///////////////////// MODEL, VIEW, PROJECTION //////////////////////
    ////////////////////////////////////////////////////////////////////
    // Obtain uniform location for the mvp matrices from shader
    GLuint m_Loc = glGetUniformLocation(shaderProgram, "m_mat");
    GLuint v_Loc = glGetUniformLocation(shaderProgram, "v_mat");
    GLuint p_Loc = glGetUniformLocation(shaderProgram, "p_mat");
    
    //apply initial transformation and declare
    glm::mat4 m_mat, v_mat, p_mat;
    transf = r_mMat * t_mMat * transf;
    
    //enable depth buffer to hide overlap
    glEnable(GL_DEPTH_TEST);
    
    
    
    ////////////////////////////////////////////////////////////////////
    //////////////////////////// GAME LOOP /////////////////////////////
    ////////////////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been  activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        // Apply necessary Transformations
        m_mat = transf * m_mat;
        transf = glm::mat4(); //reset transformation to identity to prevent reapplication of the transformation
        v_mat = glm::lookAt(pos, at, up);
        p_mat = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
        
        // Send MVP Info to Shaders
        glUniformMatrix4fv(m_Loc, 1, GL_FALSE, glm::value_ptr(m_mat));
        glUniformMatrix4fv(v_Loc, 1, GL_FALSE, glm::value_ptr(v_mat));
        glUniformMatrix4fv(p_Loc, 1, GL_FALSE, glm::value_ptr(p_mat));
        
        glBindVertexArray(VAO);
        glDrawElements(drawtype, indexSize - 1, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    delete [] coordinates; //free memory in heap
    coordinates = nullptr;
    delete [] indiceData; //free memory in heap
    indiceData = nullptr;
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}
