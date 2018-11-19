#include "map.h" 
using namespace std;

//////////////////////////////////////
/////////// MAP //////////////////////
//////////////////////////////////////

Map::Map(): PNG_Encoder()
{
    position = sf::Vector2f(0,0);
    renderSprite.setPosition(position);
}

Map::Map(sf::RenderTarget* user_target)
{
    Map();
    target = user_target;
    renderSprite.setPosition(position);
}

Map::Map(sf::RenderTarget* user_target, string user_fileMap)
{
    Map();
    fileMap = user_fileMap;
    target = user_target;
    renderSprite.setPosition(position);
}
    
const sf::RenderTarget* Map::getTarget() const { return target;}
sf::Vector2f Map::getPosition() const { return position;}
sf::Sprite Map::getRenderSprite() const { return renderSprite;}

void Map::setTarget(sf::RenderTarget* user_target)
{
    target = user_target;
}
void Map::setPosition(sf::Vector2f user_position)
{
    position= user_position;
    renderSprite.setPosition(position);
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
        renderTexture.create(sizeMap.x*sizeSprite.x,sizeMap.y*sizeSprite.y);
        renderTexture.clear();
        for (int i=0; i<4; i++)
        {
            for (int j=0; j<nTextureInPrio[i]; j++)
            {
                for (int k=0; k<nSprite[i][j]; k++)
                {
                    sprite[i][j][k].setTexture(texture[iTexture[i][j]]);
                    renderTexture.draw(sprite[i][j][k]);
                }
            }
        }
        sf::View view(sf::FloatRect(0,0,sizeMap.x*sizeSprite.x,sizeMap.y*sizeSprite.y));
        renderTexture.setView(view);
        renderTexture.display();
        renderSprite.setTexture(renderTexture.getTexture());
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
void Map::draw(float elapsedTime)
{
    target->draw(renderSprite);
}


ostream& operator<<(ostream& f, const Map& user_object)
{
    PNG_Encoder foo(user_object);
    cout<<foo;
    return f;
}


