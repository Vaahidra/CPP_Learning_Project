#pragma once

#include "../config.hpp"
#include "../geometry.hpp"
#include "displayable.hpp"
#include "dynamic_object.hpp"

#include <GL/freeglut.h>
#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace GL {

inline unsigned int ticks_per_sec = DEFAULT_TICKS_PER_SEC;
inline float zoom                 = DEFAULT_ZOOM;
inline bool fullscreen            = false;

using KeyStroke = std::function<void(void)>;

inline std::unordered_map<char, KeyStroke> keystrokes;

void handle_error(const std::string& prefix, const GLenum err = glGetError());
void keyboard(unsigned char key, int, int);
void toggle_fullscreen();
void change_zoom(const float factor);
void init_gl(int argc, char** argv, const char* title);
void loop();
void exit_loop();

} // namespace GL
