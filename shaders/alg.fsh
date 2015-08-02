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

double atan2(double y, double x)
{
    if (x >= 0)
        return atan(float(y / x));
    else if (y >= 0)
        return atan(float(y / x)) + acos(-1);
    else
        return atan(float(y / x)) - acos(-1);
}

void main(void)
{
    dvec2 pos = dvec2(position) / double(pow(1.1, zoom)) + move;
    float DIM = 1024.;
    double i = (pos.x + 1.) / 2. * DIM;
    double j = (-pos.y + 1.) / 2. * DIM;

    int cR = int(_sq(cos(float(atan2(j-512,i-512)/2)))*255);
    int cG = int(_sq(cos(float(atan2(j-512,i-512)/2-2*acos(-1)/3)))*255);
    int cB = int(_sq(cos(float(atan2(j-512,i-512)/2+2*acos(-1)/3)))*255);

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
