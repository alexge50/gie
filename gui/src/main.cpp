#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Graph.h>
#include <NodeEditor.h>
#include <Render.h>
#include <Events.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;

struct Parameters
{
    InputEventVector* input_events = nullptr;
    Camera* camera = nullptr;

    bool dragging = false;
    bool mouse_clicked = false;
    bool super_clicked = false;
    //float mouse_x, mouse_y;
    WorldSpaceCoordinates mouse_position = glm::vec2{};
    ScreenSpaceCoordinates mouse_position_screen_space = glm::vec2{};
};

void scroll_callback(GLFWwindow* window, double x, double y);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double x, double y);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void character_callback(GLFWwindow* window, unsigned int codepoint);

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

    InputEventVector input_events;
    EditorEventVector editor_events;

    Parameters parameters {&input_events};
    glfwSetWindowUserPointer(window, &parameters);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);

    Font font{"/usr/share/fonts/liberation-mono/LiberationMono-Regular.ttf", 32};
    NodeEditor node_editor{};
    Render render{font};

    node_editor.font = &font;
    node_editor.graph = {
        .nodes = {
            {1, {1, {0.f, 0.f}}},
            {2, {1, {100.f, 100.f}}},
            {3, {1, {200.f, 200.f}}},
        },
        .node_types = {
                {1, {
                    {{"in", glm::vec3{0.85f, 0.72f, 0.02f}, Widgets::TextBox{5}}},
                    {{"out", glm::vec3{0.85f, 0.72f, 0.02f}}},
                    "default", {0.36f, 0.17f, 0.54f}
                }}
        },
        .connections = {{{1, 0}, {2, 0}}},
        .node_computed = {},
        .node_types_computed = {}
    };

    compute_state(node_editor, node_editor.state);

    parameters.camera = &node_editor.camera;
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        node_editor.camera.screen_size = {float(width), float(height)};
        compute_state(node_editor, node_editor.state);

        editor_events.clear();
        process(node_editor, input_events, editor_events);
        input_events.clear();

        render(compute_render_data(node_editor));

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

    parameters->input_events->add(InputEvents::Scroll{static_cast<float>(y) * 0.2f});
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto parameters = static_cast<Parameters*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
    {
        parameters->input_events->add(InputEvents::KeyPressed{InputEvents::KeyPressed::Key::DELETE});
    }
    else if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
    {
        parameters->input_events->add(InputEvents::KeyPressed{InputEvents::KeyPressed::Key::BACKSPACE});
    }
    else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        parameters->input_events->add(InputEvents::KeyPressed{InputEvents::KeyPressed::Key::LEFT_ARROW});
    }
    else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        parameters->input_events->add(InputEvents::KeyPressed{InputEvents::KeyPressed::Key::RIGHT_ARROW});
    }
    else if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        parameters->input_events->add(InputEvents::KeyPressed{InputEvents::KeyPressed::Key::UP_ARROW});
    }
    else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        parameters->input_events->add(InputEvents::KeyPressed{InputEvents::KeyPressed::Key::DOWN_ARROW});
    }
    else if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        parameters->input_events->add(InputEvents::KeyPressed{InputEvents::KeyPressed::Key::ENTER});
    }
}

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    auto parameters = static_cast<Parameters*>(glfwGetWindowUserPointer(window));

    parameters->mouse_position = to_world_space(*parameters->camera, glm::vec2{float(x), float(y)});
    parameters->mouse_position_screen_space = glm::vec2{float(x), float(y)};

    if(parameters->mouse_clicked)
    {


        if(!parameters->dragging)
        {
            parameters->input_events->add(InputEvents::DragBegin{parameters->mouse_position, parameters->mouse_position_screen_space, parameters->super_clicked});
            parameters->dragging = true;
        }
        else
            parameters->input_events->add(InputEvents::DragSustain{parameters->mouse_position, parameters->mouse_position_screen_space});
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    auto parameters = static_cast<Parameters*>(glfwGetWindowUserPointer(window));

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        parameters->mouse_clicked = true;
        parameters->super_clicked = mods & GLFW_MOD_SHIFT;

        parameters->input_events->add(InputEvents::Click{
            parameters->mouse_position,
            parameters->super_clicked
        });
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        if(parameters->dragging)
            parameters->input_events->add(InputEvents::DragEnd{});

        parameters->dragging = false;
        parameters->mouse_clicked = false;
        parameters->super_clicked = false;
    }
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    auto parameters = static_cast<Parameters*>(glfwGetWindowUserPointer(window));

    if(codepoint >= 0 && codepoint < 128)
        parameters->input_events->add(InputEvents::Character{static_cast<char>(codepoint)});
}