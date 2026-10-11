#include "Notepad.h"
#include "imgui.h"

#include <cfloat>
#include <cstring>

namespace {
    const int NOTEPAD_BUF_SIZE = 8192;

    char g_text[NOTEPAD_BUF_SIZE] =
        "Welcome to CSOPESY Notepad!\n"
        "\n"
        "This is a placeholder text editor for the desktop mock-up.\n"
        "Click anywhere and start typing.\n"
        "\n"
        "TODO:\n"
        " - finish the Desktop-Style OS mock-up\n"
        " - record the video walkthrough\n"
        " - prepare the PPT technical report\n";

    const char* g_status = "Ready";
}

void RenderNotepad(bool* p_open) {
    if (p_open == nullptr || !*p_open) return;

    ImGui::SetNextWindowPos(ImVec2(180.0f, 70.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(560.0f, 380.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(320.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));

    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings;
    // "###Notepad" keeps the window ID stable even if the title text changes.
    if (!ImGui::Begin("Untitled - Notepad###Notepad", p_open, flags)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) { g_text[0] = '\0'; g_status = "New document"; }
            if (ImGui::MenuItem("Save")) { g_status = "Saved (placeholder - nothing written to disk)"; }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) *p_open = false;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Clear all")) { g_text[0] = '\0'; g_status = "Cleared"; }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Text area fills the window, leaving one line at the bottom for the status bar
    float footer_h = ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y;
    if (ImGui::InputTextMultiline("##NotepadText", g_text, NOTEPAD_BUF_SIZE,
                                  ImVec2(-FLT_MIN, -footer_h),
                                  ImGuiInputTextFlags_AllowTabInput)) {
        g_status = "Edited";
    }

    ImGui::Separator();
    ImGui::TextDisabled("%s   |   Chars: %d   |   UTF-8", g_status, (int)std::strlen(g_text));

    ImGui::End();
}
