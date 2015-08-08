#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 vertex;
uniform mat4 projection;

uniform float zoom;
uniform uint DIM;
out vec2 posOffset;

void main(void)
{
    posOffset = vertex * pow(2, zoom) * (DIM / 2.) * vec2(1, -1);

    vec4 pos;
    pos.xyzw = vec4(vertex, 1., 1.);
    gl_Position = projection * pos;
}
