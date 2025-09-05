CC = gcc
CFLAGS = -Wall -g `sdl2-config --cflags`
LDFLAGS = -lm `sdl2-config --libs`
TARGET = space

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
