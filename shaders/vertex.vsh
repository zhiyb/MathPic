#version 130
in vec2 vertex;
uniform mat4 projection;
out vec2 position;

void main(void)
{
    position = vertex;
    vec4 pos;
    pos.xyzw = vec4(vertex, 1., 1.);
    gl_Position = projection * pos;
}
