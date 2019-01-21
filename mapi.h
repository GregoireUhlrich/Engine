#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

struct Sprite
{
    sf::Sprite s;
    sf::Texture* t;
};

class PNG_Encoder{

    protected:
    
    sf::Image img;
    int xPixel, yPixel, modePixel;
    int sizeImg;
    bool errorPeriodicity;
    bool errorOverFlow;
    
    std::string fileMap;
    std::vector<int> nTextureInPrio;
    std::vector<std::vector<int> > nSprite;
    std::vector<std::vector<std::vector<Sprite > > > sprite;
    int nTextureTot;
    std::vector<std::string> fileTexture;
    std::vector<sf::Texture> texture;
    
    sf::Vector2u sizeMap;
    int nExceptions;
    std::vector<int> exceptions;
    int nEvents;
    std::vector<std::string> events;
    
    int readInt();
    char readChar();
    std::string readString();
    void writeInt(int value);
    void writeChar(char c);
    void writeString(std::string s);
    
    int addTexture(std::string nameTexture);
   
    public:
    
    PNG_Encoder();
    PNG_Encoder(std::string user_fileMap);
    PNG_Encoder(std::vector<std::vector<std::vector<Sprite> > > user_sprite);
    PNG_Encoder(const PNG_Encoder& user_object);
    ~PNG_Encoder(){};
    
    std::string getFileMap() const;
    sf::Vector2u getSizeMap() const;
    void setFileMap(std::string user_fileMap);
    
    virtual int load();
    int save();
    
    PNG_Encoder& operator=(const PNG_Encoder& user_object);
    bool operator==(const PNG_Encoder& user_object);
    bool operator!=(const PNG_Encoder& user_object);
    friend std::ostream& operator<<(std::ostream& f, const PNG_Encoder& user_object);
    
    int loadTxt(std::string file);
};

class Map: public PNG_Encoder{

    private:
    
    sf::RenderTarget* target;
    
    sf::Vector2f position;
    sf::Vector2u sizeSprite;
    sf::RenderTexture texture;
    sf::Sprite sprite;
    
    void updateSprite();
   
    public:
    
    Map();
    Map(sf::RenderTarget* user_target);
    Map(sf::RenderTarget* user_target, std::string user_fileMap);
    ~Map(){};
    
    const sf::RenderTarget* getTarget() const;
    sf::Vector2f getPosition() const;
    sf::Vector2u getSizeSprite() const;
    sf::Sprite getSprite() const;
    
    void setTarget(sf::RenderTarget* user_target);
    void setPosition(sf::Vector2f user_position);
    void setSizeSprite(sf::Vector2u user_sizeSprite);
    
    int load();
    void draw(float elapsedTime);
    friend std::ostream& operator<<(std::ostream& f, const Map& user_object);
};

#endif

