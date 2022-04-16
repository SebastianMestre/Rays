CFLAGS=-O2 -Wall
LIBS=-lm -pthread

BINDIR := ../bin
OBJDIR := ../build

OBJNAMES := \
	main.o \
	geometry.o \
	intersection.o \
	ray.o \
	material.o \
	vectors.o \
	bmp.o \
	sampling.o \
	prng.o

OBJS := $(addprefix $(OBJDIR)/,$(OBJNAMES))

all: $(BINDIR)/rays

clean:
	rm $(BINDIR) -r
	rm $(OBJDIR) -r

$(BINDIR)/rays: $(OBJS) | $(BINDIR)
	cc -o $@ $^ $(LIBS)

$(BINDIR):
	mkdir $(BINDIR)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.h:
	touch $@

#### #### #### #### #### #### #### #### #### #### #### #### #### #### #### ####

$(OBJDIR)/main.o: main.c bmp.h geometry.h vectors.h intersection.h sampling.h prng.h material.h

$(OBJDIR)/sampling.o: sampling.c sampling.h vectors.h
sampling.h: vectors.h

$(OBJDIR)/bmp.o: bmp.c bmp.h
bmp.h:

$(OBJDIR)/geometry.o: geometry.c geometry.h
geometry.h: vectors.h

$(OBJDIR)/intersection.o: intersection.c intersection.h
intersection.h: geometry.h ray.h vectors.h

$(OBJDIR)/vectors.o: vectors.c vectors.h
vectors.h:

$(OBJDIR)/ray.o: ray.c ray.h
ray.h: vectors.h

$(OBJDIR)/prng.o: prng.c prng.h
prng.h:

$(OBJDIR)/material.o: material.c material.h vectors.h
material.h: vectors.h

