#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
uniform float animation;
out vec4 fragColor;

uniform dvec2 position;
in vec2 posOffset;

float _sq(float x)
{
	return x * x;
}

float atan2(float y, float x)
{
	if (x >= 0.)
		return atan(y / x);
	else if (y >= 0.)
		return atan(y / x) + acos(-1.);
	else
		return atan(y / x) - acos(-1.);
}

void main(void)
{
	vec2 pos = vec2(position) + posOffset;
	float i = pos.x;
	float j = pos.y;

	float ani = mod(animation, 1.);
	float cR = _sq(cos(atan2(j-512.,i-512.)/2. +ani*2.*acos(-1.)));
	float cG = _sq(cos(atan2(j-512.,i-512.)/2.-2.*acos(-1.)/3. +ani*2.*acos(-1.)));
	float cB = _sq(cos(atan2(j-512.,i-512.)/2.+2.*acos(-1.)/3. +ani*2.*acos(-1.)));

	fragColor.r = cR;
	fragColor.g = cG;
	fragColor.b = cB;
	fragColor.a = 1.;
}
