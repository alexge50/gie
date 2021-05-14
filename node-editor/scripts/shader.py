import sys
import os
import re

source_code = """#ifndef SHADER_{shader_name}
#define SHADER_{shader_name}
#include <glad/glad.h>
#include <optional>
#include <string>
#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <stdlib.h>

namespace Shaders {{
using namespace glm;
class {shader_name}
{{
    static constexpr const char* vertex_source = "{vertex_source}";
    static constexpr const char* fragment_source = "{fragment_source}";

public:
    using sampler2D = int;

    struct UniformData
    {{
{uniform_data}
    }};

    {shader_name}() 
    {{
        auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
        glCompileShader(vertex_shader);
    
        auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
        glCompileShader(fragment_shader);
    
        auto shaders = {{
                vertex_shader,
                fragment_shader
        }};
    
        for(auto shader: shaders)
        {{
            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if(isCompiled == GL_FALSE)
            {{
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    
                std::vector<char> errorLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
    
                std::cerr << "{shader_name}:" << std::endl;
                std::cerr << &errorLog[0] << std::endl;
    
                glDeleteShader(shader);
                exit(-1);
            }}
        }}
    
    
        program_id = glCreateProgram();
        glAttachShader(program_id, vertex_shader);
        glAttachShader(program_id, fragment_shader);
        glLinkProgram(program_id);
        
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
{uniform_get_location}
    }}

    ~{shader_name}()
    {{
        glDeleteProgram(program_id);
    }}

    void prepare(const UniformData& uniform_data)
    {{
        glUseProgram(program_id);
{uniform_setup}
    }}
    
private:
    unsigned int program_id = 0;    
    
    struct UniformLocation 
    {{
{uniform_location}
    }} location;
}};
}};
#endif
"""

assert len(sys.argv) == 4

output_filename = sys.argv[3]

with open(sys.argv[1], 'r') as f:
    vertex_source = f.read()

with open(sys.argv[2], 'r') as f:
    fragment_source = f.read()

vertex_matches = re.findall(r'uniform\s+(?P<type>\w+)\s+(?P<name>\w+)', vertex_source)
fragment_matches = re.findall(r'uniform\s+(?P<type>\w+)\s+(?P<name>\w+)', fragment_source)

uniform_variables = vertex_matches + fragment_matches  # [(type, name)]

indent = '        '

uniform_location = '\n'.join(
    [indent + f'unsigned int {name};' for _, name in uniform_variables]
)

uniform_data = '\n'.join(
    [indent + f'{type_} {name};' for type_, name in uniform_variables]
)


def generate_setup_line(uniform_variable):
    type_, name = uniform_variable

    if type_ in ['mat2', 'mat3', 'mat4']:
        size = type_[3]  # 4th character
        return f"glUniformMatrix{size}fv(location.{name}, 1, false, glm::value_ptr(uniform_data.{name}))"
    elif type_ in ['vec2', 'vec3', 'vec4']:
        size = type_[3]  # 4th character
        return f"glUniform{size}fv(location.{name}, 1, glm::value_ptr(uniform_data.{name}))"
    elif type_ == 'float':
        return f"glUniform1f(location.{name}, uniform_data.{name})"
    elif type_ == 'sampler2D':
        return f"glUniform1i(location.{name}, uniform_data.{name})"


uniform_setup = '\n'.join(
    [indent + generate_setup_line(var) + ';' for var in uniform_variables]
)

uniform_get_location = '\n'.join(
    [indent + f'location.{name} = glGetUniformLocation(program_id, "{name}");' for _, name in uniform_variables]
)

_, vertex_filename = os.path.split(sys.argv[1])
_, fragment_filename = os.path.split(sys.argv[2])

vertex_name = vertex_filename.split('.')[0]
fragment_name = fragment_filename.split('.')[0]

assert vertex_name == fragment_name

with open(output_filename, 'w') as f:
    f.write(source_code.format(
        shader_name=vertex_name,
        vertex_source=repr(vertex_source)[1:-1],
        fragment_source=repr(fragment_source)[1:-1],
        uniform_get_location=uniform_get_location,
        uniform_data=uniform_data,
        uniform_setup=uniform_setup,
        uniform_location=uniform_location
    ))
