#include "map.h" 
using namespace std;

//////////////////////////////////////
/////////// MAP //////////////////////
//////////////////////////////////////

Map::Map(): PNG_Encoder()
{
    nPrio = 4;
    position = sf::Vector2f(0,0);
    textureMap = vector<sf::RenderTexture >(nPrio);
    spriteMap = vector<sf::Sprite>(nPrio);
    for (int i=0; i<nPrio; i++)
        spriteMap[i].setPosition(position);
}

Map::Map(string user_fileMap): Map()
{
    fileMap = user_fileMap;
}
    
sf::Vector2f Map::getPosition() const { return position;}
vector<sf::Sprite > Map::getRenderSprite() const { return spriteMap;}

void Map::setPosition(sf::Vector2f user_position)
{
    position= user_position;
    for (int i=0; i<nPrio; i++)
        spriteMap[i].setPosition(position);
}
void Map::setSizeSprite(sf::Vector2i user_sizeSprite)
{
    sizeSprite = user_sizeSprite;
    updateSprite();
}

void Map::addException(sf::Vector2i positionException, int direction)
{
    bouncer.addException(positionException,direction);
}

void Map::updateSprite()
{
    if (sizeMap.x*sizeSprite.x > 0 and sizeMap.y*sizeSprite.y > 0)
    {
        for (int i=0; i<nPrio; i++)
        {
            textureMap[i].create(sizeMap.x*sizeSprite.x,sizeMap.y*sizeSprite.y);
            textureMap[i].clear(sf::Color::Transparent);
            for (int j=0; j<nTextureInPrio[i]; j++)
            {
                for (int k=0; k<nSprite[i][j]; k++)
                {
                    sprite[i][j][k].setTexture(texture[iTexture[i][j]]);
                    textureMap[i].draw(sprite[i][j][k]);
                }
            }
            sf::View fooView(sf::FloatRect(0,0,sizeMap.x*sizeSprite.x,sizeMap.y*sizeSprite.y));
            textureMap[i].setView(fooView);
            textureMap[i].display();
            spriteMap[i].setTexture(textureMap[i].getTexture());
        }
    }
}

int Map::load()
{
    int foo = PNG_Encoder::load();
    updateSprite();
    return foo;
}
int Map::loadTxt(string stringFile)
{
    int foo = PNG_Encoder::loadTxt(stringFile);
    updateSprite();
    cout<<"Sprites updated\n";
    return foo;
}

ostream& operator<<(ostream& f, const Map& user_object)
{
    PNG_Encoder foo(user_object);
    cout<<foo;
    return f;
}

IMap::IMap(): Map()
{
    isMouseHere = isMousePressed = 0;
    posMouse = posClick = sf::Vector2i(0,0);
    delta = 0;
    zoom.iZoom = 0.95;
    zoom.minZoom = 0.1;
    zoom.maxZoom = 10;
    state = MOVE;
    bigRenderSprite.setPosition(0,0);
    int color = 200;
    colorTexture = sf::Color(color,color,color);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineThickness(3);
    outline.setOutlineColor(sf::Color::White);
    outline.setPosition(0,0);

    prio = 0;
    allPrio = true;

    grid = 0;
    gridX = vector<sf::RectangleShape>(0);
    gridY = vector<sf::RectangleShape>(0);
    pass = 0;
}

IMap::IMap(sf::RenderTarget* user_target): IMap()
{
    bigRenderTexture.create(user_target->getSize().x, user_target->getSize().y);
    view = sf::View(sf::FloatRect(0,0,user_target->getSize().x,user_target->getSize().y));
    target = user_target;
}

IMap::IMap(sf::RenderTarget* user_target, string user_fileMap): IMap(user_target)
{
    fileMap = user_fileMap;
}

IMap::~IMap()
{
}

StateMap IMap::getState() const { return state;}

void IMap::setState(StateMap user_state) { state = user_state;}

void IMap::testMouse(sf::Vector2i user_posMouse)
{
    posMouse = user_posMouse;
}

void IMap::setIsMouseHere(bool user_isMouseHere)
{
    isMouseHere = user_isMouseHere;
}
void IMap::setAllPrio(bool user_allPrio) { allPrio = user_allPrio;}
void IMap::setPrio(int user_prio) { prio = user_prio;}
void IMap::setGrid(bool user_grid) { grid = user_grid;}
void IMap::setPass(bool user_pass) { pass = user_pass;}

void IMap::testEvent(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
            mousePressed();
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
            mouseReleased();
    }
    else if (event.type == sf::Event::MouseWheelMoved)
    {
        scrollMouse(event.mouseWheel.delta);
    }
    else if (event.type == sf::Event::Resized)
        windowResized();
}

void IMap::mousePressed()
{
    if (isMouseHere and !isMousePressed)
    {
        isMousePressed = 1;
        posClick = posMouse;
    }
}
void IMap::mouseReleased()
{
    isMouseHere = 0;
    isMousePressed = 0;
}
void IMap::scrollMouse(int user_delta)
{
    if (isMouseHere) delta += user_delta;
}
void IMap::windowResized()
{
    bigRenderTexture.create(target->getSize().x, target->getSize().y);
    view.setSize((float)target->getSize().x, (float)target->getSize().y);
    bigRenderSprite = sf::Sprite();
    bigRenderSprite.setTexture(bigRenderTexture.getTexture());
}

void IMap::updateSprite()
{
    //Map::updateSprite();
    if (bigRenderTexture.getSize() != target->getSize())
        windowResized();

    outline.setSize(sf::Vector2f(sizeSprite.x*sizeMap.x,sizeSprite.y*sizeMap.y));
    if (grid)
    {
        int sizeGrid = 1;
        if (gridX.size() != sizeMap.x or gridY.size() != sizeMap.y)
        {
            gridX = vector<sf::RectangleShape>(sizeMap.x-1);
            gridY = vector<sf::RectangleShape>(sizeMap.y-1);
            
            for (int i=1; i<sizeMap.x; i++)
            {
                gridX[i-1] = sf::RectangleShape(sf::Vector2f(sizeGrid, sizeMap.y*sizeSprite.y));
                gridX[i-1].setPosition(i*sizeSprite.x-sizeGrid/2,0);
                gridX[i-1].setFillColor(sf::Color(255,255,255,128));
            }
            for (int i=1; i<sizeMap.y; i++)
            {
                gridY[i-1] = sf::RectangleShape(sf::Vector2f(sizeMap.x*sizeSprite.x,sizeGrid));
                gridY[i-1].setPosition(0,i*sizeSprite.y-sizeGrid/2);
                gridY[i-1].setFillColor(sf::Color(255,255,255,128));        
            }
        }
    }
            
    if (isMouseHere)
    {
        if (delta != 0)
        {
            sf::Vector2f effectivePosMouse = bigRenderTexture.mapPixelToCoords(posMouse);
            sf::Vector2u size = target->getSize();
            sf::Vector2f center = view.getCenter();
            sf::Vector2f fooSize = view.getSize();
            
            float ratioZoom = pow(zoom.iZoom,delta);
            if (fooSize.x/size.x*ratioZoom > zoom.maxZoom)
                ratioZoom = zoom.maxZoom*size.x/fooSize.x;
            else if (fooSize.x/size.x*ratioZoom < zoom.minZoom)
                ratioZoom = zoom.minZoom*size.x/fooSize.x;
            
            center.x = (center.x - effectivePosMouse.x)*ratioZoom;
            center.y = (center.y - effectivePosMouse.y)*ratioZoom;
            center = center + effectivePosMouse;
            fooSize.x *= ratioZoom;
            fooSize.y *= ratioZoom;
            view.setCenter(center);
            view.setSize(fooSize);
            
            delta = 0;
        }

        if (isMousePressed)
        {
            if (state == MOVE)
            {
                sf::Vector2u size = target->getSize();
                sf::Vector2f fooSize = view.getSize();
                sf::Vector2f offset;
                offset.x = posClick.x-posMouse.x;
                offset.y = posClick.y-posMouse.y;
                offset.x *= fooSize.x/size.x;
                offset.y *= fooSize.y/size.y;
                view.move(offset);
                posClick = posMouse;
            }
        }
    }
}

void IMap::draw(float elapsedTime)
{
    updateSprite();
    bigRenderTexture.clear(colorTexture);
    for (int i=0; i<nPrio; i++)
    {
        spriteMap[i].setPosition(0,0);
        if (allPrio or i==prio)
            spriteMap[i].setColor(sf::Color(255,255,255,255));
        else
            spriteMap[i].setColor(sf::Color(255,255,255,64));
        bigRenderTexture.draw(spriteMap[i]);
    }
    if (grid)
    {
        for (int i=0; i<sizeMap.x-1; i++)
            bigRenderTexture.draw(gridX[i]);
        for (int i=0; i<sizeMap.y-1; i++)
            bigRenderTexture.draw(gridY[i]);
    }
    if (pass)
    {
        sf::RectangleShape fooRect;
        fooRect.setSize(sf::Vector2f(sizeSprite.x,sizeSprite.y));
        fooRect.setFillColor(sf::Color(255,0,0,64));
        for (int i=0; i<sizeMap.x; i++)
        {
            for (int j=0; j<sizeMap.y; j++)
            {
                if (!bouncer.ask(sf::Vector2i(i,j)))
                {
                    fooRect.setPosition(i*sizeSprite.x,j*sizeSprite.y);
                    bigRenderTexture.draw(fooRect);
                }
            }
        }
    }

    bigRenderTexture.draw(outline);
    bigRenderTexture.display();
    bigRenderTexture.setView(view);
    bigRenderSprite.setTexture(bigRenderTexture.getTexture());
    bigRenderSprite.setPosition(0,0);
    if (target) target->draw(bigRenderSprite);
}
