#include "map.h" 
using namespace std;

//////////////////////////////////////
/////////// MAP //////////////////////
//////////////////////////////////////

Map::Map(): PNG_Encoder()
{
    position = sf::Vector2f(0,0);
    textureMap = vector<sf::RenderTexture >(nPrio);
    spriteMap = vector<sf::Sprite>(nPrio);
    for (int i=0; i<nPrio; i++)
        spriteMap[i].setPosition(position);
    spritePlayers.setPosition(position);
}

Map::Map(string user_fileMap): Map()
{
    fileMap = user_fileMap;
}
    
sf::Vector2f Map::getPosition() const { return position;}
vector<sf::Sprite > Map::getRenderSprite() const { return spriteMap;}
sf::RenderTexture* Map::getPlayerTexture() { return &players;}

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
            players.create(sizeMap.x*sizeSprite.x,sizeMap.y*sizeSprite.y);
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
            players.setView(fooView);
            textureMap[i].display();
            players.display();
            spriteMap[i] = sf::Sprite();
            spriteMap[i].setTexture(textureMap[i].getTexture());
            spritePlayers.setTexture(players.getTexture());
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
    gameMode = false;
    centerViewGame = sf::Vector2f(0,0);
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

    isGhostSprite = false;
    ghostPosition = sf::Vector2i(0,0);
    ghostFileTexture = "";

    select = false;
    select2 = false;
    selectRect.setFillColor(sf::Color::Transparent);
    selectRect.setOutlineColor(sf::Color(84,106,139));
    selectRect.setOutlineThickness(3.);
    
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

void IMap::setGameMode(bool t_gameMode) { gameMode = t_gameMode;}

void IMap::setCenterViewGame(sf::Vector2f centerView) { centerViewGame = centerView;}

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

void IMap::setGhost(string fileTexture, sf::Vector2i nSpriteToPull, sf::Vector2i posSpriteToPull)
{
    int indexTexture = addTexture(fileTexture);
    ghostTexture.create(nSpriteToPull.x*sizeSprite.x,nSpriteToPull.y*sizeSprite.y);
    sf::Sprite sprite;
    sprite.setTexture(texture[indexTexture]);
    spriteRect = sf::IntRect(posSpriteToPull.x,posSpriteToPull.y,nSpriteToPull.x*sizeSprite.x,nSpriteToPull.y*sizeSprite.y);
    sprite.setTextureRect(spriteRect);
    ghostTexture.draw(sprite);
    ghostTexture.display();
    isGhostSprite = true;
}

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
        if (state == ADD and isGhostSprite)
        {
            sf::Vector2f foo = bigRenderTexture.mapPixelToCoords(posMouse);
            sf::Vector2u fooSize = ghostTexture.getSize();
            foo.x = round((foo.x-fooSize.x)/sizeSprite.x)*sizeSprite.x;
            foo.y = round((foo.y-fooSize.y)/sizeSprite.y)*sizeSprite.y;
            posClick = bigRenderTexture.mapCoordsToPixel(foo);
        }
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

    sf::Vector2f effectivePosMouse = bigRenderTexture.mapPixelToCoords(posMouse);
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
            sf::Vector2f fooMouse = bigRenderTexture.mapPixelToCoords(posMouse);
            sf::Vector2f fooClick = bigRenderTexture.mapPixelToCoords(posClick);
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
            //bigRenderTexture.setView(view);
            
            delta = 0;

            posMouse = bigRenderTexture.mapCoordsToPixel(fooMouse);
            posClick = bigRenderTexture.mapCoordsToPixel(fooClick);
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
    if (isMousePressed && (state == ADD || state == SELECT))
    {
        if (!select || (select && select2)) 
        {
            select = 1;
            select2 = 0;
            /*if (state == adding)
            {
                sf::Vector2f foo2 = viewMap.getSize();
                sf::Vector2u foo;
                if (isImLeft)
                    foo = imL->getNSprites();
                if (foo.x != 0)
                {
                    posClick = invConvertPos(spriteToAdd.getPosition())+sf::Vector2i(1,1);
                }
                else if (ghostSpriteCtrlC)
                {
                    int foo;
                    if (prioCtrlC == 4) foo = 0;
                    else foo = prioCtrlC;
                    posClick = invConvertPos(spriteCtrlCVec[foo][0][0].getPosition())+sf::Vector2i(1,1);
                }
            }*/
        }
        if (select)
        {
            sf::Vector2f clickEff = bigRenderTexture.mapPixelToCoords(posClick);
            //clickEff.x = floor(clickEff.x/sizeSprite.x)*sizeSprite.x;
            //clickEff.y = floor(clickEff.y/sizeSprite.y)*sizeSprite.y;
            effectivePosMouse.x = round(effectivePosMouse.x/sizeSprite.x)*sizeSprite.x;
            effectivePosMouse.y = round(effectivePosMouse.y/sizeSprite.y)*sizeSprite.y;
            int xmin = min(clickEff.x, effectivePosMouse.x);
            int xmax = max(clickEff.x, effectivePosMouse.x);
            int ymin = min(clickEff.y, effectivePosMouse.y);
            int ymax = max(clickEff.y, effectivePosMouse.y);
            xmin = max(xmin, 0);
            xmax = min(xmax, sizeMap.x*sizeSprite.x);
            ymin = max(ymin, 0);
            ymax = min(ymax, sizeMap.y*sizeSprite.y);
    
            selectRect.setPosition(xmin, ymin);
            selectRect.setSize(sf::Vector2f(xmax-xmin, ymax-ymin));
        }
    }
    else if (select)
    {
        select2 = 1;
        sf::Vector2f foo;
        sf::Vector2f foo2;
        foo = selectRect.getPosition();
        foo2 = selectRect.getSize();
        float xmin = round(foo.x/sizeSprite.x)*sizeSprite.x;
        float ymin = round(foo.y/sizeSprite.y)*sizeSprite.y;
        float xmax = max(xmin+sizeSprite.x,(float)round((foo.x+foo2.x)/sizeSprite.x)*sizeSprite.x);
        float ymax = max(ymin+sizeSprite.y,(float)round((foo.y+foo2.y)/sizeSprite.y)*sizeSprite.y);
        if (xmin < 0) xmin = 0;
        if (xmax > sizeMap.x*sizeSprite.x)  xmax = sizeMap.x*sizeSprite.x;
        if (ymin < 0) ymin = 0;
        if (ymax > sizeMap.y*sizeSprite.y)  ymax = sizeMap.y*sizeSprite.y;
        selectRect.setPosition(xmin,ymin);
        selectRect.setSize(sf::Vector2f(xmax-xmin, ymax-ymin));
        if (state == ADD)
        {
            if (isGhostSprite)
            {
                addSpriteGhost();
                select = select2 = 0;
            }
        }
    }
}

void IMap::addSpriteGhost()
{
    /*if (prio < 0 || prio > 3)
    {
        cout<<"Invalid priority !\n";
        return;
    }
    //saveState = edited;
    sprite fooSprite;
    sf::IntRect fooRect;
    string fooTexture;
    sf::vector2i sizeTot = ghostTexture.getSize();
    sizeTot.x /= sizeSprite.x;
    sizeTot.y /= sizeSprite.y;
    for (int ix=ghostPosition.x; ix<ghostPosition.x+sizeTot.x; ix++)
    {
        for (int iy=ghostPosition.x; iy<s.y/ySprites+lyS; iy++)
        {
            if (indexSpriteVec[prio][ix][iy][0] != -1)
            {
                fooSprite.x = ix*xSprites;
                fooSprite.y = iy*ySprites;
                fooRect = spriteVec[prio][indexSpriteVec[prio][ix][iy][0]][indexSpriteVec[prio][ix][iy][1]].getTextureRect();
                fooSprite.xPNG = fooRect.left;
                fooSprite.yPNG = fooRect.top;
                fooTexture = fileTextureVec[iTextureVec[prio][indexSpriteVec[prio][ix][iy][0]]];
                ctrlZObject->saveErasing(fooSprite, prio, passOrNotVec[prio][ix][iy], fooTexture);
            }
            removeSprite(prio, ix, iy);
        }
    }
    ctrlZObject->saveAdding(s, prio, lxS, lyS);
    int indexTexture = -1;
    for (int j=0; j<nTextureVec[prio]; j++)
    {
        if (t == fileTextureVec[iTextureVec[prio][j]])
            indexTexture = j;
    }
    if (indexTexture == -1)
    {
        sf::Sprite** foo3 = new sf::Sprite*[nTextureVec[prio]+1];
        for (int j=0; j<nTextureVec[prio]; j++)
        {
            foo3[j] = new sf::Sprite[nSpriteVec[prio][j]];
            for (int k=0; k<nSpriteVec[prio][j]; k++)
                foo3[j][k] = spriteVec[prio][j][k];
        }
        iTextureVec[prio].push_back(addTexture(t));
        nSpriteVec[prio].push_back(lxS*lyS);
        foo3[nTextureVec[prio]] = new sf::Sprite[lxS*lyS];
        sprite fooS = s;
        spriteVec[prio].push_back(vector<sf::Sprite>(0));
        for (int ix=0; ix<lxS; ix++)
        {
            for (int iy=0; iy<lyS; iy++)
            {
                fooS.x = s.x + ix*xSprites;
                fooS.y = s.y + iy*ySprites;
                fooS.xPNG = s.xPNG + (ix%nxS)*xSprites;
                fooS.yPNG = s.yPNG + (iy%nyS)*ySprites;
                foo3[nTextureVec[prio]][ix*lyS+iy].setTextureRect(sf::IntRect(fooS.xPNG,fooS.yPNG,xSprites,ySprites));
                foo3[nTextureVec[prio]][ix*lyS+iy].setPosition(fooS.x,fooS.y);
                spriteVec[prio][nTextureVec[prio]].push_back(foo3[nTextureVec[prio]][ix*lyS+iy]);
                indexSpriteVec[prio][s.x/xSprites+ix][s.y/ySprites+iy][0] = nTextureVec[prio];
                indexSpriteVec[prio][s.x/xSprites+ix][s.y/ySprites+iy][1] = ix*lyS+iy;
            }
        }
        
        nTextureVec[prio] += 1;
        
        for (int j=0; j<nTextureVec[prio]; j++)
        {
            delete[] foo3[j];
            foo3[j] = 0;
        }
        delete[] foo3;
        foo3 = 0;
    }
    else
    {
        sf::Sprite* foo = new sf::Sprite[nSpriteVec[prio][indexTexture]+lxS*lyS];
        for (int k=0; k<nSpriteVec[prio][indexTexture]; k++)
            foo[k] = spriteVec[prio][indexTexture][k];
        
        for (int ix=0; ix<lxS; ix++)
        {
            for (int iy=0; iy<lyS; iy++)
            {
                foo[nSpriteVec[prio][indexTexture]+ix*lyS+iy].setTexture(textureVec[iTextureVec[prio][indexTexture]]);
                foo[nSpriteVec[prio][indexTexture]+ix*lyS+iy].setTextureRect(sf::IntRect(s.xPNG+ (ix%nxS)*xSprites,s.yPNG + (iy%nyS)*ySprites,xSprites,ySprites));
                foo[nSpriteVec[prio][indexTexture]+ix*lyS+iy].setPosition(s.x + ix*xSprites,s.y + iy*ySprites);
                spriteVec[prio][indexTexture].push_back(foo[nSpriteVec[prio][indexTexture]+ix*lyS+iy]);
                indexSpriteVec[prio][s.x/xSprites+ix][s.y/ySprites+iy][0] = indexTexture;
                indexSpriteVec[prio][s.x/xSprites+ix][s.y/ySprites+iy][1] = nSpriteVec[prio][indexTexture]+ix*lyS+iy;
            }
        }
        nSpriteVec[prio][indexTexture] += lxS*lyS;
            
        delete[] foo;
        foo = 0;
    }
    resetTextureSprite();
    */
}

void IMap::draw(float elapsedTime)
{
    updateSprite();
    bigRenderTexture.clear(colorTexture);
    spritePlayers.setPosition(0,0);
    players.display();
    spritePlayers.setTexture(players.getTexture());
    for (int i=0; i<nPrio; i++)
    {
        spriteMap[i].setPosition(0,0);
        if (allPrio or i==prio)
            spriteMap[i].setColor(sf::Color(255,255,255,255));
        else
            spriteMap[i].setColor(sf::Color(255,255,255,64));
        bigRenderTexture.draw(spriteMap[i]);
        if (i == nPrio/2)
            bigRenderTexture.draw(spritePlayers);
    }
    players.clear(sf::Color::Transparent);
    if (grid and not gameMode)
    {
        for (int i=0; i<sizeMap.x-1; i++)
            bigRenderTexture.draw(gridX[i]);
        for (int i=0; i<sizeMap.y-1; i++)
            bigRenderTexture.draw(gridY[i]);
    }
    if (pass and not gameMode)
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
    if (isGhostSprite and state == ADD and not gameMode)
    {
        sf::Sprite ghostSprite;
        sf::Texture fooTexture = ghostTexture.getTexture();
        sf::Vector2u foo = ghostTexture.getSize();
        if (select)
        {
            fooTexture.setRepeated(true);
            ghostSprite.setTexture(fooTexture);
            sf::Vector2f fooSize = selectRect.getSize();
            sf::Vector2f fooPos = selectRect.getPosition();
            ghostSprite.setPosition(fooPos);
            ghostSprite.setTextureRect(sf::IntRect(0,0,fooSize.x,fooSize.y));
        }
        else
        {
            ghostSprite.setTexture(fooTexture);
            sf::Vector2f effectivePosMouse = bigRenderTexture.mapPixelToCoords(posMouse);
            effectivePosMouse.y = round(effectivePosMouse.y/sizeSprite.y)*sizeSprite.y;
            effectivePosMouse.x = round(effectivePosMouse.x/sizeSprite.x)*sizeSprite.x;
            ghostSprite.setPosition(effectivePosMouse.x-foo.x,effectivePosMouse.y-foo.y);
        }
        ghostSprite.setColor(sf::Color(255,255,255,123));
        bigRenderTexture.draw(ghostSprite);
    }
    if (select and (state == ADD or state == SELECT) and not gameMode)
        bigRenderTexture.draw(selectRect);

    bigRenderTexture.draw(outline);
    if (not gameMode) bigRenderTexture.setView(view);
    else
    {
        sf::Vector2u size = target->getSize();
        float xView = centerViewGame.x-size.x/2;
        float yView = centerViewGame.y-size.y/2;
        if (xView < 0) xView = 0;
        if (xView > sizeMap.x*sizeSprite.x-size.x) xView = sizeMap.x*sizeSprite.x-size.x;    
        if (yView < 0) yView = 0;
        if (yView > sizeMap.y*sizeSprite.y-size.y) yView = sizeMap.y*sizeSprite.y-size.y;        
        if (size.x > sizeMap.x*sizeSprite.x) xView = -(size.x-sizeMap.x*sizeSprite.x)/2;
        if (size.y > sizeMap.y*sizeSprite.y) yView = -(size.y-sizeMap.y*sizeSprite.y)/2;
        bigRenderTexture.setView(sf::View(sf::FloatRect(round(xView), round(yView),(float)size.x,(float)size.y)));
    }
    bigRenderTexture.display();
    bigRenderSprite.setTexture(bigRenderTexture.getTexture());
    bigRenderSprite.setPosition(0,0);
    if (target) target->draw(bigRenderSprite);
}
