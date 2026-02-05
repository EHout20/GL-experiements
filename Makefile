CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Wall -Iinclude
CFLAGS = -Iinclude
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

TARGET = opengl_app
CPP_FILES = main.cpp src/Shader.cpp src/Shape.cpp src/Transform.cpp

all: $(TARGET)

$(TARGET): src/glad.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(CPP_FILES) src/glad.o $(LDFLAGS)

src/glad.o: src/glad.c
	$(CC) $(CFLAGS) -c src/glad.c -o src/glad.o

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
