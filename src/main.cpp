#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Graph.h>
#include <detail/Shader.h>
#include <NodeEditor.h>
#include <Render.h>
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

    NodeEditor node_editor;
    Render render;

    node_editor.graph = {
        {{1, {{}, {},  {}, {0.f, 0.f}, {0.5f, 0.f, 0.5f}, {}}}},
        {}
    };

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        render(node_editor, {float(width), float(height)});

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