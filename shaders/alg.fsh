#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
uniform float animation;
out vec4 fragColor;

flat in dvec2 position;
in vec2 posOffset;
flat in float DIM;

float _sq(float x)
{
    return x * x;
}

float atan2(float y, float x)
{
    if (x >= 0)
        return atan(y / x);
    else if (y >= 0)
        return atan(y / x) + acos(-1.);
    else
        return atan(y / x) - acos(-1.);
}

double atan2(double y, double x)
{
    if (x >= 0)
        return atan(float(y / x));
    else if (y >= 0)
        return atan(float(y / x)) + acos(-1.);
    else
        return atan(float(y / x)) - acos(-1.);
}

void main(void)
{
    vec2 pos = vec2(position) + posOffset;
    float i = pos.x;
    float j = pos.y;

    int cR = int(_sq(cos(float(atan2(j-512,i-512)/2 +animation*2.*acos(-1) )))*255);
    int cG = int(_sq(cos(float(atan2(j-512,i-512)/2-2*acos(-1)/3 +animation*2.*acos(-1) )))*255);
    int cB = int(_sq(cos(float(atan2(j-512,i-512)/2+2*acos(-1)/3 +animation*2.*acos(-1) )))*255);

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
