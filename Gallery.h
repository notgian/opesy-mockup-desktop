#pragma once

#include "imgui.h"
#include "GL/glew.h"
#include <vector>
#include <string>

class Gallery {
public:
    Gallery();

    void InitTextures();

    // Renders the Gallery window if show_window is true
    void Render(bool* show_window, float screen_width, float screen_height);

private:
    struct GalleryImage {
        GLuint texture_id = 0;
        int width = 0;
        int height = 0;
    };

    std::vector<GalleryImage> m_images;
    int m_currentIndex = 0;
    bool m_texturesLoaded = false;
};