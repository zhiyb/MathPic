#version 130
#extension GL_ARB_gpu_shader_fp64 : enable
out vec4 fragColor;

flat in dvec2 position;
in vec2 posOffset;
flat in float DIM;

float hypot(float x, float y)
{
    float ydx = y / x;
    return abs(x) * sqrt(1 + ydx * ydx);
}

int RD(float i, float j)
{
    float r=i/256.-2,s=j/256.-2,q=r*r+s*s,n=hypot(r+(.866-r/2)/q,s+(r*.866+s/2)/q),
    d=.5/log(n);if(d<0||d>1)d=1;return int(d*(sin(n*10)*511+512)) / 4;
}

void main(void)
{
    vec2 pos = vec2(position + dvec2(posOffset));
    float i = pos.x;
    float j = pos.y;

    int cR = RD(i, j);
    int cG = 0;
    float r=i/256.-2,s=j/256.-2,q=r*r+s*s;
    int cB = int(RD(i,j)*sqrt(q/40));

    gl_FragColor.r = float(cR % 256) / 256.;
    gl_FragColor.g = float(cG % 256) / 256.;
    gl_FragColor.b = float(cB % 256) / 256.;
    gl_FragColor.a = 1.;
}
