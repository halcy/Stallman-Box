CC = gcc
CFLAGS = -g -Wall -O3 -ffast-math -std=gnu99 -lz -Iglew/include
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

ifeq ($(shell uname),Darwin)
	LIBS = -framework OpenGL -framework GLUT
else
	LIBS = -Lglew/lib -lglut -lGLEW -lSDL -lGL -lm
endif

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

test: all
	./$(PROG)

clean:
	rm -f $(PROG)
	rm -r *~
