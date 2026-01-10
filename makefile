CC=gcc
CFLAGS=-Wall -std=c99
LIBS= -L/opt/homebrew/lib -I/opt/homebrew/include -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

all:
	$(CC) src/*.c -o deliveryman $(CFLAGS) $(LIBS)