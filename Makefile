CC = clang
CFLAGS = -W -Wall -std=c99 `sdl-config --cflags`

PROJ = cards
OBJS = \
	main.o \
	graphutils.o \
	map.o \
	text.o

LIBS = `sdl-config --libs`

all: $(PROJ)

${PROJ}: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

run: all
	./$(PROJ)

clean:
	rm -f *.o

distclean: clean
	rm -f $(PROJ)

