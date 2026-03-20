#include "program.hpp"

#include <array>
#include <cassert>
#include <iostream>

static const uint16_t k_max_program_info_log_size = 1024;

Program::Program(const Shader& vs_shader_, const Shader& fs_shader_) :
    m_idx{glCreateProgram()}, m_is_ok{false}
{
    if (!vs_shader_.is_ok() || !fs_shader_.is_ok())
    {
        std::cerr << "Shaders failed to load in program" << std::endl;
        m_is_ok = false;
        return;
    }

    glAttachShader(m_idx, vs_shader_.get_idx());
    glAttachShader(m_idx, fs_shader_.get_idx());
    glLinkProgram(m_idx);

    GLint success = GL_FALSE;
    glGetProgramiv(m_idx, GL_LINK_STATUS, &success);
    m_is_ok = (success == GL_TRUE);
    if (!m_is_ok)
    {
        std::array<char, k_max_program_info_log_size> error_output;
        glGetProgramInfoLog(m_idx, k_max_program_info_log_size, nullptr, error_output.data());
        std::cerr << "Program compilation errors: " << error_output.data() << std::endl;
    }
}

Program::Program(Program&& other_)
{
    m_idx = other_.m_idx;

    other_.m_idx = GL_INVALID_INDEX;
}

Program::~Program()
{
    if (m_idx != GL_INVALID_INDEX)
    {
        glDeleteProgram(m_idx);
    }
}

Program& Program::operator=(Program&& other_)
{
    if (m_idx != GL_INVALID_INDEX)
    {
        glDeleteProgram(m_idx);
    }

    m_idx = other_.m_idx;

    other_.m_idx = GL_INVALID_INDEX;

    return (*this);
}

bool Program::is_ok() const { return m_is_ok; }

GLuint Program::get_idx() const { return m_idx; }

void Program::use() const
{
    assert(m_idx != GL_INVALID_INDEX);
    glUseProgram(m_idx);
}
