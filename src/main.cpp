#include "interfaces.h"


class Game{
    private:
            
        //zmienne mapy
        int static const mapX = 15;
        int static const mapY = 15;
        int static const window_width = 1000;
        int static const window_height = 700;            
        int unit = 35;
        //zmienne gry
        int score = 0;
        Apple* apple = NULL;
        int board[mapX][mapY]{};
        SnakeHead snake;
        int T;
        //zmienne okna
        sf::RenderWindow window;
        int static const FPS = 60;
        sf::Font font;
        sf::Text scoreText;
        int lastBestScore=0;
        int size1=80;
        int size2=60;

        void drawAll(){
                    
            int dx = (window_width - mapX*unit) / 2;
            int dy = (window_height - mapY*unit) / 2;

            //rysowanie planszy
            std::vector<std::vector<sf::RectangleShape> > grid;

            for(int i=0;i<mapX;i++){
                std::vector<sf::RectangleShape> v;
                for(int j=0;j<mapY;j++){
                    if(board[i][j] == 0){
                        sf::RectangleShape rectangle(sf::Vector2f(unit, unit));
                        if((i%2==0 and j%2==1) or (j%2==0 and i%2==1)){
                            rectangle.setFillColor(sf::Color(90, 240, 30));
                        }
                        else{
                            rectangle.setFillColor(sf::Color(100, 250, 50));
                        }
                        rectangle.setPosition(dx+i*unit,dy+j*unit);
                        v.push_back(rectangle);
                    }
                }
                grid.push_back(v);
            }

            for(int i=0;i<mapX;i++){
                for(int j=0;j<mapY;j++){
                    window.draw(grid[i][j]);
                }
            }

            //rysowanie jablka
            if(apple != NULL){
                window.draw(apple->drawApple(unit, dx, dy));
            }
                    
            //rysowanie snake
            sf::RectangleShape* snakeToDraw = snake.drawSnake(unit, dx, dy);
            for(int i=0;i<snake.getVisualSize()+1;i++){
                window.draw(snakeToDraw[i]);
            }
            delete[] snakeToDraw;

            //napis score
            scoreText.setCharacterSize(unit);
            scoreText.setString("Score: "+std::to_string(score));
            scoreText.setPosition(window_width/2 - scoreText.getLocalBounds().width/2, dy-2*unit);
            window.draw(scoreText);
    
        }

        Apple* getNewApple(){
            int xPos = rand() % mapX;
            int yPos = rand() % mapY; 
            while(snake.isOccupiedByTail(xPos, yPos) || (xPos == snake.getX() && yPos == snake.getY())){
                xPos = rand() % mapX;
                yPos = rand() % mapY; 
            }
            return new Apple(xPos, yPos);

        }

        bool checkIfGameIsOver(){
            //zderzenie z ogonem lub pojscie w przeciwna niz ide
            if(snake.isOccupiedByTail(snake.getX(), snake.getY())) return true;
            //wyszedl prawo-lewo
            if((snake.getX() <= 0 && snake.getDirection() == WEST) || (snake.getX() >= mapX-1 && snake.getDirection() == EAST)) return true;
            //wyszedl gora-dol
            if((snake.getY() <= 0 && snake.getDirection() == NORTH) || (snake.getY() >= mapY-1 && snake.getDirection() == SOUTH)) return true;
            //jesli wszystko ok
            return false;

        }
          
            
    public:
        Game(int period){
            T=period;
            //tworze okno
            window.create(sf::VideoMode(window_width, window_height), "Snake!");
            window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2-window_width/2,sf::VideoMode::getDesktopMode().height/2-window_height/2));


            //ustawienia napisu
            if(!font.loadFromFile("./SFML/pixelFont.ttf")){
                printf("Blad pobierania czcionki!\n");
            }
            scoreText.setFont(font);
            scoreText.setFillColor(sf::Color(255,255,255));

            //tworze snakea na srodku planszy
            snake = SnakeHead(7, 7, T);
        }

                
        void run(){
            //glowna petla
            int counter=0;
            while(window.isOpen()){

                //rysuje
                drawAll();

                //poruszam snake
                snake.move();

                //co T klatek zmiana pozycji snake
                if(counter % T == 0){
                    counter = counter % 10000;
                    snake.confirmPosition();

                    //sprawdzam przegrana
                    if(checkIfGameIsOver()){
                        snake.freeSnake();
                        window.close();                     

                    }

                    //tworze ewentualnie jablko
                    if(apple == NULL){
                        apple = getNewApple();
                    }
                    //zjadam ew jablko
                    else if(snake.getX() == apple->getX() && snake.getY() == apple->getY()){
                        snake.snakeGrow();
                        delete apple;
                        apple = NULL;
                        //zwiekszanie score'a
                        score++;
                    }

                }
                            
                //eventy okna
                sf::Event event;
                while (window.pollEvent(event))
                {
                    //zamkniecie okna
                    if (event.type == sf::Event::Closed){
                        snake.freeSnake();
                        window.close();
                    }
                                    
                }

                //sterowanie - strzalki i WSAD
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) snake.setNextDirection(WEST);
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) snake.setNextDirection(EAST);
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) snake.setNextDirection(NORTH);
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) snake.setNextDirection(SOUTH);

                //jak sleep tylko w mikrosekundach
                usleep(1000000* 1 / FPS);

                //inkrementacja licznika
                counter += 1;
                //update ekranu
                window.display();
                window.clear();
            }

            
            //zapis najlepszego wyniku
            std::ifstream getBestScore("dane.txt");
            getBestScore>>lastBestScore;
            getBestScore.close();

            if(lastBestScore<score){
                std::ofstream saveBestScore("dane.txt");
                saveBestScore<<score;
                saveBestScore.close();
            }

            end();
                
        }

        void end(){

            sf::RenderWindow endWindow;
            endWindow.create(sf::VideoMode(window_width, window_height), "Snake");
            endWindow.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2-window_width/2,sf::VideoMode::getDesktopMode().height/2-window_height/2));

            if(!font.loadFromFile("./SFML/pixelFont.ttf")){
                printf("Blad pobierania czcionki!\n");
            }


            while(endWindow.isOpen()){

                    //eventy okna
                    sf::Event event;
                    while (endWindow.pollEvent(event))
                    {
                        //zamkniecie okna
                        if (event.type == sf::Event::Closed){
                            endWindow.close();
                        }
                                        
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
                        endWindow.close();
                        Game game(20);
                        game.run();
                    } 
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
                        endWindow.close();
                        Game game(15);
                        game.run();
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){
                        endWindow.close();
                        Game game(10);
                        game.run();
                    }

                    sf::Text welcomeText("Game over! Play again!", font, size1);
                    welcomeText.setPosition(window_width/2 - welcomeText.getLocalBounds().width/2,15);
                    welcomeText.setFillColor(sf::Color(255,255,255));
                    
                    sf::Text difficultyText("choose the difficulty", font, size1);
                    difficultyText.setPosition(window_width/2 - difficultyText.getLocalBounds().width/2,80);
                    difficultyText.setFillColor(sf::Color(255,255,255));
        
                    sf::Text easyText("easy - press 1", font, size2);
                    easyText.setPosition(window_width/2 - easyText.getLocalBounds().width/2,200);
                    easyText.setFillColor(sf::Color(255,255,255));

                    sf::Text mediumText("medium - press 2", font, size2);
                    mediumText.setPosition(window_width/2 - mediumText.getLocalBounds().width/2,260);
                    mediumText.setFillColor(sf::Color(255,255,255));

                    sf::Text hardText("hard - press 3", font, size2);
                    hardText.setPosition(window_width/2 - hardText.getLocalBounds().width/2,320);
                    hardText.setFillColor(sf::Color(255,255,255));

                    sf::Text bestScoreText("Best score: " + std::__cxx11::to_string(lastBestScore), font, size2);
                    bestScoreText.setPosition(50,window_height - 100);
                    bestScoreText.setFillColor(sf::Color(255,255,255));

                    sf::Text scoreText("Your score: " + std::__cxx11::to_string(score), font, size2);
                    scoreText.setPosition(650,window_height - 100);
                    scoreText.setFillColor(sf::Color(255,255,255));
        
                    endWindow.draw(welcomeText);
                    endWindow.draw(difficultyText);
                    endWindow.draw(easyText);
                    endWindow.draw(mediumText);
                    endWindow.draw(hardText);
                    endWindow.draw(bestScoreText);
                    endWindow.draw(scoreText);

                    endWindow.display();

            }

        }     
    
};

class Configuration{
    private:
        int v=0;
        int static const window_width = 1000;
        int static const window_height = 700;
        int size1=80;
        int size2=60;
        sf::Font font;
        int lastBestScore;
        
        sf::Text difficultyText;
    public:
        Configuration(){

            //Pobranie danych
            std::ifstream getBestScore("dane.txt");
            getBestScore>>lastBestScore;
            getBestScore.close();

        }

        void drawAll(){

            sf::RenderWindow configurationWindow;
            configurationWindow.create(sf::VideoMode(window_width, window_height), "Snake");
            configurationWindow.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2-window_width/2,sf::VideoMode::getDesktopMode().height/2-window_height/2));

            if(!font.loadFromFile("./SFML/pixelFont.ttf")){
                printf("Blad pobierania czcionki!\n");
            }


            while(configurationWindow.isOpen()){

                    //eventy okna
                    sf::Event event;
                    while (configurationWindow.pollEvent(event))
                    {
                        //zamkniecie okna
                        if (event.type == sf::Event::Closed){
                            configurationWindow.close();
                        }
                                        
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
                        configurationWindow.close();
                        Game game(20);
                        game.run();
                    } 
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
                        configurationWindow.close();
                        Game game(15);
                        game.run();
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){
                        configurationWindow.close();
                        Game game(10);
                        game.run();
                    }

                    sf::Text welcomeText("Welcome to snake!", font, size1);
                    welcomeText.setPosition(window_width/2 - welcomeText.getLocalBounds().width/2,15);
                    welcomeText.setFillColor(sf::Color(255,255,255));
                    
                    sf::Text difficultyText("choose the difficulty", font, size1);
                    difficultyText.setPosition(window_width/2 - difficultyText.getLocalBounds().width/2,80);
                    difficultyText.setFillColor(sf::Color(255,255,255));
        
                    sf::Text easyText("easy - press 1", font, size2);
                    easyText.setPosition(window_width/2 - easyText.getLocalBounds().width/2,200);
                    easyText.setFillColor(sf::Color(255,255,255));

                    sf::Text mediumText("medium - press 2", font, size2);
                    mediumText.setPosition(window_width/2 - mediumText.getLocalBounds().width/2,260);
                    mediumText.setFillColor(sf::Color(255,255,255));

                    sf::Text hardText("hard - press 3", font, size2);
                    hardText.setPosition(window_width/2 - hardText.getLocalBounds().width/2,320);
                    hardText.setFillColor(sf::Color(255,255,255));

                    sf::Text bestScoreText("Best score: " + std::__cxx11::to_string(lastBestScore), font, size2);
                    bestScoreText.setPosition(50,window_height - 100);
                    bestScoreText.setFillColor(sf::Color(255,255,255));
        
                    configurationWindow.draw(welcomeText);
                    configurationWindow.draw(difficultyText);
                    configurationWindow.draw(easyText);
                    configurationWindow.draw(mediumText);
                    configurationWindow.draw(hardText);
                    configurationWindow.draw(bestScoreText);

                    configurationWindow.display();

            }

        }

};

int main()
{
   //dla randomow
    srand((unsigned) time(NULL));
    //uruchamiam 
    Configuration configuration;
    configuration.drawAll();
}
