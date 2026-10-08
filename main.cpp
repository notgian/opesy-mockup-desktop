#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "image.h"
#include "TimeClock.h"
#include "Taskbar.h" // Include the new Taskbar component

// Define fixed resolution constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

static void glfw_error_callback(int error, const char* description) {
    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main() {
    std::cout << "Hello World!" << std::endl;
    std::cout << "Let's pretend that this is the bios boot!!" << std::endl;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    // Decide GL+GLSL versions
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Make window non-resizable to enforce fixed resolution
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create window with fixed resolution constants
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ImGui GLFW g++ Example", nullptr, nullptr);
    if (window == nullptr) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load background texture
    GLuint background_texture = 0;
    int bg_width = 0, bg_height = 0;
    bool image_loaded = LoadTextureFromFile("assets/wallpaper.png", &background_texture, &bg_width, &bg_height);

    bool show_demo_window = false; // Start with demo window hidden until an app button is clicked

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // -----------------------------------------------------------------
        // Background Window (Covers the full 1280x720 screen)
        // -----------------------------------------------------------------
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
        
        ImGuiWindowFlags bg_flags = ImGuiWindowFlags_NoDecoration | 
                                    ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | 
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | 
                                    ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        
        // Push a pure black background color for this window
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        ImGui::Begin("BackgroundWindow", nullptr, bg_flags);
        
        // Render background image if loaded, otherwise falls back to solid black
        if (image_loaded) {
            ImGui::Image((ImTextureID)(intptr_t)background_texture, ImVec2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
        }

        ImGui::End();

        ImGui::PopStyleColor(); // Pop WindowBg
        ImGui::PopStyleVar(2);  // Pop WindowRounding and WindowPadding

        // -----------------------------------------------------------------
        // Render Top-Right Real-Time Clock Component
        // -----------------------------------------------------------------
        RenderTimeClock((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

        // -----------------------------------------------------------------
        // Render Bottom Taskbar Component
        // -----------------------------------------------------------------
        RenderTaskbar((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, window, &show_demo_window);

        // -----------------------------------------------------------------
        // Your other ImGui windows/elements go here
        // -----------------------------------------------------------------
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        
        // Clear screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}