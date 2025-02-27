/*                                  Notes- Coordinate, Transformations, and Z-Buffer
* OpenGL stores all its depth information in a z-buffer or a depth buffer
* GLFW automatically creates such a buffer for you! (like it stores color of output image)
* depth is stored within each fragment (as fragments z value), whenever opengl wants to output its color, it checks z value
* if current frag is behind other frag, it is discarded else overwritten (automatic depth testing)
* We must enable depth testing, it is disabled by default!, we do using glEnable and glDisable function

*/


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float mixValue = 0.50f;
float scaleValue = 1.00f;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Arnav's Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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


    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("3.3.shader.vs", "3.3.shader.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // LET US RENDER A CUBE - 36 Vertices [6 Faces x 2 Triangles X 3 Vertices Each]
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Textures
    unsigned int texture1, texture2;

    // <-------------------------------------TEXTURE 1----------------------------------->
    glGenTextures(1, &texture1); // Creating object with and ID, it is a function that takes in how many textures to generate, 
    glBindTexture(GL_TEXTURE_2D, texture1); // Binding it -> using it

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

    unsigned char* data = stbi_load("C:/Users/sinha/Desktop/Stryker Internship/Computer Graphics/OpenGL/images/illumanti_texture.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        // Now that texture is bound, we can start generating a texture using the previouly loaded image data, textures generated via glTexImage2D
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // automatically generate mipmaps
        // Let us go through the parameters one by one
        // glTexImage2D(specifies texture target- any 1d/3d texture bound objects not affected), mipmap level- leave 0 at base level,
        // what type of format we want to store the target, our image has only rgbm so we store in rgb, width, height, this is 0 for legacy purpose
        // 7 and 8th argument specify the format and datatype, last is actual image data
    }
    else {
        std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE" << std::endl;
    }
    // free space and then lets do texture 2
    stbi_image_free(data);

    // <-------------------------------------TEXTURE 2----------------------------------->
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // width height thing already declared, so we can continue with just getting our data
    data = stbi_load("C:/Users/sinha/Desktop/Stryker Internship/Computer Graphics/OpenGL/images/spiderman.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE" << std::endl;
    }
    stbi_image_free(data);

    // Activate shader before setting uniforms-> IMP!!!!!!!
    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // setting uniforms
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0); // activate texture unit --> GL_TEXTURE0 to GL_TEXTURE15 16 Textures
        glBindTexture(GL_TEXTURE_2D, texture1);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        // draw our first triangle
        ourShader.use();
        
        glm::mat4 model = glm::mat4(1.0f); //initializing a identity matrix
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-60.0f),
            glm::vec3(1.0f, 0.5f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        // we are going back basically in the scene.
        view = glm::translate(view, glm::vec3(0.0f, -0.0f, -3.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


        // Let us have some fun with the time parameter
        float mytime = glfwGetTime();
        int timelocation = glGetUniformLocation(ourShader.ID, "getTime");
        glUniform1f(timelocation, mytime);

        int mixlocation = glGetUniformLocation(ourShader.ID, "mixValue");
        glUniform1f(mixlocation, mixValue);

        int scalelocation = glGetUniformLocation(ourShader.ID, "scaleFactor");
        glUniform1f(scalelocation, scaleValue);


        // ======= First Container (Rotating and Moving) =======
        // let us change the object transformations during the loop too!
        int modelLoc = glGetUniformLocation(ourShader.ID, "model");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
  
    }

    // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------



void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { // Pressing UP key
            mixValue = mixValue + 0.0009;
            mixValue = mixValue > 1 ? 1 : mixValue;
    }   
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { // Pressing DOWN key
            mixValue = mixValue - 0.0009;
            mixValue = mixValue < 0 ? 0 : mixValue;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { // Pressing LEFT key
        scaleValue = scaleValue - 0.001;
        
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { // Pressing DOWN key
        scaleValue = scaleValue + 0.001;
    }


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

