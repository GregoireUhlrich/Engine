#include <iostream>
#include "character.h"
#include "map.h"
#include "button.h"
#include "wrapMenu.h"
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
    
    sf::RenderWindow window(sf::VideoMode(800,800), "Test engine");
    sf::View view(sf::FloatRect(0,0,32*20,32*20));
    //window.setView(view);
    Player lolita("lolita",&window);
    lolita.loadFromFile("vieille1.png");
    lolita.setSize(sf::Vector2i(32,48));
    //lolita.setGrid(false);
    lolita.setPosition(sf::Vector2f(2*32,10*32));
    
    Character fantome("fantome",&window);
    fantome.loadFromFile("fantome1.png");
    fantome.setSize(sf::Vector2i(32,48));
    fantome.setPosition(sf::Vector2f(10.5*32,10*32));
    
    Map map(&window,"level1_save.png");
    //map.loadTxt("level1.txt");
    map.load();
    
    PropButton properties;
    properties.mode = 2;
    properties.fileTextureIcon = vector<string>(2);
    properties.outlineColor = vector<sf::Color>(2);
    properties.fillColor = vector<sf::Color>(2);
    properties.outlineThickness = vector<int>(2);
    properties.fileTextureIcon[0] = "restart-grey.png";
    properties.fileTextureIcon[1] = "restart-yellow.png";
    properties.outlineThickness[0] = 1;
    properties.outlineThickness[1] = 1;
    properties.outlineColor[0] = sf::Color(110,110,110);
    properties.outlineColor[1] = sf::Color(84,106,139);
    properties.fillColor[0] = sf::Color::White;
    properties.fillColor[1] = sf::Color::White;
    PushButton button(&window, properties);
    button.setPosition(sf::Vector2f(100,100));
    
    properties.fileTextureIcon[0] = "add-grey.png";
    properties.fileTextureIcon[1] = "add-green.png";
    PressButton button2(&window, properties);
    button2.setPosition(sf::Vector2f(150,100));
    
    Button button4(&window, properties);
    button4.setPosition(sf::Vector2f(150,150));
    
    properties.fillColor[0] = sf::Color::White;
    properties.fillColor[1] = sf::Color::Green;
    properties.textColor = vector<sf::Color>(2);
    properties.textColor[0] = sf::Color::Green;
    properties.textColor[1] = sf::Color::White;
    PushButton button3(&window,properties);
    button3.setText("Enable/Disable the buttons to the left");
    button3.setIcon(false);
    button3.setPosition(sf::Vector2f(200,100));
    WrapMenu wrap(&window);
    wrap.setText("wrap test");
    cout<<"wrap created\n";
    wrap.addChoice("Choice 1");
    wrap.addChoice("Choice 2");
    wrap.addChoice("Choice 3");
    wrap.addChoice("Choice 4");
    wrap.addChoice("Choice 5");
    wrap.addChoice("Choice 6");
    map.addException(sf::Vector2i(0,0),0);
    map.addException(sf::Vector2i(1,1),0);
    map.addException(sf::Vector2i(1,1),1);
    map.addException(sf::Vector2i(1,1),2);
    map.addException(sf::Vector2i(1,1),3);
    map.addException(sf::Vector2i(2,3),3);
    lolita.setBouncer(map.getBouncer());
    fantome.setBouncer(map.getBouncer());
    sf::Clock clock;
    sf::Time elapsedTime = clock.restart();
    sf::Vector2i posMouse;
    sf::Event event;
    while (window.isOpen())
    {
        posMouse = sf::Mouse::getPosition(window);
        button.testMouse(posMouse);
        button2.testMouse(posMouse);
        button3.testMouse(posMouse);
        button4.testMouse(posMouse);
        wrap.testMouse(posMouse);
        while (window.pollEvent(event) and window.isOpen())
        {
            button.testEvent(event);
            button2.testEvent(event);
            button3.testEvent(event);
            button4.testEvent(event);
            wrap.testEvent(event);
            lolita.testEvent(event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        elapsedTime = clock.restart();
        window.clear();
        map.draw(elapsedTime.asSeconds());
        if (button.getSignal()) lolita.setPosition(sf::Vector2f(2*32,10*32));
        lolita.draw(elapsedTime.asSeconds());
        if (button2.getSignal()) fantome.draw(elapsedTime.asSeconds());
        if (button3.getSignal()) { button.setEnabled(!button.getEnabled()); button2.setEnabled(!button2.getEnabled());}
        if (fantome.getGrid()) fantome.setNextMoveDirection(alea(0,3));
        else fantome.setMoveDirection(alea(0,3));
        //cout<<lolita<<endl;
        button.draw(elapsedTime.asSeconds());
        button2.draw(elapsedTime.asSeconds());
        button3.draw(elapsedTime.asSeconds());
        button4.draw(elapsedTime.asSeconds());
        wrap.draw(elapsedTime.asSeconds());
        string choice = wrap.getChoice();
        if (choice != "")
        {
            cout<<"Wrap says: "<<choice<<endl;
            choice = "";
        }
        window.display();
    }
    map.setFileMap("level1_save.png");
    map.save();
    return 0;
}
