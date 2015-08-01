#version 130
in vec2 vertex;

out vec2 position;

void main(void)
{
    position = vertex;
    gl_Position.xy = vertex;
    gl_Position.z = 0.;
    gl_Position.w = 1.;
}
