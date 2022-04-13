#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>

class MediaPath
{
public:
    static inline void initialize(const std::string_view executable_path)
    {
        media_path = executable_path;
        media_path.replace_filename("media");
    }

    MediaPath(const std::string_view path_) : path { path_ } {}

    std::filesystem::path get_full_path() const
    {
        if (media_path.empty())
        {
            throw std::runtime_error { "MediaPath::initialize() should have been called before "
                                       "trying to retrieve a path from MediaPath." };
        }

        return media_path / path;
    }

private:
    static inline std::filesystem::path media_path;

    const std::filesystem::path path;
};
