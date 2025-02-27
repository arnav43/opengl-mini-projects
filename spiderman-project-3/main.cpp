#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// We need to load our image (say PNG), we can write our own image loader but still cumbersome, 
// Let us use a image-loading library that supports popular formats and does all work for us!
// like stb_image.h

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float mixValue = 0.50f;
float scaleValue = 1.00f;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{

    /*
    // let us translate a vector [1,0,0] by [1,1,0] ---> we should get [1+1, 0+1, 0+0] == [2 1 0]
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f); // intitializing it to identity matrix(diagnonals 1),
    // if we dont then it takes a null matrix ->> cant do subsequent operations with null matrix..
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << "[" << vec.x << " " << vec.y << " " << vec.z << "]" << std::endl; // Output [2 1 0]
    */
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); // rotating by 90 degrees along z axis
    // axis we want to rotate along should be a unit vector
    trans = glm::scale(trans, glm::vec3(1.5, 1.5, 1.5)); // scaling by 1.5x
    // how do we get these transformation matrix to the shaders?? we know GLSL also has mat4 type



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

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("3.3.shader.vs", "3.3.shader.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // Vertices  (XYZ)       Colors  (RGB)     Texture Coordinates (ST)
         0.5f,  0.5f, 0.0f,     1.0f,  0.0f, 0.0f,      1.0f,  1.0f,    // top right
         0.5f, -0.5f, 0.0f,     0.0f,  1.0f, 0.0f,      1.0f,  0.0f,    // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f,  0.0f, 1.0f,      0.0f,  0.0,     // bottom left
        -0.5f,  0.5f, 0.0f,     1.0f,  1.0f, 0.0f,      0.0f,  1.0f     // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 1. Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0); 
    // 2. Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1); 
    // 3. Texture Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
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

    // Activate shader before setting uniforms-> IMP
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
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0); // activate texture unit --> GL_TEXTURE0 to GL_TEXTURE15 16 Textures
        glBindTexture(GL_TEXTURE_2D, texture1);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        // draw our first triangle
        ourShader.use();
        
        // Let us have some fun with the time parameter
        float mytime = glfwGetTime();
        int timelocation = glGetUniformLocation(ourShader.ID, "getTime");
        glUniform1f(timelocation, mytime);

        int mixlocation = glGetUniformLocation(ourShader.ID, "mixValue");
        glUniform1f(mixlocation, mixValue);

        int scalelocation = glGetUniformLocation(ourShader.ID, "scaleFactor");
        glUniform1f(scalelocation, scaleValue);

        // let us change the object transformations during the loop too!
        float time_trans = glfwGetTime()*1.5;

        glm::mat4 trans = glm::mat4(1.0f);
        //trans = glm::rotate(trans, time_trans, glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(sin(time_trans), cos(time_trans), 1.0f));
        trans = glm::rotate(trans, time_trans, glm::vec3(0.0f, 0.0f, 1.0f));


        unsigned int transformlocation = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformlocation, 1, GL_FALSE, glm::value_ptr(trans));
        // first argument is uniforms location, second tells us how many matrices (1 in this case)
        // third asks us if we want to transpose our matrix, and then our matrix data...)
        // in the last argument, matrix data doesnt always match opengls expectation, so we convert data with value_ptr glm built in fxn

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
  
    }

    // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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

/*                                      NOTES
* Texture coordinates usually range from (0,0) to (1,1), But what happens if we go outside this range,
* the default behavior of OpenGL is to repeat the texture images --> (X.49f becomes 0.49f)
* But there are more options which OpenGL offers
* -GL_REPEAT: Repeats the texture image
* -GL_MIRRORED_REPEAT- Same as GL_REPEAT but mirrors the image with each repeat
* -GL_CLAMP_TO_EDGE- Clamps the coordinates between 0 and 1, the result is that higher coordinates becomes clamped to the edge(results in streatched edge pattern)
* -GL_CLAMP_TO_BORDER- Coordinates outside the range are now given a user specified border color
* Texture coordinates do not depend on resolution but can be any floating point value
* OpenGL has to figure out which texture pixel (TEXEL) to map the texture coordinate to
* This becomes imp if object large but texture smol
* GL_NEAREST and GL_LINEAR are two most important options for texture filtering
* We can assign a location to texture sampler-> so we can set multiple textures at once in a fragment shader
* The location of a texture is known as a TEXTURE UNIT, default value is 0.
* 
*/