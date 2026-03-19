#include <GL/gl3w.h>
#include <SDL2/SDL.h>

#include <iostream>

auto main(int argc, char* argv[]) -> int
{
    // See https://wiki.libsdl.org/SDL3/SDL_InitFlags#syntax
    SDL_Init(SDL_INIT_VIDEO);

    // See https://wiki.libsdl.org/SDL2/SDL_GLattr
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Enable double buffering (front and back). It defaults to 1 but this is just for documenting
    // https://en.wikipedia.org/wiki/Multiple_buffering#Double_buffering_in_computer_graphics
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // Number of bits in the depth buffer
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* window =
        SDL_CreateWindow("opengl tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280,
                         720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    SDL_GL_MakeCurrent(window, gl_context);

    // This is for vsync. It is the number of vertical blanks before the front and back buffers are
    // swapped. 1 tells the GPU to wait 1 v-blank before swapping front and back i.e. vsync.
    // https://wikis.khronos.org/opengl/Swap_Interval#Adaptive_Vsync
    SDL_GL_SetSwapInterval(1);

    gl3wInit();

    bool run = true;
    while (run)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) run = false;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window))
            {
                run = false;
            }
        }

        // Set the states before drawing for sanity. Setting these every frame is good enough for
        // now See https://wikis.khronos.org/opengl/OpenGL_Object
        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        // See https://registry.khronos.org/OpenGL-Refpages/gl4/html/glViewport.xhtml
        // x,y rel to lower left corner of viewport rect
        glViewport(0, 0, width, height);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// @todo

        // End of the render loop
        // This will swap the window for double buffering, displaying the current contents of the
        // buffer on the screen
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
