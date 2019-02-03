#ifndef WRAPMENU_H_INCLUDED
#define WRAPMENU_H_INCLUDED
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "button.h"

class WrapMenu: public Button{

    protected:

    bool detached;
    sf::Vector2f detachedPosition;
    sf::RenderTarget* detachedTarget;
    sf::RenderTexture attachedTexture;
    sf::Sprite attachedSprite;

    int widthWidget;
    int heightWidget;
    std::string choice;

    int yMenu;
    int lyMenu;
    sf::Vector2f sizeScrollBar;
    sf::RectangleShape scrollBar;

    public:
    
    WrapMenu();
    WrapMenu(sf::RenderTarget* user_target);
    WrapMenu(sf::RenderTarget* user_target, PropButton user_properties);
    WrapMenu(const WrapMenu& user_object);
    ~WrapMenu(){};

    virtual void addChoice(std::string newChoice, int type=0);
    virtual void addChoice(Widget* newWidget);
    void deleteChoice(std::string choice);
    void testMouse(sf::Vector2i user_posMouse);
    virtual std::string getChoice();
    std::string getCurrentChoice() const;
    void setDetached(bool user_detached);
    void setDetachedPosition(sf::Vector2f setDetachedPosition);
    void setDetachedTarget(sf::RenderTarget* user_target);

    void mouseReleased();

    void updateButton();
    virtual void draw(float elapsedTime);    
    WrapMenu& operator=(const WrapMenu& user_object);
    friend std::ostream& operator<<(std::ostream& f, const WrapMenu& user_object);
};

class WrapMenuSide: public WrapMenu{

    public:

    WrapMenuSide();
    WrapMenuSide(sf::RenderTarget* user_target);
    WrapMenuSide(sf::RenderTarget* user_target, PropButton user_properties);
    WrapMenuSide(const WrapMenu& user_object);
    ~WrapMenuSide(){};

    void addChoice(std::string newChoice, int type=0);
    void addChoice(Widget* newWidget);

    void updateButton();
    void draw(float elapsedTime);
};
        
#endif
