TRG	= MathPic
SRC	= main.c mandelbrot.c
THREADS	= 4

CFLAG	+= -std=c99 -Wall
LIBS	+= -lm -lpthread

include Makefile_generic.defs

run: $(TRG)
	./$(TRG) $(THREADS) | ppmtobmp | bmptopnm | pnmtopng > MathPic.png
