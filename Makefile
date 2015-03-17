CC = clang
CFLAGS = -save-temps -g -W -Wall -std=c99 `sdl-config --cflags`

PROJ = cards
OBJS = \
	main.o \
	actor.o \
	duel.o \
	event.o \
	graphutils.o \
	map.o \
	inventory.o \
	scene.o \
	projectile.o \
	item.o \
	text.o

LIBS = `sdl-config --libs` -lSDL_gfx

all: $(PROJ)

${PROJ}: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

run: all
	./$(PROJ)

clean:
	rm -f *.[ois]

distclean: clean
	rm -f $(PROJ)

