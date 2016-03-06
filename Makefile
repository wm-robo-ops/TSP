all: main.cpp
	g++ -g -Wall -o TSP main.cpp

clean:
	$(RM) TSP
