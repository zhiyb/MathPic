#version 130
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;
uniform float zoom;
uniform vec2 move;

float atan2(float y, float x)
{
    if (x >= 0)
        return atan(y / x);
    else if (y >= 0)
        return atan(y / x) + 2*acos(0);
    else
        return atan(y / x) - 2*acos(0);
}

// cube root
float _cr(float x)
{
    return pow(x,1.0/3.0);
}

int BL(float i, float j){
    i-=512.;j-=512.;
    float d=sqrt(i*i+j*j);
    return int(d+atan2(j,i)*82.+sin(_cr(d*d))*32.+sin(atan2(j,i)*10.)*64. + 256) % 256;
}

void main(void)
{
    dvec2 pos = dvec2(position) / double(pow(1.1, zoom)) + dvec2(move) / 1024.;
    float DIM = 1024.;
    float i = float((pos.x + 1.) / 2. * DIM);
    float j = float((-pos.y + 1.) / 2. * DIM);

    int cR = int(DIM-BL(2 * i, 2 * j));
    int cG = int(BL(j, i)+128);
    int cB = BL(i, j);

    gl_FragColor.r = float(cR % 256) / 256.;
    gl_FragColor.g = float(cG % 256) / 256.;
    gl_FragColor.b = float(cB % 256) / 256.;
    gl_FragColor.a = 1.;
}
