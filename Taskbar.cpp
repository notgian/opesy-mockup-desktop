#include "Taskbar.h"
#include "imgui.h"

void RenderTaskbar(float screen_width, float screen_height, GLFWwindow* window, bool* show_demo_window) {
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

    // App 1 Button
    if (ImGui::Button("App 1", ImVec2(70.0f, 30.0f))) {
        *show_demo_window = true;
    }

    ImGui::SameLine();

    // App 2 Button
    if (ImGui::Button("App 2", ImVec2(70.0f, 30.0f))) {
        *show_demo_window = true;
    }

    ImGui::SameLine();

    // App 3 Button
    if (ImGui::Button("App 3", ImVec2(70.0f, 30.0f))) {
        *show_demo_window = true;
    }

    /*
    // Future Custom Application Windows Implementation:
    // You can manage individual application states and render custom windows here:
    // 
    // static bool show_app1_window = false;
    // if (show_app1_window) {
    //     ImGui::Begin("App 1 Window", &show_app1_window);
    //     ImGui::Text("Welcome to Application 1!");
    //     ImGui::End();
    // }
    */

    ImGui::End();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}