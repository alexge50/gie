#ifndef NODE_EDITOR_RENDER_H
#define NODE_EDITOR_RENDER_H

#include "GraphCache.h"
#include "NodeEditor.h"
#include "Font.h"
#include "detail/GeometryBuffer.h"
#include "shader_solid.h"
#include "shader_background.h"
#include "shader_text.h"

class Render
{
public:
    Render(const Font&);
    ~Render();

    void operator()(const GraphCache& graph_cache, const StylingConfig& styling_config);

private:
    const Font* font;
    GeometryBuffer quad, quad_outline, circle, line;
    unsigned glyph_textures[256] {};

    Shaders::solid solid_shader;
    Shaders::background background_shader;
    Shaders::text text_shader;
};

#endif //NODE_EDITOR_RENDER_H
