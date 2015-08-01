varying vec2 position;

float _sq(float x)
{
    return x * x;
}

void main(void)
{
    float DIM = 19200;
    float i = (position.x + 1.) / 2. * DIM;
    float j = (-position.y + 1.) / 2. * DIM;

    float s=3./(j+99.);
    float y=(j+sin((i*i+_sq(j-700.)*5.)/100./DIM)*35.)*s;
    int r = (int((i+DIM)*s+y)%2+int((DIM*2-i)*s+y)%2)*127;
    int g = (int(5*((i+DIM)*s+y))%2+int(5*((DIM*2-i)*s+y))%2)*127;
    int b = (int(29*((i+DIM)*s+y))%2+int(29*((DIM*2-i)*s+y))%2)*127;

    gl_FragColor.r = float(r % 256) / 256.;
    gl_FragColor.g = float(g % 256) / 256.;
    gl_FragColor.b = float(b % 256) / 256.;
}
