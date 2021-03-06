#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "global.h"

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
    virtual Text getText() const{};
    virtual bool getIsMouseHere() const;
    bool getIsMousePressed() const;
    bool getEnabled() const;
    virtual bool getSignal() const { return false;}
    int getDeltaMouseWheel() const;
    virtual std::string getChoice() { return "";}
    virtual std::string getCurrentChoice() const{ return "";};
    virtual int getChoiceSet() const { return -1;}
    virtual std::string getFileLeft() const { return "";}
    virtual sf::Vector2i getNSpriteToPull() const { return sf::Vector2i(0,0);};
    virtual sf::Vector2i getPosSpriteToPull() const { return sf::Vector2i(0,0);};
    virtual void resetSelection(){};
    
    void setTarget(sf::RenderTarget* user_target);
    virtual void setPosition(sf::Vector2f user_position);
    virtual void setSize(sf::Vector2f user_size);
    void setEnabled(bool user_enabled);
    virtual void setText(std::string){};
    virtual void setTextCentered(bool user_textCentered){};
    virtual void setPressed(bool isPressed){};
    virtual void setRatioPressed(float user_ratioPressed){};
    virtual void setIcon(bool user_icon){};
    virtual void addChoice(std::string newChoice, int type=0){};
    virtual void addChoice(Widget* widget){};
    virtual void setChoiceSet(int choice){};
    virtual void addTextureFromMap(std::string file){};
    virtual void setDetached(bool user_detached){};
    virtual void setDetachedPosition(sf::Vector2f setDetachedPosition){};
    virtual void setDetachedTarget(sf::RenderTarget* user_target){};
    virtual void setFileLeft(std::string file){};
    
    virtual void testMouse(sf::Vector2i user_posMouse);
    virtual void testEvent(sf::Event event);
   
    virtual void mousePressed();
    virtual void mouseReleased();
    virtual void mouseWheel(int user_deltaMouseWheel);
    virtual void keyPressed(sf::Keyboard::Key user_key);
    virtual void keyReleased(sf::Keyboard::Key user_key);
    virtual void windowResized(){};
    
    virtual void updateButton(){};
    virtual void draw(float elapsedTime);
    
    Widget& operator=(const Widget& user_object);
};
#endif
