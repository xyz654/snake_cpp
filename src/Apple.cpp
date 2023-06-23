#include "interfaces.h"

Apple::Apple(){}
    
Apple::Apple(int xPos, int yPos){
    x = xPos;
    y = yPos;
}

Apple::~Apple(){}

sf::CircleShape Apple::drawApple(int unit, int dx, int dy){
    sf::CircleShape appleElement(unit/3);
    appleElement.setPosition(dx + x*unit + unit/6, dy + y*unit + unit/6);   //unit/6 centruje kolko na polu
    appleElement.setFillColor(sf::Color(255,0,0));
    appleElement.setOutlineColor(sf::Color(139,0,0));
    appleElement.setOutlineThickness(2.0f);
    return appleElement;
}
    
int Apple::getX(){
    return x;
}

int Apple::getY(){
    return y;
}