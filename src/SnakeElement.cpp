#include "interfaces.h"


SnakeElement::SnakeElement(){}

SnakeElement::SnakeElement(int posX, int posY, int T){
    x = posX;
    y = posY;
    xNorm = x;
    yNorm = y;
    if(T != 0) v = 1.0/T; 
    else v = 0.0;
}

SnakeElement::~SnakeElement(){}

sf::RectangleShape SnakeElement::drawSnakeElement(int unit, int dx, int dy){
    sf::RectangleShape snakeEle(sf::Vector2f(unit,unit));
    snakeEle.setPosition(dx + xNorm*unit, dy + yNorm*unit);
    snakeEle.setFillColor(sf::Color(210, 105, 30));
    return snakeEle;
}

sf::RectangleShape SnakeElement::drawSnakeElement(int unit, int dx, int dy, sf::Color color){
    sf::RectangleShape snakeEle(sf::Vector2f(unit,unit));
    snakeEle.setPosition(dx + xNorm*unit, dy + yNorm*unit);
    snakeEle.setFillColor(color);
    return snakeEle;
}

void SnakeElement::setPosition(int posX, int posY){ 
    x=posX;
    y=posY;
    xNorm=x;
    yNorm=y;
}

void SnakeElement::move(){
    if(currentDirection == NORTH) yNorm -= v;
    else if (currentDirection == EAST) xNorm += v;
    else if (currentDirection == SOUTH) yNorm += v;
    else if (currentDirection == WEST) xNorm -= v;
}
void SnakeElement::confirmPosition(){
    //przesuwam x i y
    if(currentDirection==NORTH) y -= 1;
    else if (currentDirection==EAST) x += 1;
    else if (currentDirection == SOUTH) y += 1;
    else if (currentDirection == WEST) x -= 1;

    //pozbywam sie bledow przyblizen
    xNorm = x;
    yNorm = y;

    //nowy kierunek
    if(nextDirection != NONE){
        currentDirection = nextDirection;
        nextDirection = NONE;
    }
}

void SnakeElement::setNextDirection(Direction dir){
    nextDirection = dir;
}

int SnakeElement::getX(){
    return x;
}

int SnakeElement::getY(){
    return y;
}

Direction SnakeElement::getDirection(){
    return currentDirection;
}

void SnakeElement::setCurrentDirection(Direction dir){
    currentDirection = dir;
}

void SnakeElement::setV(double velocity){
    v = velocity;
}