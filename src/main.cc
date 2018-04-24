/******************************************************************************
 * @file: main.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-04-23
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include <cstdio>
#include <external/GL/gl3w.h>
#include <SDL.h>

#include <imgui/imgui.h>
#include <external/imgui_impl_sdl_gl3.h>


// TODO(Cristian): Move this to platform
#ifdef _WIN32
#include "Windows.h"
#endif

SDL_Window *SetupSDL() {
  // Setup window
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  SDL_Window *window = SDL_CreateWindow("Picasso", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        1280, 720,
                                        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  return window;
}

int main(int, char**) {
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "SDL_Init Error: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SetupSDL();
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  (void)(gl_context);
  SDL_GL_SetSwapInterval(1);  // Enable vsync
  gl3wInit();

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  // Mainly sets up the HDC and SDL Keyboard/Mouse stuf
  ImGui_ImplSdlGL3_Init(window);

#ifdef _WIN32
  // TODO(Cristian): For some reason, we need this for stdout logging in windows
  // TODO(Cristian): Move this to the platform layer
  fflush(stderr);

  // Maximize the window
  ShowWindow((HWND)io.ImeWindowHandle, SW_MAXIMIZE);
#endif

  fprintf(stderr, "OpenGL Vendor: %s", glGetString(GL_VENDOR));
  fprintf(stderr, "OpenGL Renderer: %s", glGetString(GL_RENDERER));
  fprintf(stderr, "OpenGL Version: %s", glGetString(GL_VERSION));
  fprintf(stderr, "OpenGL Shading Language Version: %s",
              glGetString(GL_SHADING_LANGUAGE_VERSION));
  fprintf(stderr, "OpenGL Extension: %s", glGetString(GL_EXTENSIONS));


  bool done = false;
  while (!done) {
      // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
      // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
      // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
      // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
          ImGui_ImplSdlGL3_ProcessEvent(&event);
          if (event.type == SDL_QUIT)
              done = true;
      }
      ImGui_ImplSdlGL3_NewFrame(window);

			if (io.KeysDown[io.KeyMap[ImGuiKey_Escape]]) {
				done = true;
			}


      static bool show_demo = true;
      ImGui::ShowDemoWindow(&show_demo);

      glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
      glClear(GL_COLOR_BUFFER_BIT);


      ImGui::Render();
      ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
      SDL_GL_SwapWindow(window);
  }


  return 0;
}


