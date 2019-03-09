#include <iostream>
#include "character.h"
#include "map.h"
#include "button.h"
#include "wrapMenu.h"
#include "interactive.h"
#include <cstdlib>
using namespace std;

int alea(int min, int max)
{
    double r = rand()*1./RAND_MAX;
    max ++;
    int answer = floor(r*(max-min)+min);
    if (answer == max + 1) answer = max;
    return answer;
}

void action(int dir)
{
    cout<<dir<<endl;
}

int main()
{
    srand(time(0));
    //sf::ContextSettings settings;
    //settings.antialiasingLevel = 8;
    int xWindow = min(1600,(int)round(8/10.*sf::VideoMode::getDesktopMode().width));
    int yWindow = xWindow*9/16;

    sf::RenderWindow window(sf::VideoMode(xWindow, yWindow), "SFML shapes", sf::Style::Default);
    //window.setVerticalSyncEnabled(true);
    sf::View view = window.getView();
    sf::Vector2u size = window.getSize();
    //window.setView(view);
    Player lolita("lolita",&window);
    lolita.loadFromFile("AC_002.png");
    lolita.setDisplayMode(2);
    lolita.setSize(sf::Vector2i(32,48));
    //lolita.setGrid(false);
    lolita.setPosition(sf::Vector2f(2*32,10*32));
       
    IMap map(&window,"Maps/testMap.png");
    //map.loadTxt("Maps/testMap.txt");
    map.load();
 
    Car car("Blue", map.getPlayerTexture());
    car.loadFromFile("car_blue_a.png");
    car.setSize(sf::Vector2i(82,145));
    car.setPosition(sf::Vector2f(500,200));
    car.setBouncer(map.getBouncer());

    Car car2("Red", map.getPlayerTexture());
    car2.loadFromFile("car_red_a.png");
    car2.addAngularVelocity(8);
    car2.setSize(sf::Vector2i(82,145));
    car2.setPosition(sf::Vector2f(500,500));
    car2.setBouncer(map.getBouncer());
    car2.setKeyDefinition(sf::Keyboard::Z, sf::Keyboard::Up);
    car2.setKeyDefinition(sf::Keyboard::S, sf::Keyboard::Down);
    car2.setKeyDefinition(sf::Keyboard::Q, sf::Keyboard::Left);
    car2.setKeyDefinition(sf::Keyboard::D, sf::Keyboard::Right);

    Interactive Int(&window, &map);
    map.addException(sf::Vector2i(0,0),0);
    map.addException(sf::Vector2i(1,1),0);
    map.addException(sf::Vector2i(1,1),1);
    map.addException(sf::Vector2i(1,1),2);
    map.addException(sf::Vector2i(1,1),3);
    map.addException(sf::Vector2i(2,3),3);
    lolita.setBouncer(map.getBouncer());
    sf::Clock clock;
    sf::Clock clock2;
    clock2.restart();
    sf::Time time2;
    sf::Time elapsedTime = clock.restart();
    sf::Vector2i posMouse(0,0);
    sf::Event event; 
        

    int label = 0;
    bool gameMode = false;
    bool printTime = false;
    while (window.isOpen())
    {
        posMouse = sf::Mouse::getPosition(window);
        label = 0;
        time2 = clock2.restart();
        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        if (not gameMode) Int.testMouse(posMouse);
        time2 = clock2.restart();

        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        map.testMouse(posMouse);
        time2 = clock2.restart();

        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        map.setIsMouseHere(not Int.getIsMouseHere());
        time2 = clock2.restart();


        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        while (window.pollEvent(event) and window.isOpen())
        {
            if (not gameMode) Int.testEvent(event);
            map.testEvent(event);
            lolita.testEvent(event);
            car.testEvent(event);
            car2.testEvent(event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    car.setPosition(sf::Vector2f(500,200));
                    car2.setPosition(sf::Vector2f(500,500));
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    gameMode = not gameMode;
                    map.setGameMode(gameMode);
                }
            }
        }
        time2 = clock2.restart();

        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        if (size != window.getSize())
        {
            size = window.getSize();
            window.setView(sf::View(sf::FloatRect(0,0,size.x,size.y)));
        }
        time2 = clock2.restart();

        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;

        elapsedTime = clock.restart();
        window.clear();
        time2 = clock2.restart();

        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        lolita.draw(elapsedTime.asSeconds());
        testCollision(&car, &car2);
        car.draw(elapsedTime.asSeconds());
        car2.draw(elapsedTime.asSeconds());
        time2 = clock2.restart();

        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        if (gameMode)
        {
           sf::Vector2f center = car.getPosition();
           center.x += car2.getPosition().x;
           center.y += car2.getPosition().y;
           center.x /= 2;
           center.y /= 2;
           map.setCenterViewGame(center);
        }
        map.draw(elapsedTime.asSeconds());
        time2 = clock2.restart();

        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        if (not gameMode) Int.draw(elapsedTime.asSeconds());
        time2 = clock2.restart();


        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        window.display();
        time2 = clock2.restart();

        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;

        string foo = Int.getChoice();
        if (foo != "")
        {
            if (foo == "Quit") window.close();
        }
        time2 = clock2.restart();

        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
    }
 
    return 0;
}
