#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
uniform float animation;
out vec4 fragColor;

uniform uint DIM;
uniform dvec2 position;
in vec2 posOffset;

double atan2(double y, double x)
{
    if (x >= 0)
        return atan(float(y / x));
    else if (y >= 0)
        return atan(float(y / x)) + acos(-1);
    else
        return atan(float(y / x)) - acos(-1);
}

// cube root
double _cr(double x)
{
    return pow(float(x), 1.0/3.0);
}

int BL(double i, double j){
    i-=512.;j-=512.;
    double d=sqrt(i*i+j*j);
    return int(d+atan2(j,i)*82.+sin(float(_cr(d*d)))*32.+sin(float(atan2(j,i)*10.))*64. + 256) % 256;
}

void main(void)
{
    dvec2 pos = position + dvec2(posOffset);
    double i = pos.x;
    double j = pos.y;

    int cR = int(DIM-BL(2 * i, 2 * j)+animation * 256.);
    int cG = int(BL(j, i)+128+animation * 256.);
    int cB = int(BL(i, j)+animation * 256.);

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
