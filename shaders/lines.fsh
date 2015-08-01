varying vec2 position;

float _sq(float x)
{
    return x * x;
}

void main(void)
{
    float DIM = 1024;
    float i = (position.x + 1.) / 2. * DIM;
    float j = (-position.y + 1.) / 2. * DIM;

    int r = int((i+j)*2.);
    int g = int((i+j)*8.);
    int b = int((i+j)*16.);

    gl_FragColor.r = float(r % 256) / 256.;
    gl_FragColor.g = float(g % 256) / 256.;
    gl_FragColor.b = float(b % 256) / 256.;
}
