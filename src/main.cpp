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
#include <Events.h>
#include <shaders.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;

struct Parameters
{
    std::vector<InputEvent>* input_events = nullptr;

    bool dragging = false;
    bool mouse_clicked = false;
    bool super_clicked = false;
    float mouse_x, mouse_y;
};

void scroll_callback(GLFWwindow* window, double x, double y);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double x, double y);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

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

    std::vector<InputEvent> input_events;
    std::vector<EditorEvent> editor_events;

    Parameters parameters {&input_events};
    glfwSetWindowUserPointer(window, &parameters);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    NodeEditor node_editor;
    Render render;

    node_editor.graph = {
        {{1, {{}, {},  {}, {200.f, 100.f}, {0.36f, 0.17f, 0.54f}, {}}}},
        {}
    };

    compute(node_editor);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        process(node_editor, input_events, editor_events);
        input_events.clear();

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

    parameters->input_events->push_back(InputEvents::Scroll{static_cast<float>(y)});
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto parameters = static_cast<Parameters*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
    {
        parameters->input_events->push_back(InputEvents::Delete{});
    }
}

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    auto parameters = static_cast<Parameters*>(glfwGetWindowUserPointer(window));

    parameters->mouse_x = x;
    parameters->mouse_y = y;

    if(parameters->mouse_clicked)
    {
        if(!parameters->dragging)
        {
            parameters->input_events->push_back(InputEvents::DragBegin{static_cast<float>(x), static_cast<float>(y), parameters->super_clicked});
            parameters->dragging = true;
        }
        else
            parameters->input_events->push_back(InputEvents::DragSustain{static_cast<float>(x), static_cast<float>(y)});
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    auto parameters = static_cast<Parameters*>(glfwGetWindowUserPointer(window));

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        parameters->mouse_clicked = true;
        parameters->super_clicked = mods & GLFW_MOD_SHIFT;

        parameters->input_events->push_back(InputEvents::Click{static_cast<float>(parameters->mouse_x), static_cast<float>(parameters->mouse_y)});
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        if(parameters->dragging)
            parameters->input_events->push_back(InputEvents::DragEnd{});

        parameters->dragging = false;
        parameters->mouse_clicked = false;
        parameters->super_clicked = false;
    }

}