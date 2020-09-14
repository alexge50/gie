#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader.h>
#include <shaders.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;

struct Parameters
{
    float scroll = 5.f;
    double x, y;
};

void scroll_callback(GLFWwindow* window, double x, double y);


int main()
{
    GLFWwindow* window;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            "Node Editor",
            nullptr,
            nullptr
    );

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    Parameters parameters;
    glfwSetWindowUserPointer(window, &parameters);
    glfwSetScrollCallback(window, scroll_callback);


    unsigned int vbo, ebo, vao;

    {
        unsigned int v[2];
        glGenBuffers(2, v);
        vbo = v[0];

        glGenVertexArrays(1, &vao);
    }


    float background_quad[] = {
            0, 1.0,
            1.0, 1.0,
            0, 0,
            1.0, 0,
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            8 * sizeof(float),
            background_quad,
            GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0
    );

    Shader shader = createShader(
    std::string{reinterpret_cast<const char*>(background_vertex_glsl)},
    std::string{reinterpret_cast<const char*>(background_fragment_glsl)}).value();

    shader.use();
    auto mvp_location = shader.getUniformLocation("mvp");
    auto model_location = shader.getUniformLocation("model");
    auto scale_location = shader.getUniformLocation("scale");

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        float scale = glm::max(parameters.scroll * 0.5f, 0.001f);

        glm::mat4 model =
                glm::scale(glm::mat4(1.f), glm::vec3(float(width), float(height), 0.f));
        glm::mat4 projection = glm::ortho(0.f, float(width), float(height), 0.f);

        glm::mat4 mvp = projection * model;

        glUniformMatrix4fv(mvp_location, 1, 0, glm::value_ptr(mvp));
        glUniformMatrix4fv(model_location, 1, 0, glm::value_ptr(model));

        glUniform1f(scale_location, scale);

        shader.use();
        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
                );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void scroll_callback(GLFWwindow* window, double x, double y)
{
    auto parameters = static_cast<Parameters*>(glfwGetWindowUserPointer(window));
    parameters->scroll += y;
}