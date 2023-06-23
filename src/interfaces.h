#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include<fstream>
#include<cstdlib>


enum Direction{
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4,
    NONE=5
};



class Apple{
    private:
        int x;
        int y;
    public:
        Apple();
        Apple(int xPos, int yPos);
        ~Apple();
        sf::CircleShape drawApple(int unit, int dx, int dy);
        int getX();
        int getY();
};


class SnakeElement{

    protected:
        int x;
        int y;
        double xNorm;
        double yNorm;
        double v;
        Direction currentDirection = NORTH;
        Direction nextDirection = NONE;
    public:
        SnakeElement();
        SnakeElement(int posX, int posY, int T);
        ~SnakeElement();
        sf::RectangleShape drawSnakeElement(int unit, int dx, int dy);
        sf::RectangleShape drawSnakeElement(int unit, int dx, int dy, sf::Color color);
        void move();
        void setPosition(int posX, int posY);
        void confirmPosition();
        void setNextDirection(Direction dir);
        int getX();
        int getY();
        Direction getDirection();
        void setCurrentDirection(Direction dir);
        void setV(double velocity);
};


class SnakeHead : public SnakeElement {
    private:
        SnakeElement* tail;
        SnakeElement* newTailElement = NULL;
        int size = 0;
        void addTailElement();
    public:
        SnakeHead();
        SnakeHead(int posX, int posY, int T);
        void snakeGrow();
        sf::RectangleShape* drawSnake(int unit, int dx, int dy);
        void move();
        void confirmPosition();
        int getVisualSize();
        bool isOccupiedByTail(int xPos, int yPos);
        void freeSnake();
};