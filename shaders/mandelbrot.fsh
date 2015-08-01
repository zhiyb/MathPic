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

    float x=0, y=0;
    int k;
    for(k=0;k++<256;) {
        float a=x*x-y*y+(i-768.0)/512;
        y=2*x*y+(j-512.0)/512;
        x=a;
        if(x*x+y*y>4)
            break;
    }
    int r = int(log(k)*47.);
    int g = int(log(k)*47.);
    int b = int(128.-log(k)*23.);

    gl_FragColor.r = float(r % 256) / 256.;
    gl_FragColor.g = float(g % 256) / 256.;
    gl_FragColor.b = float(b % 256) / 256.;
}
