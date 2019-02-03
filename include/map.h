#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "PNG_Encoder.h"
#include "bouncer.h"
#include "global.h"

struct PropZoom{

    double iZoom;
    double maxZoom;
    double minZoom;
};

class Map: public PNG_Encoder{

    protected:

    int nPrio;
    
    sf::Vector2f position;
    std::vector<sf::RenderTexture > textureMap;
    std::vector<sf::Sprite > spriteMap;
    
    void updateSprite();
   
    public:
    
    Map();
    Map(std::string fileMap);
    ~Map(){};
    
    sf::Vector2f getPosition() const;
    std::vector<sf::Sprite > getRenderSprite() const;
    
    void setPosition(sf::Vector2f user_position);
    void setSizeSprite(sf::Vector2i user_sizeSprite);
    
    void addException(sf::Vector2i positionException, int direction);

    int load();
    friend std::ostream& operator<<(std::ostream& f, const Map& user_object);
    
    int loadTxt(std::string stringFile);
};


class IMap: public Map{

    protected:

    StateMap state;

    sf::RenderTarget* target;

    sf::RenderTexture bigRenderTexture;
    sf::View view;
    sf::Color colorTexture;
    sf::Sprite bigRenderSprite;
    sf::RectangleShape outline;

    bool isMouseHere;
    bool isMousePressed;
    sf::Vector2i posMouse;
    sf::Vector2i posClick;
    int delta;

    int prio;
    bool allPrio;

    PropZoom zoom;

    bool grid;
    std::vector<sf::RectangleShape> gridX;
    std::vector<sf::RectangleShape> gridY;

    bool pass;

    bool isGhostSprite;
    sf::RenderTexture ghostTexture;
    std::string ghostFileTexture;
    sf::Vector2i ghostPosition;

    bool select, select2;
    sf::RectangleShape selectRect;

    public:

    IMap();
    IMap(sf::RenderTarget* user_target);
    IMap(sf::RenderTarget* user_target, std::string user_fileMap);
    ~IMap();

    StateMap getState() const;

    void setState(StateMap user_state);

    void testMouse(sf::Vector2i posMouse);
    void setIsMouseHere(bool user_isMouseHere);
    void setAllPrio(bool user_allPrio);
    void setPrio(int user_prio);
    void setGrid(bool user_grid);
    void setPass(bool user_pass);
    void setGhost(std::string texture, sf::Vector2i nSpriteToPull, sf::Vector2i posSpriteToPull);

    void testEvent(sf::Event event);
    void mousePressed();
    void mouseReleased();
    void scrollMouse(int user_delta);
    void windowResized();

    void updateSprite();
    void draw(float elapsedTime);
};


#endif

