#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;
uniform float zoom;
uniform dvec2 move;
uniform float animation;
out vec4 fragColor;

double atan2(double y, double x)
{
    if (x >= 0)
        return atan(float(y / x));
    else if (y >= 0)
        return atan(float(y / x)) + acos(-1);
    else
        return atan(float(y / x)) - acos(-1);
}

float sin(double x)
{
    return sin(float(x));
}

float pow(double x, double y)
{
    return pow(float(x), float(y));
}

// cube root
double _cr(double x)
{
    return pow(x,1.0/3.0);
}

int BL(double i, double j){
    i-=512.;j-=512.;
    double d=sqrt(i*i+j*j);
    return int(d+atan2(j,i)*82.+sin(_cr(d*d))*32.+sin(atan2(j,i)*10.)*64. + 256) % 256;
}

void main(void)
{
    dvec2 pos = dvec2(position) / double(pow(1.1, zoom)) + move;
    float DIM = 1024.;
    double i = (pos.x + 1.) / 2. * DIM;
    double j = (-pos.y + 1.) / 2. * DIM;

    int cR = int(DIM-BL(2 * i, 2 * j)+animation * 256.);
    int cG = int(BL(j, i)+128+animation * 256.);
    int cB = int(BL(i, j)+animation * 256.);

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
