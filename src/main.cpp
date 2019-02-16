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
       
    IMap map(&window,"level1_save.png");
    //map.loadTxt("level1.txt");
    map.load();
 
    Car car("voiture", map.getPlayerTexture());
    car.loadFromFile("AC_002.png");
    car.setSize(sf::Vector2i(32,48));
    car.setPosition(sf::Vector2f(500,200));


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
    bool printTime = false;
    while (window.isOpen())
    {
        posMouse = sf::Mouse::getPosition(window);
        label = 0;
        time2 = clock2.restart();
        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        Int.testMouse(posMouse);
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
            Int.testEvent(event);
            map.testEvent(event);
            lolita.testEvent(event);
            car.testEvent(event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                    car.setPositionMap(posMouse);
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
        map.draw(elapsedTime.asSeconds());
        time2 = clock2.restart();

        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        lolita.draw(elapsedTime.asSeconds());
        car.draw(elapsedTime.asSeconds());
        time2 = clock2.restart();

        if (printTime) cout<<"LABEL "<<label<<": "<<time2.asSeconds()<<endl;
        label++;
        Int.draw(elapsedTime.asSeconds());
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
