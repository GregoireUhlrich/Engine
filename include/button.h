#ifndef PUSHBUTTON_H_INCLUDED
#define PUSHBUTTON_H_INCLUDED
#include <iostream>
#include "SFML/Graphics.hpp"
#include "widget.h"
#include <string>

struct PropButton{

    int mode;
    float scaleIcon;
    std::vector<std::string> fileTextureIcon;
    std::vector<sf::Texture> textureIcon;
    std::vector<sf::Sprite> spriteIcon;
    std::vector<sf::Color> fillColor;
    std::vector<sf::Color> outlineColor;
    std::vector<sf::Color> textColor;
    std::vector<int> outlineThickness;
};

class Button: public Widget{

    protected:
    
    PropButton properties;
    int offsetText;
    Text text;
    sf::RectangleShape outline;
    float ratioPressed;
    
    bool init;
    bool textCentered;
    int iMode;
    bool signal;
    bool icon;
    bool chirality;
    
    public:
    
    Button();
    Button(sf::RenderTarget* user_target);
    Button(sf::RenderTarget* user_target, const PropButton& user_properties);
    Button(sf::RenderTarget* user_target, const PropButton& user_properties, bool user_icon);
    Button(const Button& user_object);
    ~Button(){};
    
    sf::Sprite getSpriteIcon() const;
    Text getText() const;
    virtual bool getSignal();
    virtual std::string getChoice() { return getText().getString().toAnsiString();}
    virtual std::string getCurrentChoice() const{ return getText().getString().toAnsiString();}
    
    void setPosition(sf::Vector2f user_position);
    void setSize(sf::Vector2f user_size);
    void setProperties(PropButton user_properties);
    void setTextureIcon(std::string textureFile);
    void setText(std::string user_text);
    void setTextCentered(bool user_textCentered);
    void setIcon(bool user_icon);
    void setScale(float user_scale);
    void setChirality(bool user_chirality);
    void setRatioPressed(float user_ratioPressed);
    
    virtual void testMouse(sf::Vector2i user_posMouse);
    
    virtual void updateButton();
    void draw(float elapsedTime);
    
    Button& operator=(const Button& user_object);
    friend std::ostream& operator<<(std::ostream& f, const Button& user_object);
};

class PushButton: public Button{

    public:

    PushButton();
    PushButton(sf::RenderTarget* user_target);
    PushButton(sf::RenderTarget* user_target, const PropButton& user_properties);
    PushButton(const PushButton& user_object);
    ~PushButton(){};

    void mousePressed();
    void mouseReleased();

    void updateButton();

    PushButton& operator=(const PushButton& user_object);
};

class PressButton: public PushButton{

    public:
    
    PressButton();
    PressButton(sf::RenderTarget* user_target);
    PressButton(sf::RenderTarget* user_target, const PropButton& user_properties);
    PressButton(const PressButton& user_object);
    ~PressButton(){};
    
    bool getSignal() const;
    void setPressed(bool isPressed);
    void testMouse(sf::Vector2i posMouse);
    void mouseReleased();

    PressButton& operator=(const PressButton& user_object);
};

#endif

