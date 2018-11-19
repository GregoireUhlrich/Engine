#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "PNG_Encoder.h"
#include "bouncer.h"

class Map: public PNG_Encoder{

    private:
    
    sf::RenderTarget* target;
    
    sf::Vector2f position;
    sf::RenderTexture renderTexture;
    sf::Sprite renderSprite;
    
    void updateSprite();
   
    public:
    
    Map();
    Map(sf::RenderTarget* user_target);
    Map(sf::RenderTarget* user_target, std::string user_fileMap);
    ~Map(){};
    
    const sf::RenderTarget* getTarget() const;
    sf::Vector2f getPosition() const;
    sf::Sprite getRenderSprite() const;
    
    void setTarget(sf::RenderTarget* user_target);
    void setPosition(sf::Vector2f user_position);
    void setSizeSprite(sf::Vector2i user_sizeSprite);
    
    void addException(sf::Vector2i positionException, int direction);
    
    int load();
    void draw(float elapsedTime);
    friend std::ostream& operator<<(std::ostream& f, const Map& user_object);
    
    int loadTxt(std::string stringFile);
};

#endif

