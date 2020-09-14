#ifndef NODE_EDITOR_RENDER_H
#define NODE_EDITOR_RENDER_H

#include "NodeEditor.h"
#include "detail/Shader.h"

class Render
{
public:
    Render();
    ~Render();

    void operator()(const NodeEditor& node_editor, glm::vec2 screen_size);

private:
    unsigned int quad_vbo, quad_vao;

    Shader background_shader;
    int background_mvp_location, background_model_location, background_scale_location;
};

#endif //NODE_EDITOR_RENDER_H
