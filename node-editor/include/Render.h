#ifndef NODE_EDITOR_RENDER_H
#define NODE_EDITOR_RENDER_H

#include "GraphCache.h"
#include "NodeEditor.h"
#include "detail/GeometryBuffer.h"
#include "shader_solid.h"
#include "shader_background.h"

static const float OUTLINE_Z_LOCATION = 2.f;
static const float BASE_Z_LOCATION = 0.f;
static const float PORT_Z_LOCATION = 3.f;
static const float PORT_OUTLINE_Z_LOCATION = 4.f;
static const float HEADER_Z_LOCATION = 1.f;
static const float STRIDE_Z_LOCATION = 5.f;
static const float CONNECTION_Z_LOCATION = 1.f;

class Render
{
public:
    Render();
    ~Render();

    void operator()(const GraphCache& graph_cache, const StylingConfig& styling_config);

private:
    GeometryBuffer quad, quad_outline, circle, line;

    Shaders::solid solid_shader;
    Shaders::background background_shader;
};

#endif //NODE_EDITOR_RENDER_H
