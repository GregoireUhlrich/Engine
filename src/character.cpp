#include "character.h" 
using namespace std;

/////////////////////////////////////////////
//////////// CLASS CHARACTER ////////////////
/////////////////////////////////////////////

Character::Character()
{
    target = 0;
    name = "";
    position = sf::Vector2f(0,0);
    grid = 1;
    isOnGrid = 1;
    staticDirection = 0;
    moveDirection = 0;
    nextMoveDirection = -1;
    moving = 0;
    velocity = 3*32;
    sizeSprite = sf::Vector2i(32,32);
    size = sf::Vector2i(32,32);
    texture.create(size.x,size.y);
    shiftPosition = sf::Vector2f((sizeSprite.x-size.x)/2,sizeSprite.y-size.y);
    textureRect = sf::IntRect(0,0,size.x,size.y);
}

Character::Character(string user_name)
{
    *this = Character();
    name = user_name;
}

Character::Character(sf::RenderTarget* user_target)
{
    *this = Character();
    target = user_target;
}

Character::Character(string user_name, sf::RenderTarget* user_target)
{
    *this = Character();
    target = user_target;
    name = user_name;
}

Character::Character(const Character& user_object)
{
    *this = user_object;
}

const sf::RenderTarget* Character::getTarget() const { return target;}
string Character::getName() const { return name;}
sf::Vector2f Character::getPosition() const { return position;}
sf::Vector2i Character::getNextIntPosition() const
{
    if (!grid or isOnGrid) return sf::Vector2i(round(position.x/sizeSprite.x),round(position.y/sizeSprite.y));
    switch(moveDirection)
    {
        case 0:
        return sf::Vector2i(round(position.x/sizeSprite.x)+1,round(position.y/sizeSprite.y));
        break;
        
        case 1:
        return sf::Vector2i(round(position.x/sizeSprite.x),round(position.y/sizeSprite.y)-1);
        break;
        
        case 2:
        return sf::Vector2i(round(position.x/sizeSprite.x)-1,round(position.y/sizeSprite.y));
        break;
        
        case 3:
        return sf::Vector2i(round(position.x/sizeSprite.x),round(position.y/sizeSprite.y)+1);
        break;
    }
}
sf::Vector2f Character::getShiftPosition() const { return shiftPosition;}
bool Character::getGrid() const { return grid;}
bool Character::getIsOnGrid() const { return isOnGrid;}
int Character::getStaticDirection() const { return staticDirection;}
int Character::getMoveDirection() const { return moveDirection;}
int Character::getNextMoveDirection() const { return nextMoveDirection;}
bool Character::getMoving() const { return moving;}
float Character::getVelocity() const { return velocity;}
sf::Vector2i Character::getSizeSprite() const { return sizeSprite;}
sf::Vector2i Character::getSize() const { return size;}
const sf::Texture& Character::getTexture() const { return texture;}
sf::Sprite Character::getSprite() const { return sprite;}
sf::IntRect Character::getTextureRect() const { return textureRect;}

void Character::setTarget(sf::RenderTarget* user_target)
{
    target = user_target;
}
void Character::setName(string user_name)
{
    name = user_name;
}
void Character::setPosition(sf::Vector2f user_position)
{
    if (!grid)
    {
        position = user_position;
        setSpritePosition();
    }
    else
    {
        if (round(user_position.x/sizeSprite.x) != user_position.x/sizeSprite.x or round(user_position.y/sizeSprite.y) != user_position.y/sizeSprite.y)
        {
            cout<<"Warning: position given to \"Character::setPosition(sf::Vector2f user_position)\" is not on grid: taking round(user_position).\n";
            user_position.x = round(user_position.x/sizeSprite.x)*sizeSprite.x;
            user_position.y = round(user_position.y/sizeSprite.y)*sizeSprite.y;
        }
        position = user_position;
        setSpritePosition();
        isOnGrid = 1;
    }
}
void Character::setBouncer(Bouncer user_bouncer)
{
    bouncer = user_bouncer;
}
void Character::setGrid(bool user_grid)
{
    grid = user_grid;
    if (grid and (round(position.x/sizeSprite.x) != position.x/sizeSprite.x or round(position.y/sizeSprite.y) != position.y/sizeSprite.y))
    {
        cout<<"Warning: call of \"Character::setGrid(bool user_grid)\" makes the character not on grid: taking round(position).\n";
        position.x = round(position.x/sizeSprite.x)*sizeSprite.x;
        position.y = round(position.y/sizeSprite.y)*sizeSprite.y;
    }
}
void Character::setStaticDirection(int user_staticDirection)
{
    if (user_staticDirection >= 0 and user_staticDirection < 4)
    {
        staticDirection = user_staticDirection;
    }
    else cout<<"Bad direction given to \"Character::setStaticDirection(int user_staticDirection)\": must be between 0 and 3.\n";
}
void Character::setMoveDirection(int user_moveDirection)
{
    if (user_moveDirection >= 0 and user_moveDirection < 4)
    {
        moveDirection = user_moveDirection;
        nextMoveDirection = moveDirection;
        staticDirection = moveDirection;
        moving = 1;
    }
    else cout<<"Bad direction given to \"Character::setMoveDirection(int user_moveDirection)\": must be between 0 and 3.\n";
}
void Character::setNextMoveDirection(int user_nextMoveDirection)
{
    if (user_nextMoveDirection >= -1 and user_nextMoveDirection < 4)
    {
        if (grid)
        {
            nextMoveDirection = user_nextMoveDirection;
            if (!moving and user_nextMoveDirection > -1)
            {
                moving = 1;
                moveDirection = nextMoveDirection;
            }
        }
        else
        {
            if (user_nextMoveDirection > -1)
            {
                moveDirection = user_nextMoveDirection;
                moving = 1;
            }
            else moving = 0;
        }
    }
    else cout<<"Bad direction given to \"Character::setMoveDirection(int user_nextMoveDirection)\": must be between -1 (no next move) and 3.\n";
}
void Character::setMoving(bool user_moving)
{
    if (!user_moving and moving and grid and !isOnGrid) cout<<"\"Character::setMoving(bool user_moving)\" denied: grid mode is activated and character not on grid.\n";
    else
    {
        moving = user_moving;
        staticDirection = moveDirection;
    }
}
void Character::setVelocity(float user_velocity)
{
    velocity = user_velocity;
}
void Character::setSizeSprite(sf::Vector2i user_sizeSprite)
{
    if (user_sizeSprite.x > 0 and user_sizeSprite.y > 0)
    {
        sizeSprite = user_sizeSprite;
        if (grid and (round(position.x/sizeSprite.x) != position.x/sizeSprite.x or round(position.y/sizeSprite.y) != position.y/sizeSprite.y))
        {
            cout<<"Warning: call of \"Character::setSizeSprite(sf::Vector2i user_sizeSprite)\" makes the character not on grid: taking round(position).\n";
            position.x = round(position.x/sizeSprite.x)*sizeSprite.x;
            position.y = round(position.y/sizeSprite.y)*sizeSprite.y;
        }
    }
    else cout<<"Invalid size in \"Character::setSizeSprite(sf::Vector2i user_sizeSprite)\": cannot be equal to zero.\n";
}
void Character::setSize(sf::Vector2i user_size)
{
    if (user_size.x > 0 and user_size.y > 0)
    {
        size = user_size;
        shiftPosition = sf::Vector2f((sizeSprite.x-size.x)/2,sizeSprite.y-size.y);
        textureRect = sf::IntRect(textureRect.left,textureRect.top,size.x,size.y);
        sprite.setTextureRect(textureRect);
    }
    else cout<<"Invalid size in \"Character::setSize(sf::Vector2i user_size)\": cannot be equal to zero.\n";
}

bool Character::loadFromFile(string file)
{
    if (texture.loadFromFile(file))
    {
        sprite.setTexture(texture);
        sprite.setTextureRect(textureRect);
        return 1;
    }
    cout<<"File not found in \"Character::loadFromFile(string file)\": nothing to do, previous texture is lost.\n";
    return 0;
}

void Character::move(sf::Vector2f shift)
{
    position += shift;
}

void Character::setSpritePosition()
{
    sprite.setPosition(position+shiftPosition);
}

void Character::updateGrid(float elapsedTime)
{
    if (moving and !isOnGrid)
    {
        float deltaX, deltaY; // distance before the next square
        float distance = velocity*elapsedTime; //distance the character would walk normally
        sf::Vector2f save_position = position;
        switch(moveDirection)
        {
            case 0:
            deltaX = ceil(position.x/sizeSprite.x)*sizeSprite.x - position.x;
            if (distance < deltaX or nextMoveDirection == moveDirection) position.x += distance;
            else {position.x += deltaX; isOnGrid = 1;}
            break;
            
            case 1:
            deltaY = position.y - floor(position.y/sizeSprite.y)*sizeSprite.y;
            if (distance < deltaY or nextMoveDirection == moveDirection) position.y -= distance;
            else {position.y -= deltaY; isOnGrid = 1;}
            break;
            
            case 2:
            deltaX = position.x - floor(position.x/sizeSprite.x)*sizeSprite.x;
            if (distance < deltaX or nextMoveDirection == moveDirection) position.x -= distance;
            else {position.x -= deltaX; isOnGrid = 1;}
            break;
            
            case 3:
            deltaY = ceil(position.y/sizeSprite.y)*sizeSprite.y - position.y; // distance between the caracter and the next square
            if (distance < deltaY or nextMoveDirection == moveDirection) position.y += distance; // we walk normally
            else {position.y += deltaY; isOnGrid = 1;} // we finish on the square
            break;
        }
        if (moveDirection == nextMoveDirection and !bouncer.ask(position,sizeSprite,moveDirection))
        {
            position = save_position;
            nextMoveDirection = -1;
        }
        else
        {
            if (isOnGrid)
            {
                if (nextMoveDirection == -1) moving = 0;
                else moveDirection = nextMoveDirection;
            }
            setSpritePosition();
        }
    }
    else if (moving)
    {
        isOnGrid = 0;
        update(elapsedTime);
    }
}

void Character::update(float elapsedTime)
{
    if (moving)
    {
        sf::Vector2f save_position = position;
        switch(moveDirection)
        {
            case 0: position.x += velocity*elapsedTime; break;
            case 1: position.y -= velocity*elapsedTime; break;
            case 2: position.x -= velocity*elapsedTime; break;
            case 3: position.y += velocity*elapsedTime; break;
        }
        if (!bouncer.ask(position,sizeSprite,moveDirection))
        {
            if (grid) isOnGrid = 1;
            position = save_position;
            setNextMoveDirection(-1);
            setMoving(false);
        }
        else setSpritePosition();
    }
}

void Character::draw(float elapsedTime)
{
    if (grid) updateGrid(elapsedTime);
    else update(elapsedTime);
    target->draw(sprite);
}


Character& Character::operator=(const Character& user_object)
{
    target = user_object.target;
    name = user_object.name;
    position = user_object.position;
    shiftPosition = user_object.shiftPosition;
    grid = user_object.grid;
    isOnGrid = user_object.isOnGrid;
    staticDirection = user_object.staticDirection;
    moveDirection = user_object.moveDirection;
    nextMoveDirection = user_object.nextMoveDirection;
    moving = user_object.moving;
    velocity = user_object.velocity;
    sizeSprite = user_object.sizeSprite;
    size = user_object.size;
    texture = user_object.texture;
    sprite = user_object.sprite;
    textureRect = user_object.textureRect;
    
    return *this;
}

ostream& operator<<(ostream& f, const Character& user_object)
{
    cout<<"Moving: "<<user_object.moving<<endl;
    cout<<"Grid: "<<user_object.grid<<endl;
    cout<<"isOnGrid: "<<user_object.isOnGrid<<endl;
    cout<<"StaticDirection: "<<user_object.staticDirection<<endl;
    cout<<"MoveDirection: "<<user_object.moveDirection<<endl;
    cout<<"NextMoveDirection: "<<user_object.nextMoveDirection<<endl;
    return f;
}

/////////////////////////////////////////////
//////////// CLASS PLAYER ///////////////////
/////////////////////////////////////////////

Player::Player(): Character()
{
    keyDefinition = vector<int>(sf::Keyboard::KeyCount);
    for (int i=0; i<sf::Keyboard::KeyCount; i++)
        keyDefinition[i] = -1;
    keyDefinition[sf::Keyboard::Right] = 0;
    keyDefinition[sf::Keyboard::Up] = 1;
    keyDefinition[sf::Keyboard::Left] = 2;
    keyDefinition[sf::Keyboard::Down] = 3;
    keyDefinition[sf::Keyboard::Space] = 4;
    
    currentKey = sf::Keyboard::Unknown;
    thresholdMove = 0.1;
    time = 0;
}

Player::Player(string user_name)
{
    *this = Player();
    name = user_name;
}

Player::Player(sf::RenderTarget* user_target)
{
    *this = Player();
    target = user_target;
}

Player::Player(string user_name, sf::RenderTarget* user_target)
{
    *this = Player();
    name = user_name;
    target = user_target;
}

Player::Player(const Player& user_object)
{
    *this = user_object;
}
    
std::vector<int> Player::getKeyDefinition() const { return keyDefinition;}
sf::Keyboard::Key Player::getCurrentKey() const { return currentKey;}
float Player::getTime() const { return time;}
float Player::getThresholdMove() const { return thresholdMove;}

void Player::setKeyDefinition(std::vector<int> user_keyDefinition)
{
    keyDefinition = user_keyDefinition;
}
void Player::setTime(float user_time)
{
    time = user_time;
}
void Player::setThresholdMove(float user_thresholdMove)
{
    thresholdMove = user_thresholdMove;
}

void Player::keyPressed(sf::Keyboard::Key key)
{
    if (grid)
    {
        key = (sf::Keyboard::Key)keyDefinition[key];
        if (key != -1)
        {
            if (key < 4)
            {
                if (key != currentKey)
                {
                    currentKey = key;
                    time = 0;
                    if (key == staticDirection) time = thresholdMove + 1;
                }
                if (!isOnGrid or time > thresholdMove)
                {
                    setNextMoveDirection(key);
                    time = 0;
                }
                if (!moving) staticDirection = key;
            }
        }
        else
        {
            currentKey = sf::Keyboard::Unknown;
            time = 0;
        }
    }
}

void Player::keyReleased(sf::Keyboard::Key key)
{
    key = (sf::Keyboard::Key)keyDefinition[key];
    if (key != -1)
    {
        if (key < 4 and key == currentKey)
        {
            currentKey = sf::Keyboard::Unknown;
            setNextMoveDirection(-1);
        }
    }
}

void Player::updateGrid(float elapsedTime)
{
    if (moving and !isOnGrid)
    {
        float deltaX, deltaY; // distance before the next square
        float distance = velocity*elapsedTime; //distance the character would walk normally
        sf::Vector2f save_position = position;
        switch(moveDirection)
        {
            case 0:
            deltaX = ceil(position.x/sizeSprite.x)*sizeSprite.x - position.x;
            if (distance < deltaX or nextMoveDirection == moveDirection) position.x += distance;
            else {position.x += deltaX; isOnGrid = 1;}
            break;
            
            case 1:
            deltaY = position.y - floor(position.y/sizeSprite.y)*sizeSprite.y;
            if (distance < deltaY or nextMoveDirection == moveDirection) position.y -= distance;
            else {position.y -= deltaY; isOnGrid = 1;}
            break;
            
            case 2:
            deltaX = position.x - floor(position.x/sizeSprite.x)*sizeSprite.x;
            if (distance < deltaX or nextMoveDirection == moveDirection) position.x -= distance;
            else {position.x -= deltaX; isOnGrid = 1;}
            break;
            
            case 3:
            deltaY = ceil(position.y/sizeSprite.y)*sizeSprite.y - position.y; // distance between the caracter and the next square
            if (distance < deltaY or nextMoveDirection == moveDirection) position.y += distance; // we walk normally
            else {position.y += deltaY; isOnGrid = 1;} // we finish on the square
            break;
        }
        if (moveDirection == nextMoveDirection and !bouncer.ask(position,sizeSprite,moveDirection))
        {
            position = save_position;
            nextMoveDirection = -1;
        }
        else
        {
            if (isOnGrid)
            {
                if (nextMoveDirection == -1) moving = 0;
                else moveDirection = nextMoveDirection;
            }
            setSpritePosition();
        }
    }
    else if (moving)
    {
        isOnGrid = 0;
        update(elapsedTime);
    }
}

void Player::testEvent(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape) setPosition(sf::Vector2f(2*32,10*32));
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        keyReleased(event.key.code);
    }
}

void Player::draw(float elapsedTime)
{
    for (int i=0; i<sf::Keyboard::KeyCount; i++)
        if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i))
            keyPressed((sf::Keyboard::Key)i);
    time += elapsedTime;
    if (grid) updateGrid(elapsedTime);
    else update(elapsedTime);
    target->draw(sprite);
}


Player& Player::operator=(const Player& user_object)
{
    Character::operator=(user_object);
    keyDefinition = user_object.keyDefinition;
    currentKey = user_object.currentKey;
    time = user_object.time;
    thresholdMove = user_object.thresholdMove;

    return *this;
}

ostream& operator<<(ostream& f, const Player& user_object)
{
    Character foo = user_object;
    cout<<foo;
    cout<<"CurrentKey: "<<user_object.currentKey<<endl;
    cout<<"ElapsedTime: "<<user_object.time<<endl;
    
    return f;
}