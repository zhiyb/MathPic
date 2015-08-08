#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 vertex;
uniform mat4 projection;

uniform float zoom;
uniform dvec2 move;
flat out dvec2 position;
out vec2 posOffset;
flat out int DIM;

void main(void)
{
    DIM = 1024;
    position = (move + vec2(1, -1)) * (DIM / 2.) * vec2(1, -1);
    posOffset = vertex * pow(2, zoom) * (DIM / 2.) * vec2(1, -1);

    vec4 pos;
    pos.xyzw = vec4(vertex, 1., 1.);
    gl_Position = projection * pos;
}
