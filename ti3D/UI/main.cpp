#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() 
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui Template", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    // UI State
    bool show_control_panel = true;
    bool show_debug_window = false;
    bool show_about = false;
    ImVec2 mouse_pos;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Main Menu Bar
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New")) { /* Placeholder */ }
                if (ImGui::MenuItem("Open")) { /* Placeholder */ }
                if (ImGui::MenuItem("Save")) { /* Placeholder */ }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) glfwSetWindowShouldClose(window, true);
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Control Panel", NULL, &show_control_panel);
                ImGui::MenuItem("Debug Window", NULL, &show_debug_window);
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("About")) show_about = true;
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // 2. Control Panel (can be moved)
        if (show_control_panel) {
            ImGui::Begin("Control Panel", &show_control_panel);
            
            // Placeholder controls
            static float value = 0.5f;
            ImGui::Text("This is a control panel");
            ImGui::SliderFloat("Dummy Slider", &value, 0.0f, 1.0f);
            
            static bool check = true;
            ImGui::Checkbox("Dummy Checkbox", &check);
            
            if (ImGui::Button("Dummy Button")) {
                std::cout << "Button clicked!" << std::endl;
            }
            
            ImGui::End();
        }

        // 3. Debug Window
        if (show_debug_window) {
            ImGui::Begin("Debug Window", &show_debug_window);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 4. About Window
        if (show_about) {
            ImGui::Begin("About Dear ImGui Template", &show_about, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Dear ImGui Template v1.0");
            ImGui::Separator();
            ImGui::Text("A simple template for building applications with Dear ImGui");
            ImGui::Text("By Your Name");
            if (ImGui::Button("Close")) show_about = false;
            ImGui::End();
        }

        // 5. Status Bar (fixed at bottom)
        mouse_pos = ImGui::GetMousePos();
        ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - 30));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, 30));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 4));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.7f));
        ImGui::Begin("StatusBar", nullptr, 
                     ImGuiWindowFlags_NoDecoration | 
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoInputs);
        
        ImGui::Text("Mouse: (%.1f, %.1f)", mouse_pos.x, mouse_pos.y);
        ImGui::SameLine(io.DisplaySize.x - 150);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        
        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
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