CC = gcc
SRC = rt.c vector.c sphere.c light.c plane.c
TARGET = rt

all:
	$(CC) $(SRC) -lm -o $(TARGET)
png:
	convert img.ppm img.png
clean:
	rm -f *.o $(TARGET)
