#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
uniform float animation;
out vec4 fragColor;

flat in dvec2 position;
in vec2 posOffset;
in float DIM;

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
    dvec2 pos = position + dvec2(posOffset);
    double i = pos.x;
    double j = pos.y;

    int cR = int(_sq(cos(float(atan2(j-512,i-512)/2 +animation*2.*acos(-1) )))*255);
    int cG = int(_sq(cos(float(atan2(j-512,i-512)/2-2*acos(-1)/3 +animation*2.*acos(-1) )))*255);
    int cB = int(_sq(cos(float(atan2(j-512,i-512)/2+2*acos(-1)/3 +animation*2.*acos(-1) )))*255);

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
