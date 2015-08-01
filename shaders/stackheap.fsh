#version 130
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;
uniform float zoom;
uniform vec2 move;

float _sq(float x)
{
    return x * x;
}

void main(void)
{
    vec2 pos = position / pow(1.1, zoom) + move / 1024.;
    float DIM = 10240.;
    float i = (pos.x + 1.) / 2. * DIM;
    float j = (-pos.y + 1.) / 2. * DIM;

    float s=3./(j+99.);
    float y=(j+sin((i*i+_sq(j-700.)*5.)/100./DIM)*35.)*s;
    int r = (int((i+DIM)*s+y)%2+int((DIM*2-i)*s+y)%2)*127;
    int g = (int(5*((i+DIM)*s+y))%2+int(5*((DIM*2-i)*s+y))%2)*127;
    int b = (int(29*((i+DIM)*s+y))%2+int(29*((DIM*2-i)*s+y))%2)*127;

    gl_FragColor.r = float(r % 256) / 256.;
    gl_FragColor.g = float(g % 256) / 256.;
    gl_FragColor.b = float(b % 256) / 256.;
}
