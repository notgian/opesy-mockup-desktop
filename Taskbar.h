#pragma once

#include <GLFW/glfw3.h>

// Renders the bottom taskbar with "PWR" and "App 1" - "App 3" buttons
// show_task_manager / show_notepad are optional so older call sites still compile.
void RenderTaskbar(float screen_width, float screen_height, GLFWwindow* window, bool* show_demo_window,
                   bool* show_task_manager = nullptr, bool* show_notepad = nullptr);