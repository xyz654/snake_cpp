main:
	g++ -c main.cpp Apple.cpp SnakeElement.cpp SnakeHead.cpp -I$(CURDIR)/SFML/sfml/include
	g++ Apple.o -o Apple SnakeElement.o -o SnakeElement SnakeHead.o -o SnakeHead main.o -o main -L$(CURDIR)/SFML/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system


clean:
	rm -f main main.o Apple Apple.o SnakeElement.o SnakeElement SnakeHead.o SnakeHead

all:
	make clean
	make main



