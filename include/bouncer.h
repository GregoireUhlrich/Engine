#ifndef BOUNCER_H_INCLUDED
#define BOUNCER_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Bouncer{

    private:
    
    int nExceptions;
    std::vector<std::vector<std::vector<bool> > > passOrNot;
    sf::Vector2i sizeMap;
    
    void countExceptions();
   
    public:
    
    Bouncer();
    Bouncer(const Bouncer& user_object);
    ~Bouncer(){};
    
    int getNExceptions() const;
    std::vector<std::vector<std::vector<bool> > > getPassOrNot() const;
    sf::Vector2i getSizeMap() const;
    
    void setNExceptions(int user_nExceptions);
    void setPassOrNot(std::vector<std::vector<std::vector<bool> > > user_passOrNot);
    void setSizeMap(sf::Vector2i user_sizeMap);
    
    void addException(sf::Vector2i position, int direction);
    bool ask(sf::Vector2f position, sf::Vector2i sizeSprite ,int direction);
        
    Bouncer& operator=(const Bouncer& user_object);
    friend std::ostream& operator<<(std::ostream& f, const Bouncer& user_object);
};
        
#endif
