#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
uniform float animation;
out vec4 fragColor;

uniform uint DIM;
uniform dvec2 position;
in vec2 posOffset;

double _sq(double x)
{
    return x * x;
}

void main(void)
{
    dvec2 pos = position + dvec2(posOffset);
    double i = pos.x;
    double j = pos.y;

    double s=3./(j+99.);
    double y=(j+sin(float((i*i+_sq(j-700.)*5.)/100./DIM +animation*2.*acos(-1)))*35.)*s;
    int cR = (int((i+DIM)*s+y)%2+int((DIM*2-i)*s+y)%2)*127;
    int cG = (int(5*((i+DIM)*s+y))%2+int(5*((DIM*2-i)*s+y))%2)*127;
    int cB = (int(29*((i+DIM)*s+y))%2+int(29*((DIM*2-i)*s+y))%2)*127;

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
