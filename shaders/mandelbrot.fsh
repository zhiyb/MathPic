#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;
uniform float zoom;
uniform dvec2 move;
out vec4 fragColor;

void main(void)
{
    dvec2 pos = dvec2(position) / double(pow(1.1, zoom)) + move;
    float DIM = 1024.;
    double i = (pos.x + 1.) / 2. * DIM * 8e-9;
    double j = (-pos.y + 1.) / 2. * DIM * 8e-9;

    double a=0,b=0,c,d;
    int n=0;
    while((c = a * a) + (d = b * b) < 4. && n++ < 880.) {
        b = 2. * a * b + j - .645411;
        a = c - d + i + .356888;
    }
    int cR = int(255.*pow((n-80.)/800.,3.));
    int cG = int(255.*pow((n-80.)/800.,.7));
    int cB = int(255.*pow((n-80.)/800.,.5));

    fragColor.r = float(cR % 256) / 256.;
    fragColor.g = float(cG % 256) / 256.;
    fragColor.b = float(cB % 256) / 256.;
    fragColor.a = 1.;
}
