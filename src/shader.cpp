#include "shader.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

#include "config.hpp"

static const uint16_t k_max_shader_info_log_size = 1024;
static const auto k_shader_gl_types =
    std::array<int, static_cast<uint8_t>(Shader::Type::Num)>{GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

Shader::Shader(const Type type_, const std::string& filename_) :
    m_idx{GL_INVALID_INDEX}, m_type{type_}, m_is_ok{false}
{
    const std::string shader_path = std::string(SHADER_DIR) + "/" + filename_ + ".glsl";
    std::string shader_source = read_from_file(shader_path);
    const GLchar* gl_shader_source = reinterpret_cast<const GLchar*>(shader_source.data());

    m_idx = glCreateShader(k_shader_gl_types[static_cast<uint8_t>(m_type)]);

    // Compile shader
    glShaderSource(m_idx, 1, &gl_shader_source, nullptr);
    glCompileShader(m_idx);

    GLint success = GL_FALSE;
    glGetShaderiv(m_idx, GL_COMPILE_STATUS, &success);
    m_is_ok = (success == GL_TRUE);
    if (!m_is_ok)
    {
        std::array<char, k_max_shader_info_log_size> error_output;
        glGetShaderInfoLog(m_idx, k_max_shader_info_log_size, nullptr, error_output.data());
        std::cerr << "Shader compilation errors: " << error_output.data() << std::endl;
    }
}

Shader::Shader(Shader&& other_)
{
    m_type = other_.m_type;
    m_idx = other_.m_idx;

    other_.m_idx = GL_INVALID_INDEX;
}

Shader::~Shader()
{
    if (m_idx != GL_INVALID_INDEX)
    {
        glDeleteShader(m_idx);
    }
}

bool Shader::is_ok() const { return m_is_ok; }

GLuint Shader::get_idx() const { return m_idx; }

std::string Shader::read_from_file(const std::string& path_)
{
    std::ifstream file(path_, std::ios::in | std::ios::binary);
    if (!file)
    {
        std::cerr << "Shader " << path_ << " could not be loaded" << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}
