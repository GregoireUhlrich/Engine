#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "global.h"

class Widget{

    protected:
    
    sf::RenderTarget* target;
    sf::RenderTexture texture;
    sf::Sprite sprite;
    
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Vector2i posMouse;
    sf::Vector2i posClick;
    int nKey;
    std::vector<sf::Keyboard::Key> key;
    int nAttachedWidget;
    std::vector<Widget*> attachedWidget;
    
    bool enabled;
    bool isMouseHere;
    bool isMousePressed;
    int deltaMouseWheel;
   
    virtual void mousePressed();
    virtual void mouseReleased();
    void mouseWheel(int user_deltaMouseWheel);
    void keyPressed(sf::Keyboard::Key user_key);
    void keyReleased(sf::Keyboard::Key user_key);
    
    public:
    
    Widget();
    Widget(sf::RenderTarget* user_target);
    Widget(const Widget& user_object);
    ~Widget(){};
    
    const sf::RenderTarget* getTarget() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    sf::Vector2i getPosMouse() const;
    sf::Vector2i getPosClick() const;
    bool getIsMouseHere() const;
    bool getIsMousePressed() const;
    bool getEnabled() const;
    int getDeltaMouseWheel() const;
    virtual std::string getChoice() { return "";}
    
    void setTarget(sf::RenderTarget* user_target);
    virtual void setPosition(sf::Vector2f user_position);
    virtual void setSize(sf::Vector2f user_size);
    void setEnabled(bool user_enabled);
    virtual void setText(std::string){};
    
    virtual void testMouse(sf::Vector2i user_posMouse);
    void testEvent(sf::Event event);
    
    virtual void draw(float elapsedTime);
    
    Widget& operator=(const Widget& user_object);
};

class Text: public sf::Text{
    
    private:
    
    sf::Font font;
    
    public:
    
    Text();
    Text(std::string user_string);
    Text(const sf::String& str, const sf::Font &user_font, unsigned int characterSize);
    ~Text(){};
    
    void setFont(std::string fontFile);
};

#endif
