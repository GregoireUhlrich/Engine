#ifndef WRAPMENU_H_INCLUDED
#define WRAPMENU_H_INCLUDED
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "button.h"

class WrapMenu: public Button{

    protected:

    int widthWidget;
    int heightWidget;
    std::string choice;

    void mouseReleased();

    public:
    
    WrapMenu();
    WrapMenu(sf::RenderTarget* user_target);
    WrapMenu(sf::RenderTarget* user_target, PropButton user_properties);
    WrapMenu(const WrapMenu& user_object);
    ~WrapMenu(){};

    void addChoice(std::string newChoice,  int type=0);
    void deleteChoice(std::string choice);
    void testMouse(sf::Vector2i user_posMouse);
    std::string getChoice();

    void draw(float elapsedTime);    
    WrapMenu& operator=(const WrapMenu& user_object);
    friend std::ostream& operator<<(std::ostream& f, const WrapMenu& user_object);
};
        
#endif
