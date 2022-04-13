#pragma once

#include "../img/image.hpp"
#include "opengl_interface.hpp"

#include <GL/glut.h>
#include <array>
#include <cassert>

namespace GL {

inline GLuint init_texture(const img::Image* image)
{
    static_assert(sizeof(decltype(*image->get_data())) == sizeof(GLubyte));

    GLuint tex_index;
    assert(image);
    glGenTextures(1, &tex_index);
    glBindTexture(GL_TEXTURE_2D, tex_index);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, image->get_pixel_size(), image->get_width(), image->get_height(), 0,
                 image->has_alpha() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image->get_data());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    handle_error("Cannot create texture");
    return tex_index;
}

class Texture2D
{
protected:
    const img::Image* image = nullptr;
    GLuint tex_index        = 0;
    float tile_width        = 0.f;

public:
    Texture2D(const img::Image* image_, const size_t num_tiles = 1) :
        image { image_ }, tex_index { init_texture(image) }, tile_width { 1.0f / num_tiles }
    {}

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    ~Texture2D() { glDeleteTextures(1, &tex_index); }

    void draw(Point2D pos, const Point2D& dim, const size_t tile_idx = 0) const
    {
        glBindTexture(GL_TEXTURE_2D, tex_index);
        glColor3f(1, 1, 1);

        glBegin(GL_QUADS);
        draw_point(TopLeft, pos, dim, tile_idx);
        draw_point(TopRight, pos, dim, tile_idx);
        draw_point(BottomRight, pos, dim, tile_idx);
        draw_point(BottomLeft, pos, dim, tile_idx);
        glEnd();

        handle_error("Cannot display texture");
    }

    const img::Image& get_image() const { return *image; }

private:
    enum Coordinate
    {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Count,
    };

    static inline const std::array<Point2D, Coordinate::Count> _texture_shifts {
        Point2D { 0, 0 }, Point2D { 1, 0 }, Point2D { 0, 1 }, Point2D { 1, 1 }
    };

    static inline const std::array<Point2D, Coordinate::Count> _screen_shifts {
        Point2D { -1, 1 }, Point2D { 1, 1 }, Point2D { -1, -1 }, Point2D { 1, -1 }
    };

    void draw_point(Coordinate coordinate, const Point2D& screen_pos, const Point2D& screen_dim,
                    const size_t tile_idx) const
    {
        const auto& tex_shift    = _texture_shifts[coordinate];
        const auto& screen_shift = _screen_shifts[coordinate];

        glTexCoord2f((tile_idx + tex_shift.x()) * tile_width, tex_shift.y());

        const auto vertex = screen_pos + screen_dim * screen_shift * 0.5f;
        glVertex2fv(vertex.values);
    }
};

} // namespace GL