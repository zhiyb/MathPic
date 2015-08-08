#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 vertex;
uniform mat4 projection;

uniform float zoom;
uniform uint DIM;
out vec2 posOffset;

void main(void)
{
    posOffset = vec2(1, -1) * vertex * float(DIM / 2) * pow(2, zoom);

    vec4 pos;
    pos.xyzw = vec4(vertex, 0., 1.);
    gl_Position = projection * pos;
}
