CC=gcc
CFLAGS=-Wall -g
TARGET=main.o

all: $(TARGET)

$(TARGET): main.c functions.c structure.h
	$(CC) $(CFLAGS) main.c functions.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET) cars.txt

rebuild: clean all run
