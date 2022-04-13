#pragma once

#include "../config.hpp"
#include "stb_image.h"

#include <filesystem>
#include <string>
#include <vector>

namespace img {

class Image
{
private:
    unsigned char* data     = nullptr;
    unsigned int width      = 0u;
    unsigned int height     = 0u;
    unsigned int pixel_size = 0u;

public:
    Image(const std::filesystem::path& filepath)
    {
        int w = 0;
        int h = 0;
        int p = 0;

        data       = stbi_load(filepath.string().c_str(), &w, &h, &p, 0);
        width      = static_cast<unsigned int>(w);
        height     = static_cast<unsigned int>(h);
        pixel_size = static_cast<unsigned int>(p);
    }

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    ~Image() { stbi_image_free(data); }

    bool valid() const { return pixel_size != 0; }
    bool has_alpha() const { return pixel_size == 4; }

    unsigned int get_width() const { return width; }
    unsigned int get_height() const { return height; }
    unsigned int get_pixel_size() const { return pixel_size; }

    const unsigned char* get_data() const { return data; }
};

} // namespace img