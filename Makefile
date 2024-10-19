CC = g++
CXX_STANDARD = -std=c++17  # Replace with -std=c++11, -std=c++14, -std=c++20 as needed
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all:
	$(CC) $(CXX_STANDARD) $(FILENAME).cpp $(LIBS) -o $(basename $(FILENAME))
