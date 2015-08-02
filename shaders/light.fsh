#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;
uniform float zoom;
uniform dvec2 move;
out vec4 fragColor;

float _sq(float x)
{
    return x * x;
}

void main(void)
{
    dvec2 pos = dvec2(position) / double(pow(1.1, zoom)) + move;
    float DIM = 1024.;
    int i = int((pos.x + 1.) / 2. * DIM);
    int j = int((-pos.y + 1.) / 2. * DIM);

    float w=sqrt(_sq(32.-i%64)+_sq(32.-j%64))/91,x=sqrt(_sq(i-512)+_sq(j-512))/724;
    int cR = int(((2048-i-j)*w/2048*x-w*x+1)*255.);
    int cG = int(((1024-i+j)*w/2048*x-w*x+1)*255.);
    int cB = int(((1024+i-j)*w/2048*x-w*x+1)*255.);

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
