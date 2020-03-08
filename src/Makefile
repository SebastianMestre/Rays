CFLAGS=-O2
LIBS=-lm

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
	random.o

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

#### #### #### #### #### #### #### #### #### #### #### #### #### #### #### ####

$(OBJDIR)/main.o: main.c bmp.h geometry.h vectors.h intersection.h sampling.h random.h material.h

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

$(OBJDIR)/random.o: random.c random.h
random.h:

$(OBJDIR)/material.o: material.c material.h vectors.h
material.h:

