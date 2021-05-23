#ifndef NODE_EDITOR_RENDER_H
#define NODE_EDITOR_RENDER_H

#include "RenderData.h"
#include "NodeEditor.h"
#include "Font.h"
#include "detail/GeometryBuffer.h"
#include "shader_solid.h"
#include "shader_background.h"
#include "shader_text.h"
#include "shader_stencil_text.h"
#include "shader_color_space.h"
#include "shader_monochrome_gradient.h"

class Render
{
public:
    Render(const Font&);
    ~Render();

    void operator()(const RenderData& render_data);

private:
    const Font* font;
    GeometryBuffer quad, quad_outline, circle, line;
    unsigned glyph_textures[256] {};

    Shaders::solid solid_shader;
    Shaders::background background_shader;
    Shaders::text text_shader;
    Shaders::stencil_text stencil_text_shader;
    Shaders::color_space color_space;
    Shaders::monochrome_gradient monochrome_gradient;
};

#endif //NODE_EDITOR_RENDER_H
