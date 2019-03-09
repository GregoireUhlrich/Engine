#include "bouncer.h" 
using namespace std;

Bouncer::Bouncer()
{
    nExceptions = 0;
    passOrNot = vector<vector<vector<bool> > >(0);
    sizeMap = sf::Vector2i(0,0);
}

Bouncer::Bouncer(const Bouncer& user_object)
{
    *this = user_object;
}
    
std::vector<std::vector<std::vector<bool> > > Bouncer::getPassOrNot() const { return passOrNot;}
int Bouncer::getNExceptions() const { return nExceptions;}
sf::Vector2i Bouncer::getSizeMap() const { return sizeMap;}

void Bouncer::setPassOrNot(std::vector<std::vector<std::vector<bool> > > user_passOrNot)
{
    sf::Vector2i newSizeMap(0,0);
    if (passOrNot.size() != user_passOrNot.size())
    {
        newSizeMap.x = user_passOrNot.size();
    }
    for (int i=0; i<passOrNot.size(); i++)
    {
        if (passOrNot[i].size() != user_passOrNot[i].size())
        {
            newSizeMap.y = user_passOrNot[i].size();
        }
        for (int j=0; j<passOrNot[i].size(); j++)
        {
            if (user_passOrNot[i][j].size() != 4)
            {
                cout<<"Denied \"Bouncer::setPassOrNot\": needs 4 bool per box.\n";
                return;
            }
        }
    }
    if (newSizeMap != sf::Vector2i(0,0))
    {
        if (newSizeMap.x == 0) newSizeMap.x = sizeMap.x;
        if (newSizeMap.y == 0) newSizeMap.y = sizeMap.y;
        setSizeMap(newSizeMap);
    }
    passOrNot = user_passOrNot;
    countExceptions();
}
void Bouncer::setSizeMap(sf::Vector2i user_sizeMap)
{
    if (user_sizeMap.x > 0 and user_sizeMap.y > 0)
    {
        if (user_sizeMap.y > sizeMap.y)
        {
            for (int i=0; i<sizeMap.x; i++)
            {
                for (int j=sizeMap.y; j<user_sizeMap.y; j++)
                {
                    passOrNot[i].push_back(vector<bool>(4));
                    for (int k=0; k<4; k++)
                        passOrNot[i][j][k] = 1;
                }
            }
        }
        else
        {
            for (int i=0; i<sizeMap.x; i++)
                passOrNot[i].erase(passOrNot[i].begin()+user_sizeMap.y, passOrNot[i].begin()+sizeMap.y);
        }
        if (user_sizeMap.x > sizeMap.x)
        {
            for (int i=sizeMap.x; i<user_sizeMap.x; i++)
            {
                passOrNot.push_back(vector<vector<bool> >(user_sizeMap.y));
                for (int j=0; j<user_sizeMap.y; j++)
                {
                    passOrNot[i][j] = vector<bool>(4);
                    for (int k=0; k<4; k++)
                        passOrNot[i][j][k] = 1;
                }
            }
        }
        else
        {
            passOrNot.erase(passOrNot.begin()+user_sizeMap.x, passOrNot.begin()+sizeMap.x);
        }
        sizeMap = user_sizeMap;
        countExceptions();
    }
    else cout<<"Denied change size map in \"Bouncer::setSizeMap\": sizeMap is 0\n";
}

void Bouncer::countExceptions()
{
    nExceptions = 0;
    for (int i=0; i<passOrNot.size(); i++)
        for (int j=0; j<passOrNot[i].size(); j++)
            for (int k=0; k<4; k++)
                nExceptions += !(passOrNot[i][j][k]);
}

void Bouncer::addException(sf::Vector2i position, int direction)
{
    if (passOrNot[position.x][position.y][direction])
    {
        nExceptions += 1;
        passOrNot[position.x][position.y][direction] = 0;
    }
}

bool Bouncer::ask(sf::Vector2f position, sf::Vector2i sizeSprite, int direction) const
{
    sf::Vector2f relativePosition = position;
    relativePosition.x /= sizeSprite.x;
    relativePosition.y /= sizeSprite.y;
    int antiDirection = (direction + 2)%4;
    int ix = round(relativePosition.x);
    int iy = round(relativePosition.y);
    if (ix < 0 or ix >= sizeMap.x or iy < 0 or iy >= sizeMap.y) return 0;
    switch(direction)
    {
        case 0:
        ix = ceil(relativePosition.x);
        if (ix >= sizeMap.x) return 0;
        else if(!passOrNot[ix][iy][antiDirection]) return 0;
        break;
        
        case 1:
        iy = floor(relativePosition.y);
        if (iy < 0) return 0;
        else if(!passOrNot[ix][iy][antiDirection]) return 0;
        break;
        
        case 2:
        ix = floor(relativePosition.x);
        if (ix < 0) return 0;
        else if(!passOrNot[ix][iy][antiDirection]) return 0;
        break;
        
        case 3:
        iy = ceil(relativePosition.y);
        if (iy >= sizeMap.y) return 0;
        else if(!passOrNot[ix][iy][antiDirection]) return 0;
        break;
    }
    return 1;
}

bool Bouncer::ask(sf::Vector2f position, sf::Vector2i sizeSprite) const
{
    sf::Vector2f relativePos;
    relativePos.x = position.x/sizeSprite.x;
    relativePos.y = position.y/sizeSprite.y;
    if (relativePos.x >= sizeMap.x or relativePos.y >= sizeMap.y) return false;
    if (relativePos.x < 0 or relativePos.y < 0) return false;
    return true;
}

bool Bouncer::ask(sf::Vector2i position) const
{
    if (position.x >= sizeMap.x or position.y >= sizeMap.y) return false;
    if (position.x < 0 or position.y < 0) return false;
    for (int i=0; i<4; i++)
        if (!passOrNot[position.x][position.y][i])
            return false;
    return true;
}

Bouncer& Bouncer::operator=(const Bouncer& user_object)
{
    passOrNot = user_object.passOrNot;
    nExceptions = user_object.nExceptions;
    sizeMap = user_object.sizeMap;
    
    return *this;
}

ostream& operator<<(ostream& f, const Bouncer& user_object)
{
    cout<<user_object.nExceptions<<" exceptions.\n";
    cout<<"Size Map: "<<user_object.sizeMap.x<<" "<<user_object.sizeMap.y<<endl;
    for (int i=0; i<user_object.sizeMap.x; i++)
        for (int j=0; j<user_object.sizeMap.y; j++)
            for (int k=0; k<4; k++)
                if (!user_object.passOrNot[i][j][k])
                    cout<<i<<"  "<<j<<"  "<<k<<endl;
    return f;
}

