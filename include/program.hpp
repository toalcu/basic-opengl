#pragma once

#include <GL/gl3w.h>

#include "shader.hpp"

class Program
{
public:
    /**
     * @brief Constructs a new program object.
     *
     * @param vs_shader_ The vertex shader
     * @param fs_shader_ The fragment shader
     */
    Program(const Shader& vs_shader_, const Shader& fs_shader_);

    /**
     * @brief Copy constructors are disabled as we only want one program associated with the given
     * fragment and vertex shaders.
     *
     * @param other_ Thee program not to copy
     */
    Program(const Program& other_) = delete;

    /**
     * @brief Ownership of the program is transferred to the current program object.
     *
     * @param other_ The program to move from
     */
    Program(Program&& other_);

    /**
     * @brief Frees up any resources associated with the program.
     */
    ~Program();

    Program& operator=(const Program& other_) = delete;
    Program& operator=(Program&& other_);

    /**
     * @brief Returns true if the program was linked successfully.
     *
     * @return True if the program can be used, false otherwise
     */
    bool is_ok() const;

    /**
     * @brief Returns the program index.
     *
     * @return The program index
     */
    GLuint get_idx() const;

    /**
     * @brief Use the current program.
     */
    void use() const;

private:
    GLuint m_idx;
    bool m_is_ok;
};
