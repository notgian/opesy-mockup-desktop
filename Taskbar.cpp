#include "Taskbar.h"
#include "imgui.h"

void RenderTaskbar(float screen_width, float screen_height, GLFWwindow* window, bool* show_demo_window, bool* show_gallery_window,
                   bool* show_task_manager, bool* show_notepad) {
    float taskbar_height = 45.0f;
    ImGui::SetNextWindowPos(ImVec2(0, screen_height - taskbar_height), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(screen_width, taskbar_height), ImGuiCond_Always);

    ImGuiWindowFlags taskbar_flags = ImGuiWindowFlags_NoDecoration |
                                     ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoSavedSettings |
                                     ImGuiWindowFlags_NoFocusOnAppearing |
                                     ImGuiWindowFlags_NoNav;

    // Solid opaque black background and zero padding for a clean dock look
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 6.0f));

    ImGui::Begin("TaskbarWindow", nullptr, taskbar_flags);

    // PWR Button (Leftmost) - closes the window/application gracefully
    if (ImGui::Button("PWR", ImVec2(50.0f, 30.0f))) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();


    // App 1 Button -> Notepad (unique app)
    if (ImGui::Button("NOTE", ImVec2(70.0f, 30.0f))) {
        if (show_notepad) {
            *show_notepad = true;
            ImGui::SetWindowFocus("Untitled - Notepad###Notepad");
        }
    }

    ImGui::SameLine();

    // App 2 Button
    if (ImGui::Button("GALLERY", ImVec2(70.0f, 30.0f))) {
        if (show_gallery_window) {
            *show_gallery_window = true;
            ImGui::SetWindowFocus("Gallery App");
        }
    }

    // App 3 Button -> Task Manager
    ImGui::SameLine();

    if (ImGui::Button("TASK", ImVec2(70.0f, 30.0f))) {
        if (show_task_manager) {
            *show_task_manager = true;
            ImGui::SetWindowFocus("Task Manager"); // bring to front if already open
        }
    }


    ImGui::End();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}