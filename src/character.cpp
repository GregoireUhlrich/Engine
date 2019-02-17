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
    staticDirection = 3;
    moveDirection = 0;
    nextMoveDirection = -1;
    moving = 0;
    velocity = 3*32;
    displayMode = 1;
    elapsedDisplayTime = 0;
    timeStepDisplay = 0.15;
    sizeSprite = sf::Vector2i(32,32);
    setSize(sf::Vector2i(32,32));
    texture.create(size.x,size.y);
    shiftPosition = sf::Vector2f((sizeSprite.x-size.x)/2,sizeSprite.y-size.y);
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
            //cout<<"Warning: position given to \"Character::setPosition(sf::Vector2f user_position)\" is not on grid: taking round(user_position).\n";
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
        //cout<<"Warning: call of \"Character::setGrid(bool user_grid)\" makes the character not on grid: taking round(position).\n";
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
void Character::setDisplayMode(int user_displayMode)
{
    displayMode = user_displayMode;
}

void Character::setSizeSprite(sf::Vector2i user_sizeSprite)
{
    if (user_sizeSprite.x > 0 and user_sizeSprite.y > 0)
    {
        sizeSprite = user_sizeSprite;
        if (grid and (round(position.x/sizeSprite.x) != position.x/sizeSprite.x or round(position.y/sizeSprite.y) != position.y/sizeSprite.y))
        {
            //cout<<"Warning: call of \"Character::setSizeSprite(sf::Vector2i user_sizeSprite)\" makes the character not on grid: taking round(position).\n";
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
        sprite.setTextureRect(sf::IntRect(0,0,size.x,size.y));
        updateTextureRect();
    }
    else cout<<"Invalid size in \"Character::setSize(sf::Vector2i user_size)\": cannot be equal to zero.\n";
}

bool Character::loadFromFile(string file)
{
    if (texture.loadFromFile(file))
    {
        sprite.setTexture(texture);
        updateTextureRect();
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

int convertDir(int dir)
{
    switch(dir)
    {
        case 0:
        return 2;
        break;

        case 1:
        return 3;
        break;

        case 2:
        return 1;
        break;

        default:
        return 0;
    }
}

void Character::updateTextureRect(float elapsedTime)
{
    elapsedDisplayTime += elapsedTime;
    if (elapsedDisplayTime > timeStepDisplay)
    {
        sf::IntRect fooRect;
        elapsedDisplayTime = 0;
        if (moving and displayMode == 1)
        {
            fooRect = sprite.getTextureRect();
            fooRect.left += size.x;
            fooRect.left = (fooRect.left/size.x)%4*size.x;
            fooRect.top = convertDir(moveDirection)*size.y;
            sprite.setTextureRect(fooRect);
        }
        else if (displayMode == 1)
        {
            fooRect = sprite.getTextureRect();
            fooRect.top = convertDir(staticDirection)*size.y;
            sprite.setTextureRect(fooRect);
        }
        else if (moving and displayMode == 2)
        {
            fooRect = sprite.getTextureRect();
            fooRect.left += size.x;
            fooRect.left = (fooRect.left/size.x)%7*size.x;
            if (fooRect.left < 3*size.x) fooRect.left = 3*size.x;
            fooRect.top = convertDir(moveDirection)*size.y;
            sprite.setTextureRect(fooRect);
        }
        else if (displayMode == 2)
        {
            fooRect = sprite.getTextureRect();
            fooRect.left += size.x;
            fooRect.left = (fooRect.left/size.x)%3*size.x;
            fooRect.top = convertDir(staticDirection)*size.y;
            sprite.setTextureRect(fooRect);
        }
    }
}

void Character::draw(float elapsedTime)
{
    if (grid) updateGrid(elapsedTime);
    else update(elapsedTime);
    updateTextureRect(elapsedTime);
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
    elapsedDisplayTime = user_object.elapsedDisplayTime;
    timeStepDisplay = user_object.timeStepDisplay;
    displayMode = user_object.displayMode;
    sizeSprite = user_object.sizeSprite;
    size = user_object.size;
    texture = user_object.texture;
    sprite = user_object.sprite;
    
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
                if (moving) staticDirection = moveDirection;
                if (nextMoveDirection == -1) moving = 0;
                else moveDirection = nextMoveDirection;
                elapsedDisplayTime = timeStepDisplay + 1;
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
    updateTextureRect(elapsedTime);
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

int sgn(float x)
{
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

float modulo_2pi(float theta)
{
    if (theta < 0)
    {
        int n = floor(abs(theta)/(2*M_PI));
        return theta+(n+1)*2*M_PI;
    }
    int n = floor(theta/(2*M_PI));
    return theta - 2*n*M_PI;
}

float angle(float a, float b)
{
    if (b == 0)
    {
        if (a >= 0) return 0;
        return M_PI;
    }
    if (a > 0) return atan(b/a);
    if (a < 0) return M_PI-atan(b/abs(a));
    return 0;
}

Car::Car(): Character()
{
    driftEffect = 40; // 0 = pas de drift
    accroche = 0.65; // entre 0 et 1
    theta = M_PI*3./4;
    thetaDir = theta;
    dtheta = M_PI/16;
    omega = 0;
    attenuation_omega = 3;
    maxVelocity = 800;
    acceleration = 2*maxVelocity; 
    
    currentKey = sf::Keyboard::Unknown;
    thresholdMove = 0.03;
    time = 0;
    timeKey = 0;
    keyDefinition = vector<int>(sf::Keyboard::KeyCount);
    for (int i=0; i<sf::Keyboard::KeyCount; i++)
        keyDefinition[i] = -1;
    keyDefinition[sf::Keyboard::Right] = (int)sf::Keyboard::Right;
    keyDefinition[sf::Keyboard::Left] = (int)sf::Keyboard::Left;
    keyDefinition[sf::Keyboard::Up] = (int)sf::Keyboard::Up;
    keyDefinition[sf::Keyboard::Down] = (int)sf::Keyboard::Down;

    shiftPosition = sf::Vector2f(0,0);
    grid = false;
}

Car::Car(string user_name): Character()
{
    name = user_name;
}

Car::Car(sf::RenderTarget* user_target): Car()
{
    target = user_target;
}

Car::Car(string user_name, sf::RenderTarget* user_target): Car()
{
    name = user_name;
    target = user_target;
}

Car::Car(const Car& user_object)
{
    *this = user_object;
}
    
sf::Keyboard::Key Car::getCurrentKey() const { return currentKey;}
float Car::getTime() const { return time;}
float Car::getThresholdMove() const { return thresholdMove;}
float Car::getTheta() const { return theta;}
sf::Vector2i Car::getSize() const { return size;}
sf::Vector2f Car::getVectorVelocity() const { return sf::Vector2f(velocity*cos(thetaDir),-velocity*sin(thetaDir));}

bool Car::isValid(sf::Vector2f newPos) const
{
    float deltaX = size.x/2*cos(theta) + size.y/2*sin(theta);
    float deltaY = size.y/2*cos(theta) - size.x/2*sin(theta);
    if (not bouncer.ask(sf::Vector2f(position.x+deltaX,position.y+deltaY), sizeSprite)) return false;
    if (not bouncer.ask(sf::Vector2f(position.x+deltaX,position.y-deltaY), sizeSprite)) return false;
    if (not bouncer.ask(sf::Vector2f(position.x-deltaX,position.y+deltaY), sizeSprite)) return false;
    if (not bouncer.ask(sf::Vector2f(position.x-deltaX,position.y-deltaY), sizeSprite)) return false;
    return true;
}

void Car::setPositionMap(sf::Vector2i posMouse)
{
    sf::Vector2f pos = target->mapPixelToCoords(posMouse);
    position.x = pos.x;
    position.y = pos.y;
    sprite.setPosition(position);
}

void Car::setTime(float user_time)
{
    time = user_time;
}
void Car::setThresholdMove(float user_thresholdMove)
{
    thresholdMove = user_thresholdMove;
}

void Car::setKeyDefinition(sf::Keyboard::Key keyToPress, sf::Keyboard::Key keyToUnderstand)
{
    keyDefinition[keyToUnderstand] = (int)keyToPress;
}

void Car::setVelocity(float t_velocity)
{
    if (abs (t_velocity) < maxVelocity)
        velocity = t_velocity;
    else
        velocity = maxVelocity*sgn(t_velocity);
}

void Car::setVectorVelocity(sf::Vector2f vector)
{
    velocity = norm(vector);
    thetaDir = angle(-vector.y,vector.x);
}

void Car::addAngularVelocity(float t_omega)
{
    omega += t_omega;
}

void Car::testEvent(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        float old = maxVelocity;
        if (event.key.code == sf::Keyboard::Add)
            maxVelocity += 100;
        else if (event.key.code == sf::Keyboard::Subtract)
            maxVelocity -= 100;
        acceleration *= maxVelocity/old;
    }
}

void Car::draw(float elapsedTime)
{
    time += elapsedTime;
    float oldTheta = theta;
    float dv = acceleration*elapsedTime;
    thetaDir += omega*elapsedTime;
    theta += omega*elapsedTime;
    bool keyPressed = false;
    if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)(keyDefinition[sf::Keyboard::Up])))
    {
        moving = true;
        keyPressed = true;
        velocity = min(maxVelocity, velocity + dv);
    }
    else if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)(keyDefinition[sf::Keyboard::Down])))
    {
        moving = true;
        keyPressed = true;
        velocity = max(-1*maxVelocity, velocity - dv);
    }
    else
    {
        if (abs(velocity) < dv/2) velocity = 0;
        else velocity -= sgn(velocity)*dv/2;
        if (velocity == 0)
        {
            moving = false;
            thetaDir = theta;
        }
    }
    if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)(keyDefinition[sf::Keyboard::Left])))
    {
        keyPressed = true;
        if (currentKey == sf::Keyboard::Left)
        {
            timeKey += elapsedTime;
            if (timeKey > thresholdMove)
            {
                theta -= dtheta;
                timeKey = 0;
            }
        }
        else
            currentKey = sf::Keyboard::Left;
    }
    else if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)(keyDefinition[sf::Keyboard::Right])))
    {
        keyPressed = true;
        if (currentKey == sf::Keyboard::Right)
        {
            timeKey += elapsedTime;
            if (timeKey > thresholdMove)
            {
                theta += dtheta;
                timeKey = 0;
            }
        }
        else
            currentKey = sf::Keyboard::Right;
    }
    else currentKey = sf::Keyboard::Unknown;
    theta = modulo_2pi(theta);
    if (driftEffect > 0 and keyPressed)
    {
        float velocity_t[2]{sin(thetaDir),-cos(thetaDir)};
        float velocity_d[2]{sin(theta), -cos(theta)};
        float alpha = (abs(velocity)/maxVelocity)*driftEffect*(1-accroche*abs(cos(theta-thetaDir)));
        float velocity_t_dt[2]{(alpha*velocity_t[0]+velocity_d[0]),(alpha*velocity_t[1]+velocity_d[1])};
        thetaDir = angle(-velocity_t_dt[1],velocity_t_dt[0]);
    }
    else if (keyPressed) thetaDir = theta;
    if (omega != 0)
    {
        float attenuation = attenuation_omega*(1+3*keyPressed);
        if (abs(omega) < elapsedTime*attenuation)
            omega = 0;
        else
            omega -= sgn(omega)*attenuation*elapsedTime;
    }
    if (time > thresholdMove) time = 0;
    if (moving or velocity != 0)
    {
        sf::Vector2f newPos;
        newPos.x = position.x+velocity*elapsedTime*sin(thetaDir);
        newPos.y = position.y-velocity*elapsedTime*cos(thetaDir);
        if (isValid(newPos))
        {
            position = newPos;
        }
        //sprite.move(+velocity*elapsedTime*sin(thetaDir), -velocity*elapsedTime*cos(thetaDir));
    }
    sf::FloatRect rect = sprite.getLocalBounds();
    sprite.setOrigin(rect.width/2, rect.height/2);
    sprite.setPosition(position);
    sprite.setRotation(theta*180/M_PI);

    target->draw(sprite);
}


Car& Car::operator=(const Car& user_object)
{
    Character::operator=(user_object);
    currentKey = user_object.currentKey;
    time = user_object.time;
    thresholdMove = user_object.thresholdMove;

    return *this;
}

ostream& operator<<(ostream& f, const Car& user_object)
{
    Character foo = user_object;
    cout<<foo;
    cout<<"CurrentKey: "<<user_object.currentKey<<endl;
    cout<<"ElapsedTime: "<<user_object.time<<endl;
    
    return f;
}

float distance(sf::Vector2i v1, sf::Vector2i v2)
{
    return sqrt(pow(v1.x-v2.x,2)+pow(v1.y-v2.y,2));
}
float distance(sf::Vector2f v1, sf::Vector2f v2)
{
    return sqrt(pow(v1.x-v2.x,2)+pow(v1.y-v2.y,2));
}
float scalar(sf::Vector2f v1, sf::Vector2f v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}
float cross(sf::Vector2f v1, sf::Vector2f v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}
float norm(sf::Vector2i v)
{
    return sqrt(v.x*v.x+v.y*v.y);
}
float norm(sf::Vector2f v)
{
    return sqrt(v.x*v.x+v.y*v.y);
}
sf::Vector2f rotation(float theta, sf::Vector2f v)
{
    sf::Vector2f rep;
    rep.x = cos(theta)*v.x - sin(theta)*v.y;
    rep.y = cos(theta)*v.y + sin(theta)*v.x;

    return rep;
}

void testCollision(Car* car1,Car* car2)
{
    sf::Vector2f pos1 = car1->getPosition();
    sf::Vector2f pos2 = car2->getPosition();
    sf::Vector2i size1 = car1->getSize();
    sf::Vector2i size2 = car2->getSize();
    float theta1 = car1->getTheta();
    float theta2 = car2->getTheta();
    if (norm(size1)/2+norm(size2)/2 < distance(pos1,pos2))
    {
        return;
    }
    sf::Vector2f v1 = car1->getVectorVelocity();
    sf::Vector2f v2 = car2->getVectorVelocity();

    sf::Vector2f coin;
    float deltaX = size1.x/2;
    float deltaY = size1.y/2;
    coin.x = deltaX;
    coin.y = deltaY;
    coin = rotation(theta1,coin);
    coin.x += pos1.x - pos2.x;
    coin.y += pos1.y - pos2.y;
    coin = rotation(-theta2,coin);
    if (coin.x > -size2.x/2 and coin.x < size2.x/2 and coin.y > -size2.y/2 and coin.y < size2.y/2)
    {
        Collision(car2, car1, coin);
    }
    coin.x = deltaX;
    coin.y = -deltaY;
    coin = rotation(theta1,coin);
    coin.x += pos1.x - pos2.x;
    coin.y += pos1.y - pos2.y;
    coin = rotation(-theta2,coin);
    if (coin.x > -size2.x/2 and coin.x < size2.x/2 and coin.y > -size2.y/2 and coin.y < size2.y/2)
    {
        Collision(car2, car1, coin);
    }
    coin.x = -deltaX;
    coin.y = deltaY;
    coin = rotation(theta1,coin);
    coin.x += pos1.x - pos2.x;
    coin.y += pos1.y - pos2.y;
    coin = rotation(-theta2,coin);
    if (coin.x > -size2.x/2 and coin.x < size2.x/2 and coin.y > -size2.y/2 and coin.y < size2.y/2)
    {
        Collision(car2, car1, coin);
    }
    coin.x = -deltaX;
    coin.y = -deltaY;
    coin = rotation(theta1,coin);
    coin.x += pos1.x - pos2.x;
    coin.y += pos1.y - pos2.y;
    coin = rotation(-theta2,coin);
    if (coin.x > -size2.x/2 and coin.x < size2.x/2 and coin.y > -size2.y/2 and coin.y < size2.y/2)
    {
        Collision(car2, car1, coin);
    }
    deltaX = size2.x/2;
    deltaY = size2.y/2;
    coin.x = deltaX;
    coin.y = deltaY;
    coin = rotation(theta2,coin);
    coin.x += pos2.x - pos1.x;
    coin.y += pos2.y - pos1.y;
    coin = rotation(-theta1,coin);
    if (coin.x > -size1.x/2 and coin.x < size1.x/2 and coin.y > -size1.y/2 and coin.y < size1.y/2)
    {
        Collision(car1, car2, coin);
    }
    coin.x = deltaX;
    coin.y = -deltaY;
    coin = rotation(theta2,coin);
    coin.x += pos2.x - pos1.x;
    coin.y += pos2.y - pos1.y;
    coin = rotation(-theta1,coin);
    if (coin.x > -size1.x/2 and coin.x < size1.x/2 and coin.y > -size1.y/2 and coin.y < size1.y/2)
    {
        Collision(car1, car2, coin);
    }
    coin.x = -deltaX;
    coin.y = deltaY;
    coin = rotation(theta2,coin);
    coin.x += pos2.x - pos1.x;
    coin.y += pos2.y - pos1.y;
    coin = rotation(-theta1,coin);
    if (coin.x > -size1.x/2 and coin.x < size1.x/2 and coin.y > -size1.y/2 and coin.y < size1.y/2)
    {
        Collision(car1, car2, coin);
    }
    coin.x = -deltaX;
    coin.y = -deltaY;
    coin = rotation(theta2,coin);
    coin.x += pos2.x - pos1.x;
    coin.y += pos2.y - pos1.y;
    coin = rotation(-theta1,coin);
    if (coin.x > -size1.x/2 and coin.x < size1.x/2 and coin.y > -size1.y/2 and coin.y < size1.y/2)
    {
        Collision(car1, car2, coin);
    }
}

void Collision(Car* car1, Car* car2, sf::Vector2f coin) // car2 rentre dans car 1 par l'angle coin (0 = (+,+), 1 = (+,-), 2 = (-,+), 3 = (-,-))
{
    sf::Vector2f pos1 = car1->getPosition();
    sf::Vector2f pos2 = car2->getPosition();
    sf::Vector2i size1 = car1->getSize();
    sf::Vector2i size2 = car2->getSize();
    float theta1 = car1->getTheta();
    float theta2 = car2->getTheta();

    //cout<<car2->getName()<<" in "<<car1->getName()<<endl;
    //cout<<"Pos rel = "<<pos2.x<<"  "<<pos2.y<<endl;
    //cout<<"Angle rel: "<<theta2<<endl;

    pos2.x -= pos1.x;
    pos2.y -= pos1.y;
    pos2 = rotation(-theta1,pos2);
    sf::Vector2f oldCoin;

    float XYCollision = size1.y*(0.5-pos2.x/size1.x);

    if (pos2.y > XYCollision xor pos2.y > -XYCollision)
    {
       // X Collision 
       float deltaX;
       if (coin.x < 0)
           deltaX = -size1.x/2-coin.x;
       else
           deltaX = size1.x/2-coin.x;
       oldCoin = coin;
       coin.x += deltaX/2;
       pos2.x = pos2.x+deltaX/2;
       pos2.y = pos2.y;
       pos2 = rotation(theta1,pos2);
       pos2.x += pos1.x;
       pos2.y += pos1.y;
       coin = rotation(theta1,coin);
       coin += pos1;
       oldCoin = rotation(theta1,oldCoin);
       oldCoin += pos1;
       pos1.x -= cos(theta1)*deltaX/2;
       pos1.y -= sin(theta1)*deltaX/2;
       car1->setPosition(pos1);
       car2->setPosition(pos2);
    }
    else
    {
        // Y Collision
       float deltaY;
       if (coin.y < 0)
           deltaY = -size1.y/2-coin.y;
       else
           deltaY = size1.y/2-coin.y;
       pos2.x = pos2.x;
       oldCoin = coin;
       coin.y += deltaY/2;
       pos2.y = pos2.y+deltaY/2;
       pos2 = rotation(theta1,pos2);
       pos2.x += pos1.x;
       pos2.y += pos1.y;
       coin = rotation(theta1,coin);
       coin += pos1;
       oldCoin = rotation(theta1,oldCoin);
       oldCoin += pos1;
       pos1.y -= cos(theta1)*deltaY/2;
       pos1.x += sin(theta1)*deltaY/2;
       car1->setPosition(pos1);
       car2->setPosition(pos2);
    }

    sf::Vector2f v1 = car1->getVectorVelocity();
    sf::Vector2f v2 = car2->getVectorVelocity();

    sf::Vector2f er = oldCoin-coin;
    if (norm(er) > 0)
        er = er / norm(er);

    float alpha = 0.75;
    float gamma = 1e-4;
    sf::Vector2f v_para = alpha*(er * scalar(er,v2-v1));

    sf::Vector2f force = er;

    force = force*gamma*norm(v_para);
    er = coin-pos1;
    car1->addAngularVelocity(cross(er,force));

    er = coin-pos2;
    car2->addAngularVelocity(-cross(er,force));

    v1 += v_para;
    v2 = -v2 + v_para;
    car1->setVectorVelocity(v1);
    car2->setVectorVelocity(v2);
}
