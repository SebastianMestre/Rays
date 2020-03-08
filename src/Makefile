
CFLAGS=-O2
LIBS=-lm

rays: main.o geometry.o intersection.o ray.o vectors.o bmp.o sampling.o random.o
	cc -o $@ $^ $(LIBS)

main.o: main.c bmp.h geometry.h vectors.h intersection.h sampling.h random.h

sampling.o: sampling.c sampling.h vectors.h
sampling.h: vectors.h

bmp.o: bmp.c bmp.h
bmp.h:

geometry.o: geometry.c geometry.h
geometry.h: vectors.h

intersection.o: intersection.c intersection.h
intersection.h: geometry.h ray.h vectors.h

vectors.o: vectors.c vectors.h
vectors.h:

ray.o: ray.c ray.h
ray.h: vectors.h

random.o: random.c random.h
random.h:
