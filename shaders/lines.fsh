#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
out vec4 fragColor;

uniform dvec2 position;
in vec2 posOffset;

void main(void)
{
    dvec2 pos = position + dvec2(posOffset);
    double i = pos.x;
    double j = pos.y;

    int cR = int((i+j)*2.);
    int cG = int((i+j)*8.);
    int cB = int((i+j)*16.);

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
