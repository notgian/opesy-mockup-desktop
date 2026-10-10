#include "Gallery.h"

Gallery::Gallery() {
}

void Gallery::Render(bool* show_window, float screen_width, float screen_height) {
    if (!show_window || !*show_window)
        return;

    // Center the gallery window or give it a default size/position relative to screen size
    float win_width = 800.0f;
    float win_height = 500.0f;
    ImGui::SetNextWindowPos(ImVec2((screen_width - win_width) * 0.5f, (screen_height - win_height) * 0.5f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(win_width, win_height), ImGuiCond_FirstUseEver);

    // Render the window with a close button hook linked to show_window
    if (ImGui::Begin("Gallery App", show_window)) {
        // Empty window boilerplate - ready for your content
        ImGui::Text("Welcome to the Gallery Application!");
        ImGui::Separator();
        
        // Example placeholder layout
        ImGui::BeginChild("GalleryContent", ImVec2(0, 0), true);
        ImGui::Text("Images and media content will go here...");
        ImGui::EndChild();
    }
    ImGui::End();
}