#pragma once

#include <GL/gl3w.h>

#include <cstdint>
#include <string>

class Shader
{
public:
    enum class Type : uint8_t
    {
        Vertex = 0,
        Fragment,
        Num
    };

    /**
     * @brief Constructs a new shader object.
     *
     * @param type_ The type of shader
     * @param filename_ The shader source. The filename should not include the .glsl extension
     */
    Shader(const Type type_, const std::string& filename_);

    /**
     * @brief Copy constructors are disabled as we only want one shader associated with the current
     * source
     *
     * @param other_ The shader not to copy
     */
    Shader(const Shader& other_) = delete;

    /**
     * @brief Move constructors are enabled and ownership of the shader is transferred to the
     * current shader object
     *
     * @param other_ The shader to move from
     */
    Shader(Shader&& other_);

    /**
     * @brief Free up resources associated with the shader
     */
    ~Shader();

    Shader& operator=(const Shader& other_) = delete;
    Shader& operator=(Shader&& other_);

    /**
     * @brief Returns the shader index.
     *
     * @return The shader index.
     */
    GLuint get_idx() const;

private:
    GLuint m_idx;
    Type m_type;

    std::string read_from_file(const std::string& path_);
    bool check_compilation_succeeded() const;
};
