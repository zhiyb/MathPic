#version 130
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;
uniform float zoom;
uniform vec2 move;

void main(void)
{
    vec2 pos = position / pow(1.1, zoom) + move / 1024.;
    float DIM = 1024.;
    float i = (pos.x + 1.) / 2. * DIM;
    float j = (-pos.y + 1.) / 2. * DIM;

    int r = int((i+j)*2.);
    int g = int((i+j)*8.);
    int b = int((i+j)*16.);

    gl_FragColor.r = float(r % 256) / 256.;
    gl_FragColor.g = float(g % 256) / 256.;
    gl_FragColor.b = float(b % 256) / 256.;
}
