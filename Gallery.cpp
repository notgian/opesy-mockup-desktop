#include "Gallery.h"
#include "image.h"
#include <string>

Gallery::Gallery() : m_currentIndex(0), m_texturesLoaded(false) {}

void Gallery::InitTextures() {
    if (m_texturesLoaded) return;

    m_images.resize(5);
    for (int i = 0; i < 5; ++i) {
        std::string path = "./assets/" + std::to_string(i + 1) + ".png";
        LoadTextureFromFile(path.c_str(), &m_images[i].texture_id, &m_images[i].width, &m_images[i].height);
    }
    m_texturesLoaded = true;
}

void Gallery::Render(bool* show_window, float screen_width, float screen_height) {
    if (!show_window || !*show_window)
        return;

    InitTextures();

    float win_width = 800.0f;
    float win_height = 500.0f;
    ImGui::SetNextWindowPos(ImVec2((screen_width - win_width) * 0.5f, (screen_height - win_height) * 0.5f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(win_width, win_height), ImGuiCond_FirstUseEver);

    // Render the window with a close button hook linked to show_window
    if (ImGui::Begin("Gallery App", show_window)) {
        ImGui::Text("Image Gallery Viewer");
        ImGui::Separator();

        // Reserve space at the bottom for the control row (height: ~50px)
        float content_height = ImGui::GetContentRegionAvail().y - 50.0f;
        ImGui::BeginChild("GalleryContent", ImVec2(0, content_height), true, ImGuiWindowFlags_NoScrollbar);

        if (!m_images.empty() && m_images[m_currentIndex].texture_id != 0) {
            auto& current_img = m_images[m_currentIndex];

            // Fit image proportionally within content area
            float max_w = ImGui::GetContentRegionAvail().x;
            float max_h = ImGui::GetContentRegionAvail().y;
            
            float aspect_ratio = (float)current_img.width / (float)current_img.height;
            float render_w = max_w;
            float render_h = max_w / aspect_ratio;

            if (render_h > max_h) {
                render_h = max_h;
                render_w = max_h * aspect_ratio;
            }

            // Center horizontally
            float x_offset = (max_w - render_w) * 0.5f;
            if (x_offset > 0.0f) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_offset);
            }

            // Center vertically (optional, adds clean padding)
            float y_offset = (max_h - render_h) * 0.5f;
            if (y_offset > 0.0f) {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + y_offset);
            }

            ImGui::Image((ImTextureID)(intptr_t)current_img.texture_id, ImVec2(render_w, render_h));
        } else {
            ImGui::Text("Failed to load or find images in ./assets/ (1.png - 5.png)");
        }

        ImGui::EndChild();

        // Control row below the content area
        ImGui::Spacing();
        
        // Center the controls row horizontally
        float controls_width = 200.0f;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - controls_width) * 0.5f);

        // Left Arrow Button
        if (ImGui::Button("< Prev", ImVec2(60.0f, 30.0f))) {
            m_currentIndex--;
            if (m_currentIndex < 0) {
                m_currentIndex = 4; // Loop back to last image (5th image)
            }
        }

        ImGui::SameLine(0.0f, 20.0f);

        // Display current index text e.g., "1/5"
        std::string indicator = std::to_string(m_currentIndex + 1) + "/5";
        
        // Offset text slightly to vertically align with buttons
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
        ImGui::Text("%s", indicator.c_str());
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.0f);

        ImGui::SameLine(0.0f, 20.0f);

        // Right Arrow Button
        if (ImGui::Button("Next >", ImVec2(60.0f, 30.0f))) {
            m_currentIndex++;
            if (m_currentIndex > 4) {
                m_currentIndex = 0; // Loop back to first image
            }
        }
    }
    ImGui::End();
}