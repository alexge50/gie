#ifndef GIE_FONT_H
#define GIE_FONT_H

#include <string_view>
#include <vector>

#include <detail/CenteredBox.h>

class Font
{
public:
    struct Glyph
    {
        std::vector<uint8_t> data;
        glm::ivec2 bearing;
        unsigned int advance;
        unsigned int width;
        unsigned int height;
    };

    Font(const char* font_file, int font_size);
    Glyph get_glyph(char c) const;
    int get_font_size() const;
    glm::vec2 compute_bounding_box(std::string_view, float target_font_size) const;

private:
    Glyph glyphs[128];
    int font_size;
};

#endif //GIE_FONT_H