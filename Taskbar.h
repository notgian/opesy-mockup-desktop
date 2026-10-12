#pragma once

#include <GLFW/glfw3.h>

// show_task_manager / show_notepad are optional so older call sites still compile.
void RenderTaskbar(float screen_width, float screen_height, GLFWwindow* window, bool* show_demo_window, bool* show_gallery_window,
                   bool* show_task_manager = nullptr, bool* show_notepad = nullptr);
