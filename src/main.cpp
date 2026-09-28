#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
constexpr double PI = 3.14159265358979323846;

#include "Shader.h"
#include "Sphere.h"
#include "Camera.h"
#include "Texture.h"
#include "ProjectPaths.h"

void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

//  Screen settings
const unsigned int screenWidth = 1920;
const unsigned int screenHeight = 1080;

//  Camera settings
Camera camera(glm::vec3(0.0f, 0.0f, 4500.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

//  Time between current and last frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int runApplication(int argc, char** argv)
{

    /*  Window creation and GLEW initialization */
    /* Initialize the library */
    glfwSetErrorCallback([](int code, const char* message) {
        std::cerr << "GLFW error " << code << ": " << message << '\n';
    });
    if (!glfwInit()) throw std::runtime_error("GLFW initialization failed.");

    //  GLFW version and core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Solar System", NULL, NULL);
    if (!window)
    {
        throw std::runtime_error("Could not create an OpenGL 3.3 window.");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /*  Set function for mouse control  */
    glfwSetCursorPosCallback(window, mouseCallback);
    /* Set function for scroll zooming  */
    glfwSetScrollCallback(window, scrollCallback);
    /*  Tell GLFW to capture our mouse  */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        throw std::runtime_error(reinterpret_cast<const char*>(glewGetErrorString(err)));
    }
    /*  Window creation and GLEW initialization */

    // GLEW may probe legacy extensions in a core context.
    while (glGetError() != GL_NO_ERROR) {}
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        glViewport(0, 0, width, height);
    });
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    const auto root = resourceRoot(argv[0]);
    const std::string shader_directory = (root / "shaders").string() + "/";
    const std::string texture_directory = (root / "assets/textures").string() + "/";

    /*  Configure global OpenGL state   */
    glEnable(GL_DEPTH_TEST);

    /*  Shader paths    */
    std::string vs = shader_directory + "vert.glsl";
    std::string fs = shader_directory + "frag.glsl";
    /*  Shader paths    */

    //  Generate shader
    Shader shaderProgram(vs.data(), fs.data());

    /*  Generate Texture    */
    //  Sun
    std::string texturePath = texture_directory + "2k_sun.jpg";
    Texture sunTexture(texturePath.data(), GL_RGB);
    sunTexture.texUnit(shaderProgram, "tex0");
    //  Mercury
    texturePath = texture_directory + "2k_mercury.jpg";
    Texture mercuryTexture(texturePath.data(), GL_RGB);
    mercuryTexture.texUnit(shaderProgram, "tex0");
    //  Venus
    texturePath = texture_directory + "2k_venus.jpg";
    Texture venusTexture(texturePath.data(), GL_RGB);
    venusTexture.texUnit(shaderProgram, "tex0");
    //  Earth
    texturePath = texture_directory + "2k_earth.jpg";
    Texture earthTexture(texturePath.data(), GL_RGB);
    earthTexture.texUnit(shaderProgram, "tex0");
    //  Mars
    texturePath = texture_directory + "2k_mars.jpg";
    Texture marsTexture(texturePath.data(), GL_RGB);
    marsTexture.texUnit(shaderProgram, "tex0");
    //  Jupiter
    texturePath = texture_directory + "2k_jupiter.jpg";
    Texture jupiterTexture(texturePath.data(), GL_RGB);
    jupiterTexture.texUnit(shaderProgram, "tex0");
    //  Saturn
    texturePath = texture_directory + "2k_saturn.jpg";
    Texture saturnTexture(texturePath.data(), GL_RGB);
    saturnTexture.texUnit(shaderProgram, "tex0");
    //  Uranus
    texturePath = texture_directory + "2k_uranus.jpg";
    Texture uranusTexture(texturePath.data(), GL_RGB);
    uranusTexture.texUnit(shaderProgram, "tex0");
    //  Neptune
    texturePath = texture_directory + "2k_neptune.jpg";
    Texture neptuneTexture(texturePath.data(), GL_RGB);
    neptuneTexture.texUnit(shaderProgram, "tex0");
    /*  Generate Texture    */


    /*  Generate orbits and Saturn rings  */
    //  Vertices
    std::vector<float> circleVert;
    GLfloat x;
    GLfloat z;
    float angle;
    for (int i = 0; i < 2000; i++)
    {
        angle = (float) (PI / 2 - i * (PI / 1000));

        x = sin(angle) * 100;
        z = cos(angle) * 100;

        circleVert.push_back(x);
        circleVert.push_back(0.0f);
        circleVert.push_back(z);
    }
    //  Buffer data
    unsigned int VBO_t, VAO_t;
    glGenVertexArrays(1, &VAO_t);
    glGenBuffers(1, &VBO_t);
    glBindVertexArray(VAO_t);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_t);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * circleVert.size(), circleVert.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    /*  Generate orbits and Saturn rings  */


    // A real annulus with radial UVs. Bands are generated in the fragment shader.
    std::vector<float> ringVertices;
    constexpr int ringSegments = 256;
    for (int i = 0; i <= ringSegments; ++i) {
        const float angle = static_cast<float>(2.0 * PI * i / ringSegments);
        for (int edge = 0; edge < 2; ++edge) {
            const float radius = edge == 0 ? 110.0f : 180.0f;
            ringVertices.insert(ringVertices.end(), {radius * std::cos(angle),
                radius * std::sin(angle), 0.0f, static_cast<float>(edge), 0.0f});
        }
    }
    GLuint ringVAO, ringVBO;
    glGenVertexArrays(1, &ringVAO);
    glGenBuffers(1, &ringVBO);
    glBindVertexArray(ringVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ringVBO);
    glBufferData(GL_ARRAY_BUFFER, ringVertices.size() * sizeof(float), ringVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    /*  Generate sphere */
    Sphere sun(500.0f, 36 * 5, 18 * 5);
    Sphere mercury(3.0f, 36, 18);
    Sphere venus(10.0f, 36, 18);
    Sphere earth(10.0f, 36, 18);
    Sphere mars(6.0f, 36, 18);
    Sphere jupiter(100.0f, 36, 18);
    Sphere saturn(90.0f, 36, 18);
    Sphere uranus(50.0f, 36, 18);
    Sphere neptune(45.0f, 36, 18);
    /*  Generate sphere */

    lastFrame = static_cast<float>(glfwGetTime());
    int frames = 0;
    const bool smokeTest = argc > 1 && std::string(argv[1]) == "--smoke-test";
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //  Delta frame so frames don't affect camera speed
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
        if (framebufferWidth == 0 || framebufferHeight == 0) {
            glfwWaitEvents();
            continue;
        }
        deltaTime = std::min(deltaTime, 0.1f);
        //  Input
        processInput(window);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //  Activate shader
        shaderProgram.Activate();
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "renderMode"), 0);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);

        proj = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(framebufferWidth) / framebufferHeight, 0.1f, 7000.0f);
        shaderProgram.setMat4("proj", proj);

        view = camera.GetViewMatrix();
        // Keep the scene stationary; camera movement is controlled by input.
        shaderProgram.setMat4("view", view);
        view = glm::rotate(view, glm::radians(100.0f), glm::vec3(1.0, 0.0, 0.0));
        view = glm::rotate(view, glm::radians(10.0f), glm::vec3(0.0, 1.0, 0.0));
        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("model", model);

        /*      Sun     */

        //apply texture
        sunTexture.Bind();
        
        //global model
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);

        //local model
        glm::mat4 modelSun = glm::mat4(1.0f);
        modelSun = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.2f,
                               glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMat4("model", modelSun);


        sun.Draw();
        /*      Sun     */

      
        /*  Mercury   */
        //apply texture
        mercuryTexture.Bind();

        //global model
        model = glm::mat4(1.0f);
        model = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.47f,
                          glm::vec3(0.0f, 0.0f, 1.f));
        shaderProgram.setMat4("model", model);
        model = glm::translate(model, glm::vec3(620.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);

        //local model
        glm::mat4 modelMercury = glm::mat4(1.0f);
        modelMercury = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.5f,
                                   glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMat4("model", modelMercury);

        //draw planet
        mercury.Draw();
        /*  Mercury   */


        /*  Venus   */
        //apply texture
        venusTexture.Bind();

        //global model

        model = glm::mat4(1.0f);
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.35f,
                       glm::vec3(0.0f, 0.0f, 1.f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(720.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(177.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);

        //local model
        glm::mat4 modelVenus = glm::mat4(1.0f);
        modelVenus = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.3f,
                                   glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMat4("model", modelVenus);

        //draw planet
        venus.Draw();
        /*  Venus   */


        /*  Earth   */
        //apply texture
        earthTexture.Bind();

        //global model

        model = glm::mat4(1.0f);
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.30f,
                          glm::vec3(0.0f, 0.0f, 1.f));
        shaderProgram.setMat4("model", model);
        model = glm::translate(model, glm::vec3(-800.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(23.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);

        //local model
        glm::mat4 modelEarth = glm::mat4(1.0f);
        modelEarth = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 1.0f,
                                 glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMat4("model", modelEarth);

        //draw planet
        earth.Draw();
        /*  Earth   */


        /*  Mars   */
        //apply texture
        marsTexture.Bind();

        //global model

        model = glm::mat4(1.0f);
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.24f,
                            glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(950.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);

        //local model
        glm::mat4 modelMars = glm::mat4(1.0f);
        modelMars = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.95f,
                                 glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMat4("model", modelMars);

        //draw planet
        mars.Draw();
        /*  Mars   */
        

        /*  Jupiter   */
        //apply texture
        jupiterTexture.Bind();

        //global model

        model = glm::mat4(1.0f);
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.13f,
                            glm::vec3(0.0f, 0.0f, 1.f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(1200.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);

        //local model
        glm::mat4 modelJupiter = glm::mat4(1.0f);
        modelJupiter = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 1.2f,
                                glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMat4("model", modelJupiter);

        //draw planet
        jupiter.Draw();
        /*  Jupiter   */


        /*  Saturn   */
        //apply texture
        saturnTexture.Bind();

        //global model

        model = glm::mat4(1.0f);
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.10f,
                            glm::vec3(0.0f, 0.0f, 1.f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(150.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(1600.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(27.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
        shaderProgram.setMat4("model", model);

        //local model
        glm::mat4 modelSaturn = glm::mat4(1.0f);
        modelSaturn = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 1.1f,
                                   glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMat4("model", modelSaturn);

        //draw planet
        saturn.Draw();
        /*  Saturn   */


        /* Saturn rings: reuse Saturn's orbital position and axial tilt. */
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "renderMode"), 2);
        shaderProgram.setMat4("model", model);
        glBindVertexArray(ringVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(ringVertices.size() / 5));
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "renderMode"), 0);

        /*  Uranus   */
        //apply texture
        uranusTexture.Bind();

        //global model

        model = glm::mat4(1.0f);
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.07f,
                            glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(2000.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(98.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);

        //local model
        glm::mat4 modelUranus = glm::mat4(1.0f);
        modelUranus = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 1.05f,
                                  glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMat4("model", modelUranus);

        //draw planet
        uranus.Draw();
        /*  Uranus   */


        /*  Neptune   */
        //apply texture
        neptuneTexture.Bind();

        //global model

        model = glm::mat4(1.0f);
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 0.05f,
                            glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(300.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(2400.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);

        //local model
        glm::mat4 modelNeptune = glm::mat4(1.0f);
        modelNeptune = glm::rotate(model, (GLfloat) glfwGetTime() * glm::radians(20.0f) * 1.05f,
                                  glm::vec3(0.0f, 0.0f, 1.f));
        shaderProgram.setMat4("model", modelNeptune);

        //draw planet
        neptune.Draw();
        /*  Neptune   */


        /*  Orbits  */
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "renderMode"), 1);
        glBindVertexArray(VAO_t);
        glLineWidth(1.0f);

        model = glm::mat4(1.0f);

        for (int i = 0; i < 8; i++)
        {
            if (i == 0) model = glm::scale(model, glm::vec3(6.2f, 6.2f, 6.2f));
            if (i == 1) model = glm::scale(model, glm::vec3(7.2f, 7.2f, 7.2f));
            if (i == 2) model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
            if (i == 3) model = glm::scale(model, glm::vec3(9.5f, 9.5f, 9.5f));
            if (i == 4) model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
            if (i == 5) model = glm::scale(model, glm::vec3(16.0f, 16.0f, 16.0f));
            if (i == 6) model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
            if (i == 7) model = glm::scale(model, glm::vec3(24.0f, 24.0f, 24.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
            shaderProgram.setMat4("model", model);
            glDrawArrays(GL_LINE_LOOP, 0, (GLsizei) circleVert.size() / 3);
            model = glm::mat4(1.0f);
        }
        /*  Orbits  */


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        if (smokeTest) {
            const GLenum error = glGetError();
            if (error != GL_NO_ERROR) throw std::runtime_error("OpenGL smoke-test error: " + std::to_string(error));
            if (++frames >= 3) glfwSetWindowShouldClose(window, true);
        }
    }
    
    //  Delete shaders and textures
    shaderProgram.Delete();
    sunTexture.Delete();
    mercuryTexture.Delete();
    venusTexture.Delete();
    earthTexture.Delete();
    marsTexture.Delete();
    jupiterTexture.Delete();
    saturnTexture.Delete();
    uranusTexture.Delete();
    neptuneTexture.Delete();
    glDeleteBuffers(1, &VBO_t);
    glDeleteVertexArrays(1, &VAO_t);
    glDeleteBuffers(1, &ringVBO);
    glDeleteVertexArrays(1, &ringVAO);

    return 0;
}

//  Function for processing keyboard input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
}

//  Function for processing mouse input
void mouseCallback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Reversed since y-coordinates go from bottom to top
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

//  Function for scroll-wheel zoom
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// Resources in runApplication are destroyed while the GL context is still alive.
int main(int argc, char** argv) {
    int result = 0;
    try { result = runApplication(argc, argv); }
    catch (const std::exception& error) {
        std::cerr << "Solar System: " << error.what() << '\n';
        result = 1;
    }
    glfwTerminate();
    return result;
}
