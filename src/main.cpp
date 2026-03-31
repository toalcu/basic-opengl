#include <SDL2/SDL.h>

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "program.hpp"
#include "shader.hpp"

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

    // clang-format off
    float vertices[] = {
        // positions              // colours

        // front face
        -0.5f, -0.5f,  0.5f,      1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,      1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,      1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,      1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,      1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,      1.0f, 0.0f, 0.0f,

        // back face
        -0.5f, -0.5f, -0.5f,      0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,      0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,      0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,      0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,      0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,      0.0f, 1.0f, 0.0f,

        // left face
        -0.5f,  0.5f,  0.5f,      0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,      0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,      0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,      0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,      0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,      0.0f, 0.0f, 1.0f,

        // right face
        0.5f,  0.5f,  0.5f,      1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,      1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,      1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,      1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,      1.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,      1.0f, 1.0f, 0.0f,

        // top face
        -0.5f,  0.5f, -0.5f,      1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,      1.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,      1.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,      1.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,      1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,      1.0f, 0.0f, 1.0f,

        // bottom face
        -0.5f, -0.5f, -0.5f,      0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,      0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,      0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,      0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,      0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,      0.0f, 1.0f, 1.0f
    };
    // clang-format on

    Shader vert = Shader(Shader::Type::Vertex, "cube_vs");
    Shader frag = Shader(Shader::Type::Fragment, "cube_fs");
    Program program = Program(vert, frag);

    if (!program.is_ok())
    {
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    // Vertex array object for storing vertex attribute calls
    GLuint vao = 0;
    // Vertex buffer object for storing vertices in the GPU
    GLuint vbo = 0;

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    // Bind the VAO first, then bind and configure the VBOs
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Configure the currently bound buffer (vbo), and copy the vertices into it. This is why its a
    // "state machine"
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // How to interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // Each vertex attribute takes its data from memory managed by the VBO, and which VBO it takes
    // it from is determined by the VBO currently bound to GL_ARRAY_BUFFER. Similarly 0 here is the
    // location
    glEnableVertexAttribArray(0);

    // Colour attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)SDL_GetTicks() / 1000.0f * glm::radians(50.0f),
                            glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection =
            glm::perspective(glm::radians(45.0f),
                             static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

        unsigned int model_loc = glGetUniformLocation(program.get_idx(), "model");
        unsigned int view_loc = glGetUniformLocation(program.get_idx(), "view");
        unsigned int projection_loc = glGetUniformLocation(program.get_idx(), "projection");

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

        // Bind before drawing
        glBindVertexArray(vao);
        // It's triangles all the way down. They are the simplest polygon that is always planar and
        // always well defined for rasterisation, and any more complex shape can be broken down into
        // them
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // End of the render loop
        // This will swap the window for double buffering, displaying the current contents of the
        // buffer on the screen
        SDL_GL_SwapWindow(window);
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
