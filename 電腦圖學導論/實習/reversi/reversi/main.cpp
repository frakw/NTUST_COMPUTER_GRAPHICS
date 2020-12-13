// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


#define MINIAUDIO_STATIC
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "reversi.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
//void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
//{
//    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
//    if (pDecoder == NULL) {
//        return;
//    }
//
//    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);
//
//    (void)pInput;
//}
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_bool32 isLooping = MA_TRUE;

    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    /*
    A decoder is a data source which means you can seemlessly plug it into the ma_data_source API. We can therefore take advantage
    of the "loop" parameter of ma_data_source_read_pcm_frames() to handle looping for us.
    */
    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL, isLooping);

    (void)pInput;
}


void display()
{
    glColor3b(100,100,100);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();

    glFlush();
}
int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "REVERSI GAME", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version); //glsl_version可以使用字符串"#version 150"替代

    bool show_demo_window = true;
    bool show_another_window = false;
    bool UI_adjust = true;
    bool game_window_open = true;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;
    result = ma_decoder_init_file("bounce.wav", NULL, &decoder);
    if (result != MA_SUCCESS) {
        return -2;
    }
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &decoder;
    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }


    ImFont* font = io.Fonts->AddFontFromFileTTF("font/NotoSansHans-Regular.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->GetGlyphRangesChineseSimplifiedCommon();
    Reversi reversi;
    reversi.set_piece_texture("image/Reversi_Black.png","image/Reversi_White.png","image/empty.png");

    //ImDrawList::AddImage();
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImGui::Begin("UI_adjust", &UI_adjust, ImGuiWindowFlags_MenuBar);
            static float  grid_size = 50.0f;
            ImGui::SliderFloat(u8"格子大小", &grid_size, 0.0f, 100.0f);
            ImGui::ShowStyleEditor();
            
            ImGui::End();
            ImGui::SetNextWindowSize(ImVec2(600,600));
            ImGui::Begin("Game Board", &game_window_open, ImGuiWindowFlags_MenuBar);
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                    if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
                    if (ImGui::MenuItem("Close", "Ctrl+W")) { game_window_open = false; }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }


            //static float my_color;
            //// Edit a color (stored as ~4 floats)
            //ImGui::ColorEdit4("Color", &my_color);

            //// Plot some values
            //const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
            //ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

            //// Display contents in a scrolling region
            //ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
            //ImGui::BeginChild("Scrolling");
            //for (int n = 0; n < 50; n++)
            //    ImGui::Text("%04d: Some text", n);
            //ImGui::EndChild();
            reversi.find_available_grid();
            int id = 0;
            ImGui::Text(u8"playerA黑棋 數量:%d", reversi.black_amount());
            ImGui::SameLine();
            ImGui::Text(u8"playerB白棋 數量:%d", reversi.white_amount());
            if (ImGui::Button("pass")) {
                reversi.next_round();
            }
            if (reversi.round_end()) {
                Reversi_Color* winner = reversi.winner();
                if (!winner) {
                    std::cout << "平手" << std::endl;
                }
                else {
                    if (*winner == Reversi_Color::BLACK) {
                        std::cout << "黑棋勝" << std::endl;
                    }
                    else {
                        std::cout << "白棋勝" << std::endl;
                    }
                }
            }
            ImGui::NewLine();
            for (int i = 0;i < reversi.H();i++) {
                for (int j = 0;j < reversi.W();j++) {
                    ImGui::PushID(id++);
                    if (reversi[i][j].is_black()) {
                        ImGui::ImageButton(ImTextureID(reversi.tex_black_id), ImVec2(grid_size, grid_size));
                    }
                    else if (reversi[i][j].is_white()) {
                        ImGui::ImageButton(ImTextureID(reversi.tex_white_id), ImVec2(grid_size, grid_size));
                    }
                    else if(reversi[i][j].available()){
                        if (ImGui::ImageButton(ImTextureID(reversi.tex_empty_id), ImVec2(grid_size, grid_size), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0.5, 0.5, 0.5, 50))) {
                            reversi.set_piece(j,i);
                        }
                    }
                    else {
                        ImGui::ImageButton(ImTextureID(reversi.tex_empty_id), ImVec2(grid_size, grid_size));

                    }
                    ImGui::SameLine();
                    ImGui::PopID();
                }
                ImGui::NewLine();
            }
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        glUseProgram(0);
        display();

    }
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}