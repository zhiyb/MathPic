#include <math.h>
#include "algorithm.h"

#define DM1 (DIM-1)
#define _sq(x) ((x)*(x))	// square
#define _cb(x) abs((x)*(x)*(x))	// absolute value of cube
#define _cr(x) (unsigned char)(pow((x),1.0/3.0))	// cube root

uint32_t pixel_write(int i, int j)
{
	uint32_t clr;
	double a = 0, b = 0, c, d, n = 0;
	while ((c = a * a) + (d = b * b) < 4 && n++ < 880) {
		b = 2 * a * b + j * 8e-9 - .645411;
		a = c - d + i * 8e-9 + .356888;
	}
	unsigned char RD = 255 * pow((n - 80) / 800, 3.);
	unsigned char GR = 255 * pow((n - 80) / 800, .7);
	unsigned char BL = 255 * pow((n - 80) / 800, .5);
	clr = (uint32_t)RD;
	clr |= (uint32_t)GR << 8;
	clr |= (uint32_t)BL << 16;
	return clr;
}
