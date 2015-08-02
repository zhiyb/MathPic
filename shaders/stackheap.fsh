#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;
uniform float animation;
uniform float zoom;
uniform dvec2 move;
out vec4 fragColor;

double _sq(double x)
{
    return x * x;
}

float sin(double x)
{
    return sin(float(x));
}

void main(void)
{
    dvec2 pos = dvec2(position) / double(pow(1.1, zoom)) + move;
    float DIM = 1024.;
    double i = (pos.x + 1.) / 2. * DIM;
    double j = (-pos.y + 1.) / 2. * DIM;

    double s=3./(j+99.);
    double y=(j+sin((i*i+_sq(j-700.)*5.)/100./DIM +animation*2.*acos(-1) )*35.)*s;
    int cR = (int((i+DIM)*s+y)%2+int((DIM*2-i)*s+y)%2)*127;
    int cG = (int(5*((i+DIM)*s+y))%2+int(5*((DIM*2-i)*s+y))%2)*127;
    int cB = (int(29*((i+DIM)*s+y))%2+int(29*((DIM*2-i)*s+y))%2)*127;

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
