#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
#include "camera.h"

//--------------------------------------------------------------------------------------------------
// Callback functions
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

//--------------------------------------------------------------------------------------------------
// window Settings
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1100;

// Camera Settings
Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing Settings
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//--------------------------------------------------------------------------------------------------
int main()
{
    //--------------------------------------------------------------------------------------------------
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //--------------------------------------------------------------------------------------------------
    // Create a GLFW window
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Arnav's World", NULL, NULL);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse and make invisible

    //--------------------------------------------------------------------------------------------------
    // Load OpenGL function pointers with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering

    //--------------------------------------------------------------------------------------------------
    // building and compiling our shaders
    Shader ourCube("3.3.shader.vert", "3.3.shader.frag");
    Shader ourLight("1.light_cube.vert", "1.light_cube.frag");

    //--------------------------------------------------------------------------------------------------
    // Vertex data for a cube
    float my_vertices[] = {
        // VERTICES             // NORMAL          // S,T TEXTURE
        // Back face (z = -0.5, normal = (0, 0, -1))
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // Bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // Top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // Top-right
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // Top-left

        // Front face (z = 0.5, normal = (0, 0, 1))
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // Bottom-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // Top-right
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // Top-right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // Top-left

        // Left face (x = -0.5, normal = (-1, 0, 0))
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom-front
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-back
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top-back
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom-front
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top-back
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // Top-front

        // Right face (x = 0.5, normal = (1, 0, 0))
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-back
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-front
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // Top-front
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-back
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // Top-front
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-back

        // Bottom face (y = -0.5, normal = (0, -1, 0))
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Back-left
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // Back-right
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // Front-right
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Back-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // Front-right
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // Front-left

        // Top face (y = 0.5, normal = (0, 1, 0))
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Back-left
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // Back-right
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // Front-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Back-left
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // Front-right
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // Front-left
    };

    //--------------------------------------------------------------------------------------------------
    // VAOs VBOs- Our Cube
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); // lets bind vao first then vbo

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(my_vertices), my_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0); // Position attribute
    glEnableVertexAttribArray(0);                                                  // ACTIVATE position

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))); // Normal attribute
    glEnableVertexAttribArray(1);                                                                    // ACTIVATE normal

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))); // Texture attribute
    glEnableVertexAttribArray(2);                                                                    // ACTIVATE texture

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //--------------------------------------------------------------------------------------------------
    // Light VAO (Same VBO Data) - Light Source 1
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //--------------------------------------------------------------------------------------------------
    // ----------------------Adding texture
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate our data
    int width, height, nrChannels;
    // Flip the image so OpenGL renders it correctly
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load("C:/Users/sinha/Desktop/Stryker Internship/Computer Graphics/textures/wooden_crate.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE" << std::endl;
    }
    stbi_image_free(data);

    // Activate shader before setting uniforms-> IMP!!!!!!!
    ourCube.use();
    glUniform1i(glGetUniformLocation(ourCube.ID, "material.diffuse"), 0); // setting uniforms

    // ---------------------- Adding Specular Texture
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Flip the image so OpenGL renders it correctly
    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("C:/Users/sinha/Desktop/Stryker Internship/Computer Graphics/textures/specular_map3_wooden_crate.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE" << std::endl;
    }
    stbi_image_free(data);

    // Activate shader before setting uniforms-> IMP!!!!!!!
    ourCube.use();
    glUniform1i(glGetUniformLocation(ourCube.ID, "material.specular"), 1); // setting uniforms

    //--------------------------------------------------------------------------------------------------
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        //--------------------------------------------------------------------------------------------------
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //--------------------------------------------------------------------------------------------------
        // Input handling
        float timeValue = glfwGetTime();
        float rBackground = sin(timeValue * 0.10);
        float gBackground = 0.50f;
        float bBackground = cos(timeValue * 0.10);
        processInput(window);
        glClearColor(0.0325, 0.0325, 0.0325, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ;

        //--------------------------------------------------------------------------------------------------
        // Setting properties through uniforms

        ourCube.use();
        glUniform3f(glGetUniformLocation(ourCube.ID, "light.position"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(ourCube.ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z); // camera position

        // texture activate
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0); // activate texture unit --> GL_TEXTURE0 to GL_TEXTURE15 16 Textures
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // light properties
        glm::vec3 lightColor;
        // lightColor.x = static_cast<float>(sin(timeValue * 2.0f)); // -> Changing light color
        lightColor.x = static_cast<float>(sin(timeValue * 2.0f));
        lightColor.y = static_cast<float>(sin(timeValue * 1.25f));
        lightColor.z = static_cast<float>(sin(timeValue * 0.84f));

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);   // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

        glUniform3f(glGetUniformLocation(ourCube.ID, "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
        glUniform3f(glGetUniformLocation(ourCube.ID, "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
        glUniform3f(glGetUniformLocation(ourCube.ID, "light.specular"), 1.0f, 1.0f, 1.0f);

        // sending light source color to light's fragment shader
        ourLight.use();
        glUniform3f(glGetUniformLocation(ourLight.ID, "lightSourceColor"), lightColor.x, lightColor.y, lightColor.z);

        //--------------------------------------------------------------------------------------------------
        // 3D Cube Object
        glm::mat4 model = glm::mat4(1.0f); // intitalizing to identity matrix
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        ourCube.use();
        int model_location = glGetUniformLocation(ourCube.ID, "model"); // sending these to shaders via uniform
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

        int view_location = glGetUniformLocation(ourCube.ID, "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

        int projection_location = glGetUniformLocation(ourCube.ID, "projection");
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

        // rendering the cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0); // Unbind after use (optional but good practice

        //--------------------------------------------------------------------------------------------------
        // Light Source Object

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        ourLight.use();
        model_location = glGetUniformLocation(ourLight.ID, "model");
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
        view_location = glGetUniformLocation(ourLight.ID, "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
        projection_location = glGetUniformLocation(ourLight.ID, "projection");
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

        // rendering light source
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        //--------------------------------------------------------------------------------------------------
        glfwSwapBuffers(window); // Swap buffers and poll IO events
        glfwPollEvents();
    }

    //--------------------------------------------------------------------------------------------------
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate(); // Cleanup and exit
    return 0;
    //--------------------------------------------------------------------------------------------------
}

//--------------------------------------------------------------------------------------------------
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
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

//--------------------------------------------------------------------------------------------------
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//--------------------------------------------------------------------------------------------------
// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
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

//--------------------------------------------------------------------------------------------------
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

//--------------------------------------------------------------------------------------------------
