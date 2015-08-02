#version 130
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;
uniform float zoom;
uniform vec2 move;

float _sq(float x)
{
    return x * x;
}

float atan2(float y, float x)
{
    if (x >= 0)
        return atan(y / x);
    else if (y >= 0)
        return atan(y / x) + acos(-1);
    else
        return atan(y / x) - acos(-1);
}

void main(void)
{
    vec2 pos = position / pow(1.1, zoom) + move / 1024.;
    float DIM = 1024.;
    float i = (pos.x + 1.) / 2. * DIM;
    float j = (-pos.y + 1.) / 2. * DIM;

    int cR = int(_sq(cos(atan2(j-512,i-512)/2))*255);
    int cG = int(_sq(cos(atan2(j-512,i-512)/2-2*acos(-1)/3))*255);
    int cB = int(_sq(cos(atan2(j-512,i-512)/2+2*acos(-1)/3))*255);

    gl_FragColor.r = float(cR % 256) / 256.;
    gl_FragColor.g = float(cG % 256) / 256.;
    gl_FragColor.b = float(cB % 256) / 256.;
    gl_FragColor.a = 1.;
}
