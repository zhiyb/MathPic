#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
out vec4 fragColor;

flat in dvec2 position;
in vec2 posOffset;
flat in int DIM;

void main(void)
{
    dvec2 pos = position + dvec2(posOffset);
    double i = pos.x * 8e-9 + .356888;
    double j = pos.y * 8e-9 - .645411;

    double a = 0, b = 0, c, d;
    int n = 0;
    while((c = a * a) + (d = b * b) < 4. && n++ < 880.) {
        b = 2. * a * b + j;
        a = c - d + i;
    }

    fragColor.r = pow((n-80.)/800.,3.);
    fragColor.g = pow((n-80.)/800.,.7);
    fragColor.b = pow((n-80.)/800.,.5);
    fragColor.a = 1.;
}
