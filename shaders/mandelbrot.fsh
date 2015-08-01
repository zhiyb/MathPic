#version 130
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;
uniform float zoom;
uniform vec2 move;

void main(void)
{
    dvec2 pos = dvec2(position) / double(pow(1.1, zoom)) + dvec2(move) / 1024LF;
    float DIM = 1024.;
    double i = (pos.x + 1.) / 2. * DIM;
    double j = (-pos.y + 1.) / 2. * DIM;

#if 0
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
#else
    double a=0,b=0,c,d;
    int n=0;
    while((c=a*a)+(d=b*b)<4.&&n++<880.) {
        b = 2.*a*b+j*8e-9-.645411;
        a = c-d+i*8e-9+.356888;
    }
    int cR = int(255.*pow((n-80.)/800.,3.));
    int cG = int(255.*pow((n-80.)/800.,.7));
    int cB = int(255.*pow((n-80.)/800.,.5));
#endif

    gl_FragColor.r = float(cR % 256) / 256.;
    gl_FragColor.g = float(cG % 256) / 256.;
    gl_FragColor.b = float(cB % 256) / 256.;
    gl_FragColor.a = 1.;
}
