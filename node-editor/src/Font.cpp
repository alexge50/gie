#include <Font.h>

#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H

Font::Font(const char *font_file, int font_size):
    font_size{font_size}
{
    FT_Library ft;
    if(FT_Init_FreeType(&ft))
    {
        std::terminate();
    }

    FT_Face face;
    if(FT_New_Face(ft, font_file, 0, &face))
    {
        std::terminate();
    }

    FT_Set_Pixel_Sizes(face, 0, font_size);

    max_height_ = 0.f;
    float max_bearing = 0.f;
    max_sub_line_ = 0.f;
    for(int i = 0; i < 128; i ++)
    {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            std::terminate();
        }

        glyphs[i].data.resize(face->glyph->bitmap.width * face->glyph->bitmap.rows);
        std::copy(face->glyph->bitmap.buffer, face->glyph->bitmap.buffer + face->glyph->bitmap.width * face->glyph->bitmap.rows, glyphs[i].data.begin());
        glyphs[i].width = face->glyph->bitmap.width;
        glyphs[i].height = face->glyph->bitmap.rows;
        glyphs[i].advance = face->glyph->advance.x;
        glyphs[i].bearing = glm::vec2{face->glyph->bitmap_left, face->glyph->bitmap_top};

        max_bearing = std::max(max_bearing, static_cast<float>(glyphs[i].bearing.y));
        max_sub_line_ = std::max(max_sub_line_, static_cast<float>(glyphs[i].height) - static_cast<float>(glyphs[i].bearing.y));
    }
    max_height_ = max_bearing + max_sub_line_;
}

Font::Glyph Font::get_glyph(char c) const
{
    return glyphs[int(c)];
}

glm::vec2 Font::compute_bounding_box(std::string_view string, float target_font_size) const
{
    float scale = target_font_size / float(font_size);
    float width = 0.f;
    float height = 0.f;

    for(char c: string)
    {
        width += (glyphs[int(c)].advance >> 6) * scale;
        height = std::max(height, static_cast<float>(glyphs->height) * scale);
    }

    return glm::vec2{width, height};
}

int Font::get_font_size() const
{
    return font_size;
}

glm::vec2 Font::compute_max_bounding_box(int text_length, float target_font_size) const
{
    float scale = target_font_size / float(font_size);
    unsigned int max_advance = 0;
    float height = 0.f;

    for(const auto &glyph: glyphs)
    {
        max_advance = std::max(max_advance, glyph.advance);
        height = std::max(height, static_cast<float>(glyphs->height) * scale);
    }

    return glm::vec2{
            (max_advance >> 6) * text_length * scale,
            max_height_ * scale
    };
}

float Font::max_height(float target_font_size) const
{
    float scale = target_font_size / float(font_size);
    return max_height_ * scale;
}

float Font::max_sub_line(float target_font_size) const
{
    float scale = target_font_size / float(font_size);
    return max_sub_line_ * scale;
}


