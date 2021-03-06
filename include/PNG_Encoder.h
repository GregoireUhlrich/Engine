#ifndef PNG_ENCODER_H_INCLUDED
#define PNG_ENCODER_H_INCLUDED
#include <SFML/Graphics.hpp>
#include "bouncer.h"
#include "global.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>

class PNG_Encoder{

    protected:
    
    sf::Image img;
    int xPixel, yPixel, modePixel;
    int sizeImg;
    bool errorPeriodicity;
    bool errorOverFlow;
    
    int nPrio;
    std::string fileMap;
    std::vector<int> nTextureInPrio;
    std::vector<std::vector<int> > nSprite;
    std::vector<std::vector<std::vector<sf::Sprite > > > sprite;
    std::vector<std::vector<std::vector<std::vector<int> > > > indexSprite;
    std::vector<std::vector<int > > iTexture;
    int nTextureTot;
    std::vector<std::string> fileTexture;
    std::vector<sf::Texture> texture;
    
    sf::Vector2i sizeSprite;
    sf::Vector2i sizeMap;
    Bouncer bouncer;
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
    PNG_Encoder(const PNG_Encoder& user_object);
    ~PNG_Encoder(){};
    
    std::string getFileMap() const;
    std::vector<std::string > getFileTextures() const;
    sf::Vector2i getSizeSprite() const;
    sf::Vector2i getSizeMap() const;
    Bouncer getBouncer() const;
    virtual void setSizeSprite(sf::Vector2i user_sizeSprite);
    void setFileMap(std::string user_fileMap);
    
    virtual int load();
    int save();
    
    PNG_Encoder& operator=(const PNG_Encoder& user_object);
    bool operator==(const PNG_Encoder& user_object);
    bool operator!=(const PNG_Encoder& user_object);
    friend std::ostream& operator<<(std::ostream& f, const PNG_Encoder& user_object);
    
    virtual int loadTxt(std::string stringFile);
};

#endif
