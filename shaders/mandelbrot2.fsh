#version 130
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;
uniform float zoom;
uniform vec2 move;

void main(void)
{
    dvec2 pos = dvec2(position) / double(pow(1.1, zoom)) + dvec2(move) / 1024.;
    float DIM = 1024.;
    double i = (pos.x + 1.) / 2. * DIM;
    double j = (-pos.y + 1.) / 2. * DIM;

    double x=0, y=0;
    int k;
    for(k=0;k++<256;) {
        double a=x*x-y*y+(i-768.0)/512.;
        y=2.*x*y+(j-512.0)/512.;
        x=a;
        if(x*x+y*y>4.)
            break;
    }
#if 0
    int cR = int(log(k)*47.);
    int cG = int(log(k)*47.);
    int cB = int(128.-log(k)*23.);
#else
    int cR = k*16;
    int cG = k*8;
    int cB = k;
#endif

    gl_FragColor.r = float(cR % 256) / 256.;
    gl_FragColor.g = float(cG % 256) / 256.;
    gl_FragColor.b = float(cB % 256) / 256.;
    gl_FragColor.a = 1.;
}
