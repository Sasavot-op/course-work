CC=gcc
CFLAGS=-Wall -g
TARGET=main

all: $(TARGET)

$(TARGET): main.c functions.c util.c
	$(CC) $(CFLAGS) main.c functions.c util.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) cars.db

rebuild: clean all
