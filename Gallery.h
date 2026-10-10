#pragma once

#include "imgui.h"

class Gallery {
public:
    Gallery();

    // Renders the Gallery window if show_window is true
    void Render(bool* show_window, float screen_width, float screen_height);
};