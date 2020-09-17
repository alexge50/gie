#ifndef NODE_EDITOR_RENDER_H
#define NODE_EDITOR_RENDER_H

#include "NodeEditor.h"
#include "detail/Shader.h"
#include "detail/GeometryBuffer.h"

class Render
{
public:
    Render();
    ~Render();

    void operator()(const NodeEditor& node_editor);

private:
    GeometryBuffer quad, quad_outline;

    Shader background_shader;
    int background_mvp_location, background_model_location, background_scale_location;
    int background_background_location, background_foreground_location, background_zoom_location, background_camera_position_location;

    Shader solid_shader;
    int solid_mvp_location, solid_color_location;
};

#endif //NODE_EDITOR_RENDER_H
