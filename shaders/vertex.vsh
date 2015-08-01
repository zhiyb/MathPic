attribute vec2 vertex;
uniform float zoom;
uniform vec2 move;
varying vec2 position;

void main(void)
{
    position = vertex / pow(1.1, zoom) + move / 1024.;
    gl_Position.xy = vertex;
    gl_Position.z = 0;
    gl_Position.w = 1;
}
