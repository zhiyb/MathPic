attribute vec2 vertex;
uniform float zoom;
uniform vec2 move;
varying vec2 position;

void main(void)
{
    position = (vertex + move / 1024.) / pow(1.1, zoom);
    gl_Position.xy = vertex;
    gl_Position.z = 0;
    gl_Position.w = 1;
}
