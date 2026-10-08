#include "TimeClock.h"
#include "imgui.h"
#include <ctime>
#include <iomanip>
#include <sstream>

void RenderTimeClock(float screen_width, float screen_height) {
    // Get current local system time
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    // Format time string (e.g., "10:45:30 PM")
    std::ostringstream time_oss;
    time_oss << std::put_time(local_time, "%I:%M:%S %p");
    std::string time_str = time_oss.str();

    // Set clock window dimensions and position (top-right with a 15px margin)
    float clock_width = 180.0f;
    float clock_height = 50.0f;
    float margin = 15.0f;

    ImGui::SetNextWindowPos(ImVec2(screen_width - clock_width - margin, margin), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(clock_width, clock_height), ImGuiCond_Always);

    ImGuiWindowFlags clock_flags = ImGuiWindowFlags_NoDecoration |
                                   ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_NoFocusOnAppearing |
                                   ImGuiWindowFlags_NoNav;

    // Style configuration: Translucent dark background and rounded corners
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.6f)); // Translucent black
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 12.0f));

    ImGui::Begin("TimeClockWindow", nullptr, clock_flags);

    // Center the text inside the clock box
    float text_width = ImGui::CalcTextSize(time_str.c_str()).x;
    ImGui::SetCursorPosX((clock_width - text_width) * 0.5f);
    
    // Render the clock text
    ImGui::Text("%s", time_str.c_str());

    ImGui::End();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}