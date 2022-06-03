TARGET = mymalloc
SRC    = $(TARGET).c
CC     = gcc
CFLAGS = -I. -g -Wall -Wvla -Werror -fsanitize=address,undefined
DEPS = mymalloc.h
OBJ = mymalloc.o memgrind.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o *.a *.dylib *.dSYM
