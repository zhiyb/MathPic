#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
uniform float animation;
out vec4 fragColor;

uniform dvec2 position;
in vec2 posOffset;

void main(void)
{
    dvec2 pos = position + dvec2(posOffset);
    double i = pos.x;
    double j = pos.y;

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
    int ani = int(animation < 0.5 ? animation * 512 : (1 - animation) * 512);
    int cR = int(k*16 +ani);
    int cG = int(k*8 +ani);
    int cB = int(k +ani);
#endif

    gl_FragColor.r = float(cR % 256) / 256.;
    gl_FragColor.g = float(cG % 256) / 256.;
    gl_FragColor.b = float(cB % 256) / 256.;
    gl_FragColor.a = 1.;
}
