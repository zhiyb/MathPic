#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;
uniform float zoom;
uniform dvec2 move;
out vec4 fragColor;

void main(void)
{
    dvec2 pos = dvec2(position) / double(pow(1.1, zoom)) + move;
    float DIM = 1024.;
    double i = (pos.x + 1.) / 2. * DIM;
    double j = (-pos.y + 1.) / 2. * DIM;

    int cR = int((i+j)*2.);
    int cG = int((i+j)*8.);
    int cB = int((i+j)*16.);

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
