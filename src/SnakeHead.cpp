#include "interfaces.h"



SnakeHead::SnakeHead(int posX, int posY, int T){
        x = posX;
        y = posY;
        xNorm = x;
        yNorm = y;
        v = 1.0/T; 

        tail = new SnakeElement[1];

        //ogon poczatkowy
        snakeGrow();
    }

    SnakeHead::SnakeHead(){}


    void SnakeHead::addTailElement(){
        //tworze nowy ogon o zwiekszonej dlugosci
        SnakeElement* newTail = new SnakeElement[size+1];

        if(size > 0){
            //przepisuje stary ogon
            for(int i=0;i<size;i++){
                newTail[i] = tail[i];
            }

            //tworze nowy element ogona
            newTail[size] = *newTailElement;
            newTail[size].setNextDirection(tail[size-1].getDirection());
            newTail[size].setV(v);


            //podmieniam ogony
            delete[] tail;
        }
        else{
            newTail[0] = *newTailElement;
            newTail[0].setNextDirection(currentDirection);
            newTail[0].setV(v);
            
        }

        //NULLuje wskaznik na nowy element
        newTailElement = NULL;

        //inkrementuje dlugosc ogona
        size++;
        
        //przypinam nowy ogon
        tail = newTail;
        
    }

    void SnakeHead::snakeGrow(){
        //zbieram informacje o ostatnim elemencie
        int xPos = x;
        int yPos = y;
        if(size > 0){
            SnakeElement lastTailElement = tail[size-1];
            xPos = lastTailElement.getX();
            yPos = lastTailElement.getY();
        }
        
        //tworze nowy element
        newTailElement = new SnakeElement(xPos, yPos, 0);
        newTailElement->setCurrentDirection(NONE);

    }

    sf::RectangleShape* SnakeHead::drawSnake(int unit, int dx, int dy){
        sf::RectangleShape* result = new sf::RectangleShape[getVisualSize()+1];

        //ogon
        for(int i=0;i<size;i++){
            result[i] = tail[i].drawSnakeElement(unit, dx, dy);
        }

        //zagiecia
        int j = 0;
        if(currentDirection != tail[0].getDirection()){
            result[size+j+1] = SnakeElement(x, y, 0).drawSnakeElement(unit, dx, dy);
            j++;
        }
        for(int i=1;i<size;i++){
            if(tail[i-1].getDirection() != tail[i].getDirection()){
                result[size+j+1] = SnakeElement(tail[i-1].getX(), tail[i-1].getY(), 0).drawSnakeElement(unit, dx, dy);
                j++;
            }
        }

        //ewentualny nowy element
        if(newTailElement != NULL) result[getVisualSize()-1] = newTailElement->drawSnakeElement(unit, dx, dy);

        //glowa
        result[getVisualSize()] = SnakeElement::drawSnakeElement(unit, dx, dy, sf::Color(139,69,19));

        //returnuje wynik
        return result;
    }

    void SnakeHead::move(){
        //glowa
        SnakeElement::move();

        //ogon
        for(int i=0;i<size;i++){
            tail[i].move();
        }
    }

    void SnakeHead::confirmPosition()
    {
        //dodaje ew nowy element do ogona
        if(newTailElement != NULL) addTailElement();

        //ogarniam ogon
        tail[0].setNextDirection(currentDirection);
        for(int i=1;i<size;i++){
            tail[i].setNextDirection(tail[i-1].getDirection());
        }
        for(int i=0;i<size;i++){
            tail[i].confirmPosition();
        }

        

        //podstawowa wersja confirmposition
        SnakeElement::confirmPosition();

    }


    int SnakeHead::getVisualSize(){
        //rozmiar i glowa
        int result = size+1;
        //zgiecia
        if(currentDirection != tail[0].getDirection()) result++;
        for(int i=1;i<size;i++){
            if(tail[i-1].getDirection() != tail[i].getDirection()) result++;
        }
        //jezeli jest nowy element
        if(newTailElement != NULL) return result+1;
        //jezeli nie ma nowego elelemntu niedodanego do ogona
        return result;
    }

    bool SnakeHead::isOccupiedByTail(int xPos, int yPos){
        //ogon
        for(int i=0;i<size;i++) if(tail[i].getX() == xPos && tail[i].getY() == yPos) return true;
        //chce isc w przeciwna strone niz aktualnie ide
        if(tail[0].getDirection() == NORTH && currentDirection == SOUTH) return true;
        if(tail[0].getDirection() == EAST && currentDirection == WEST) return true;
        if(tail[0].getDirection() == SOUTH && currentDirection == NORTH) return true;
        if(tail[0].getDirection() == WEST && currentDirection == EAST) return true;
        //jak nic nie przeszkadza
        return false;
    }

    void SnakeHead::freeSnake(){
        //usuwam nowy element jesli jest
        if(newTailElement != NULL) delete newTailElement;
        //usuwam ogon
        delete[] tail;
    }