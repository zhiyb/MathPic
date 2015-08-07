#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 vertex;
uniform mat4 projection;

uniform float zoom;
uniform dvec2 move;
flat out dvec2 position;
out vec2 posOffset;
out float DIM;

void main(void)
{
    DIM = 1024.;
    position = (vec2(-1., -1.) / pow(1.1, zoom) + move + vec2(1, -1)) / 2. * DIM * vec2(1, -1);
    posOffset = (vertex - vec2(-1, -1)) / (pow(1.1, zoom)) / 2. * DIM * vec2(1, -1);

    vec4 pos;
    pos.xyzw = vec4(vertex, 1., 1.);
    gl_Position = projection * pos;
}
