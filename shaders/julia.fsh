#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
out vec4 fragColor;

flat in dvec2 position;
in vec2 posOffset;
flat in float DIM;

double D(double x)
{
    float DIM = 1024.;
    return (x-DIM/2.)/(DIM/2.);
}

void main(void)
{
    dvec2 pos = position + dvec2(posOffset);
    double i = pos.x;
    double j = pos.y;

    double x=D(i),y=D(j),X,Y;
    int n=0;
    while(n++<200&&(X=x*x)+(Y=y*y)<4){x=X-Y+.36237;y=2*x*y+.32;}
    int cR = int(log(n)*256);

    x=D(i);
    y=D(j);
    n=0;
    while(n++<200&&(x*x+y*y)<4){X=x;Y=y;x=X*X-Y*Y+-.7;y=2*X*Y+.27015;}
    int cG = int(log(n)*128);

    x=D(i);
    y=D(j);
    n=0;
    while(n++<600&&(x*x+y*y)<4){X=x;Y=y;x=X*X-Y*Y+.36237;y=2*X*Y+.32;}
    int cB = int(log(n)*128);

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
