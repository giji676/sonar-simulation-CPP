#final:
#	g++ test.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o test


CC = g++
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all:
	$(CC) $(FILENAME).cpp $(LIBS) -o $(basename $(FILENAME))

