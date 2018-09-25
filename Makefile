CC = gcc
CFLAGS = -g -Wall -O1 -ffast-math -std=gnu99
PROG = stallmanbox

SRCS = \
	main.c \
	itoa.c \
	gameDraw.c \
	matrices.c \
	scalars.c \
	vectors.c \
	vertices.c \
	glTools.c \
	glDebug.c \
	PNG.c \
	PNGTextureLoader.c \
	sprite.c \
	spriteEngine.c \
	Audio.c \
	AudioSource.c \
	VorbisPlayer.c \

LIBS = -lz -lm
ifeq ($(shell uname),Darwin)
	LIBS += -framework OpenGL -framework GLUT
else
	LIBS += -lglut -lGLEW -lGL -lSDL
endif

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

test: all
	./$(PROG)

clean:
	rm -vf $(PROG)
	rm -vfr *~

