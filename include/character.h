#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED
#include <iostream>
#include "SFML/Graphics.hpp"
#include "bouncer.h"
#include <string>
#include <cmath>

int convertDir(int dir);

class Character{

    protected:
    
    sf::RenderTarget* target;
    
    std::string name;
    sf::Vector2f position;
    sf::Vector2f shiftPosition;
    Bouncer bouncer;
    
    bool grid;
    bool isOnGrid;
    int staticDirection;
    int moveDirection;
    int nextMoveDirection;
    bool moving;
    float velocity;
    int displayMode;
    float elapsedDisplayTime;
    float timeStepDisplay;
    
    sf::Vector2i sizeSprite;
    sf::Vector2i size;
    sf::Texture texture;
    sf::Sprite sprite;
    
    //// Protected functions
    
    void setSpritePosition();
    virtual void updateGrid(float elapsedTime);
    void updateTextureRect(float elapsedTime=0);
    void update(float elapsedTime);
   
    public:
    
    Character();
    Character(std::string user_name);
    Character(sf::RenderTarget* user_target);
    Character(std::string user_name, sf::RenderTarget* user_target);
    Character(const Character& user_object);
    virtual ~Character(){};
    
    const sf::RenderTarget* getTarget() const;
    std::string getName() const;
    sf::Vector2f getPosition() const;
    sf::Vector2i getNextIntPosition() const;
    sf::Vector2f getShiftPosition() const;
    bool getGrid() const;
    bool getIsOnGrid() const;
    int getStaticDirection() const;
    int getMoveDirection() const;
    int getNextMoveDirection() const;
    bool getMoving() const;
    float getVelocity() const;
    sf::Vector2i getSizeSprite() const;
    sf::Vector2i getSize() const;
    const sf::Texture& getTexture() const;
    sf::Sprite getSprite() const;
    
    void setTarget(sf::RenderTarget* user_target);
    void setName(std::string user_name);
    void setPosition(sf::Vector2f user_position);
    void setBouncer(Bouncer user_bouncer);
    void setGrid(bool user_grid);
    void setStaticDirection(int user_staticDirection);
    void setMoveDirection(int user_moveDirection);
    void setNextMoveDirection(int user_nextMoveDirection);
    void setMoving(bool user_moving);
    void setVelocity(float user_velocity);
    void setDisplayMode(int user_displayMode);
    void setSizeSprite(sf::Vector2i user_sizeSprite);
    void setSize(sf::Vector2i user_size);
    
    bool loadFromFile(std::string file);
    void move(sf::Vector2f shift);
    virtual void draw(float elapsedTime);
    
    Character& operator=(const Character& user_object);
    friend std::ostream& operator<<(std::ostream& f, const Character& user_object);
};

class Player: public Character{

    private:
    
    std::vector<int> keyDefinition;
    sf::Keyboard::Key currentKey;
    float time;
    float thresholdMove;
    
    void keyPressed(sf::Keyboard::Key key);
    void keyReleased(sf::Keyboard::Key key);
    void updateGrid(float elapsedTime);
   
    public:
    
    Player();
    Player(std::string user_name);
    Player(sf::RenderTarget* user_target);
    Player(std::string user_name, sf::RenderTarget* user_target);
    Player(const Player& user_object);
    ~Player(){};
    
    std::vector<int> getKeyDefinition() const;
    sf::Keyboard::Key getCurrentKey() const;
    float getTime() const;
    float getThresholdMove() const;
    
    void setKeyDefinition(std::vector<int> user_keyDefinition);
    void setTime(float user_time);
    void setThresholdMove(float user_thresholdMove);
    
    void testEvent(sf::Event event);
    void draw(float elapsedTime);
    
    Player& operator=(const Player& user_object);
    friend std::ostream& operator<<(std::ostream& f, const Player& user_object);
};

int sgn(float x);
float modulo_2pi(float theta);
float angle(float a, float b);

class Car: public Character{

    private:
    
    float theta, dtheta;
    float thetaDir;
    float omega;
    float attenuation_omega;
    float driftEffect;
    float accroche;
    float acceleration;
    float maxVelocity;
    sf::Keyboard::Key currentKey;
    float time;
    float timeKey;
    float thresholdMove;
    std::vector<int> keyDefinition;
    
    public:
    
    Car();
    Car(std::string user_name);
    Car(sf::RenderTarget* user_target);
    Car(std::string user_name, sf::RenderTarget* user_target);
    Car(const Car& user_object);
    ~Car(){};
    
    sf::Keyboard::Key getCurrentKey() const;
    float getTime() const;
    float getThresholdMove() const;
    float getTheta() const;
    sf::Vector2i getSize() const;
    bool isValid(sf::Vector2f newPos) const;
    sf::Vector2f getVectorVelocity() const;
    
    void setTime(float user_time);
    void setThresholdMove(float user_thresholdMove);
    void setPositionMap(sf::Vector2i posMouse);
    void setKeyDefinition(sf::Keyboard::Key keyToPress, sf::Keyboard::Key keyToUnderstand);
    void setVelocity(float t_velocity);
    void setVectorVelocity(sf::Vector2f vector);
    void addAngularVelocity(float t_omega);
    
    void testEvent(sf::Event event);
    void draw(float elapsedTime);
    
    Car& operator=(const Car& user_object);
    friend std::ostream& operator<<(std::ostream& f, const Car& user_object);
};

float distance(sf::Vector2f v1, sf::Vector2f v2);
float distance(sf::Vector2i v1, sf::Vector2i v2);
float scalar(sf::Vector2f v1, sf::Vector2f v2);
float cross(sf::Vector2f v1, sf::Vector2f v2);
float norm(sf::Vector2f v);
float norm(sf::Vector2i v);
sf::Vector2f rotation(float theta, sf::Vector2f v);
void testCollision(Car* car1, Car* car2);
void Collision(Car* car1, Car* car2, sf::Vector2f coin);

#endif

