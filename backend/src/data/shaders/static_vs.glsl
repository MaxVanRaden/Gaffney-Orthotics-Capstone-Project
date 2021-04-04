#version 330 core

in vec3 position;
in vec3 normal;
in vec2 uv;

uniform mat4 projection = mat4(1.0);
uniform mat4 transform = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 lightspace = mat4(1.0);

out vec2 pass_uv;
out vec3 pass_normal;
out vec3 pass_pos;
out vec4 pass_lightspace;

//
//   MAIN
//

void main() {
    pass_pos = vec3(transform * vec4(position, 1.0));
    pass_normal = transpose(inverse(mat3(transform))) * normal;
    pass_uv = uv;
    pass_lightspace = lightspace * vec4(pass_pos, 1.0);
    gl_Position = projection * view * transform * vec4(position, 1.0);
}