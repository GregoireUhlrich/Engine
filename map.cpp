#include "map.h" 
using namespace std;


//////////////////////////////////////
/////////// PNG_ENCODER //////////////
//////////////////////////////////////

PNG_Encoder::PNG_Encoder()
{
    fileMap = "";
    nTextureInPrio = vector<int>(4);
    nSprite = vector<vector<int> >(4);
    sprite = vector<vector<vector<Sprite> > >(4);
    texture = vector<vector<string> >(4);
    for (int i=0; i<4; i++)
    {
        nTextureInPrio[i] = 0;
        nSprite[i] = vector<int>(0);
        sprite[i] = vector<vector<Sprite> >(0);
        texture[i] = vector<string>(0);
    }
    xPixel = yPixel = 0;
    sizeImg = 0;
    modePixel = 0;
    errorPeriodicity = 0;
    errorOverFlow = 0;
    
    sizeMap = sf::Vector2u(0,0);
    nExceptions = nEvents = 0;
    exceptions = vector<int>(0);
    events = vector<string>(0);
}

PNG_Encoder::PNG_Encoder(string user_fileMap)
{
    PNG_Encoder();
    fileMap = user_fileMap;
}

PNG_Encoder::PNG_Encoder(vector<vector<vector<Sprite> > > user_sprite)
{
    PNG_Encoder();
    sprite = user_sprite;
    nTextureInPrio = vector<int>(4);
    nSprite = vector<vector<int> >(4);
    texture = vector<vector<string> >(4);
    for (int i=0; i<4; i++)
    {
        nTextureInPrio[i] = sprite[i].size();
        nSprite[i] = vector<int>(nTextureInPrio[i]);
        for (int j=0; j<nTextureInPrio[i]; j++)
            nSprite[i][j] = sprite[i][j].size();
        texture[i] = vector<string>(0);
    }
    
    int nTot = 4; // 4 times a number of textures
    for (int i=0; i<4; i++)
    {
        nTot += nTextureInPrio[i]; // number of sprites
        for (int j=0; j<nTextureInPrio[i]; j++)
            for (int k=0; k<nSprite[i][j]; k++)
                nTot += 4; // for each sprite 4 int.
    }
    int nPixel = nTot / 2;
    sizeImg = ceil(sqrt(nPixel));
    img.create(sizeImg,sizeImg);
}

PNG_Encoder::PNG_Encoder(const PNG_Encoder& user_object)
{
    fileMap = user_object.fileMap;
    nTextureInPrio = user_object.nTextureInPrio;
    nSprite = user_object.nSprite;
    sprite = user_object.sprite;
    img = user_object.img;
    xPixel = user_object.xPixel;
    yPixel = user_object.yPixel;
    modePixel = user_object.modePixel;
    sizeImg = user_object.sizeImg;
    errorPeriodicity = user_object.errorPeriodicity;
    errorOverFlow = user_object.errorOverFlow;
    sizeMap = user_object.sizeMap;
    nExceptions = user_object.nExceptions;
    nEvents = user_object.nEvents;
    exceptions = user_object.exceptions;
    events = user_object.events;
    texture = user_object.texture;
}

string PNG_Encoder::getFileMap() const { return fileMap;}
sf::Vector2u PNG_Encoder::getSizeMap() const { return sizeMap;}
void PNG_Encoder::setFileMap(string user_fileMap)
{
    fileMap = user_fileMap;
}

int PNG_Encoder::readInt()
{
    sf::Color c = img.getPixel(xPixel, yPixel);
    int a=0, b=0;
    if (modePixel == 0)
    {
        a = 255-c.r;
        b = 255-c.g;
        modePixel = 1;
    }
    else if (modePixel == 1)
    {
        a = 255-c.b;
        b = 255-c.a;
        modePixel = 0;
        xPixel += 1;
        if (xPixel == sizeImg)
        {
            yPixel = (yPixel+1)%sizeImg;
            if (yPixel == 0) errorPeriodicity = 1;
            xPixel = 0;
        }
    }
    return a + (int)pow(2,8)*b;
}

char PNG_Encoder::readChar()
{
    char c;
    int foo = readInt();
    c = (char)foo;

    return c;
}

string PNG_Encoder::readString()
{
    string s = "";
    char c = readChar();
    while(c != 59)
    {
        s.push_back(c);
        c = readChar();
    }
    return s;
}

void PNG_Encoder::writeInt(int value)
{
    if (value > (int)pow(2,16)-1) errorOverFlow = 1;
    int a=0, b=0;
    a = value%(int)pow(2,8);
    b = value/(int)pow(2,8);
    
    sf::Color c = img.getPixel(xPixel,yPixel);
    if (modePixel == 0)
    {
        c.r = 255-a;
        c.g = 255-b;
        modePixel = 1;
    }
    else if (modePixel == 1)
    {
        c.b = 255-a;
        c.a = 255-b;
        modePixel = 0;
    }
    img.setPixel(xPixel,yPixel,c);
    
    if (modePixel == 0)
    {
        xPixel += 1;
        if (xPixel == sizeImg)
        {
            yPixel = (yPixel+1)%sizeImg;
            if (yPixel == 0) errorPeriodicity = 1;
            xPixel = 0;
        }
    }
}

void PNG_Encoder::writeChar(char c)
{
    writeInt((int)c);
}

void PNG_Encoder::writeString(string s)
{
    for (int i=0; i<s.length(); i++)
        writeChar(s[i]);
    writeChar((char)59);
}

int PNG_Encoder::addTexture(string nameTexture)
{

}

int PNG_Encoder::load()
{
    if (!img.loadFromFile(fileMap))
    {
        cout<<"Unable to open file \""<<fileMap<<"\": loading empty map.\n";
        return 1;
    }
    sf::Vector2u fooSize = img.getSize();
    sizeImg = fooSize.x;
    sizeMap.x = readInt(); sizeMap.y = readInt();
    for (int i=0; i<4; i++)
    {
        nTextureInPrio[i] = readInt();
        nSprite[i] = vector<int>(nTextureInPrio[i]);
        sprite[i] = vector<vector<Sprite> >(nTextureInPrio[i]);
        texture[i] = vector<string>(nTextureInPrio[i]);
        for (int j=0; j<nTextureInPrio[i]; j++)
        {
            texture[i][j] = readString();
            nSprite[i][j] = readInt();
            sprite[i][j] = vector<Sprite>(nSprite[i][j]);
            for (int k=0; k<nSprite[i][j]; k++)
            {
                sprite[i][j][k].x = readInt();
                sprite[i][j][k].y = readInt();
                sprite[i][j][k].xPNG = readInt();
                sprite[i][j][k].yPNG = readInt();
            }
        }
    }
    nExceptions = readInt();
    exceptions = vector<int>(4*nExceptions);
    for (int i=0; i<4*nExceptions; i++)
        exceptions[i] = readInt();
    nEvents = readInt();
    events = vector<string>(nEvents);
    for (int i=0; i<nEvents; i++)
        events[i] = readString();
        
    if (errorPeriodicity)
    {
        errorPeriodicity = 0;
        cout<<"Error: end of PNG file has been reached. Periodicity has been applied.\n";
        return 2;
    }
    
    return 0;
}

int PNG_Encoder::save()
{
    int nTot = 1+4+2; // 4 times a number of textures + the name of the encoder + lx+ly
    for (int i=0; i<4; i++)
    {
        nTot += nTextureInPrio[i]; // number of sprites
        for (int j=0; j<nTextureInPrio[i]; j++)
        {
            nTot += texture[i][j].length(); // Size of texture names
            for (int k=0; k<nSprite[i][j]; k++)
                nTot += 4; // for each sprite 4 int.
        }
    }
    nTot += 1; // nExceptions
    nTot += 4*nExceptions; // exceptions
    nTot += 1; //nEvents
    for (int i=0; i<nEvents; i++)
        nTot += events[i].length(); // Size of events strings
    int nPixel = nTot / 2;
    sizeImg = ceil(sqrt(nPixel));
    sizeImg = sizeImg;
    img.create(sizeImg,sizeImg);
    writeInt(sizeMap.x); writeInt(sizeMap.y);
    for (int i=0; i<4; i++)
    {
        writeInt(nTextureInPrio[i]);
        for (int j=0; j<nTextureInPrio[i]; j++)
        {
            writeString(texture[i][j]);
            writeInt(nSprite[i][j]);
            for (int k=0; k<nSprite[i][j]; k++)
            {
                writeInt(sprite[i][j][k].x);
                writeInt(sprite[i][j][k].y);
                writeInt(sprite[i][j][k].xPNG);
                writeInt(sprite[i][j][k].yPNG);
            }
        }
    }
    writeInt(nExceptions);
    for (int i=0; i<4*nExceptions; i++)
        writeInt(exceptions[i]);
        
    writeInt(nEvents);
    for (int i=0; i<nEvents; i++)
        writeString(events[i]);
    
    if (!img.saveToFile(fileMap))
    {
        cout<<"Unable to save map, invalid file.\n";
        return 1;
    }
    if (errorPeriodicity)
    {
        errorPeriodicity = 0;
        cout<<"Error: end of PNG file has been reached. Periodicity has been applied.\n";
        return 3;
    }
    else if (errorOverFlow)
    {
        errorOverFlow = 0;
        cout<<"Error: Overflow in at least one value. Value(s) has(have) been truncated.\n";
        return 2;
    }
    
    return 0;
}

PNG_Encoder& PNG_Encoder::operator=(const PNG_Encoder& user_object)
{
    *this = PNG_Encoder(user_object);
    
    return *this;
}

bool PNG_Encoder::operator==(const PNG_Encoder& user_object)
{
    for (int i=0; i<4; i++)
    {
        if (nTextureInPrio[i] != user_object.nTextureInPrio[i]) return 0;
        for (int j=0; j<nTextureInPrio[i]; j++)
        {
            if (nSprite[i][j] != user_object.nSprite[i][j]) return 0;
            for (int k=0; k<nSprite[i][j]; k++)
            {
                if (sprite[i][j][k].x != user_object.sprite[i][j][k].x)
                    return 0;
                if (sprite[i][j][k].y != user_object.sprite[i][j][k].y)
                    return 0;
                if (sprite[i][j][k].xPNG != user_object.sprite[i][j][k].xPNG)
                    return 0;
                if (sprite[i][j][k].yPNG != user_object.sprite[i][j][k].yPNG)
                    return 0;
            }
        }
    }
    return 1;
}

bool PNG_Encoder::operator!=(const PNG_Encoder& user_object)
{
    return !(*this == user_object);
}

ostream& operator<<(std::ostream& f, const PNG_Encoder& user_object)
{
    cout<<"Name: "<<user_object.fileMap<<endl;
    cout<<"lx = "<<user_object.sizeMap.x<<"   ly = "<<user_object.sizeMap.y<<endl;
    for (int i=0; i<4; i++)
    {
        cout<<"nTextureInPrio["<<i<<"] = "<<user_object.nTextureInPrio[i]<<endl;
        for (int j=0; j<user_object.nTextureInPrio[i]; j++)
        {
            cout<<"nSprite["<<i<<"]["<<j<<"] = "<<user_object.nSprite[i][j]<<endl;
            for (int k=0; k<user_object.nSprite[i][j]; k++)
            {
                cout<<"sprite["<<i<<"]["<<j<<"]["<<k<<"] = "<<user_object.sprite[i][j][k].x<<" "<<user_object.sprite[i][j][k].y<<" "<<user_object.sprite[i][j][k].xPNG<<" "<<user_object.sprite[i][j][k].yPNG<<endl;
            }
        }
    }
    cout<<endl;
    for (int i=0; i<4; i++)
        for (int j=0; j<user_object.nTextureInPrio[i]; j++)
            cout<<"Prio "<<i<<" texture "<<j<<": "<<user_object.texture[i][j]<<endl;
    cout<<endl;
    cout<<user_object.nExceptions<<" nExceptions:\n";
    for (int i=0; i<user_object.nExceptions; i++)
        cout<<user_object.exceptions[4*i]<<" "<<user_object.exceptions[4*i+1]<<" "<<user_object.exceptions[4*i+2]<<" "<<user_object.exceptions[4*i+3]<<"  ";
    cout<<endl;
    cout<<user_object.nEvents<<" nEvents\n";
    for (int i=0; i<user_object.nEvents; i++)
        cout<<user_object.events[i]<<endl;
    return f;
}


int PNG_Encoder::loadTxt(string stringFile)
{
    ifstream file(stringFile.c_str(), ios::in);
    int nPrio = 4;
    if (file)
    {
        string foo;
        file>>sizeMap.x>>sizeMap.y>>foo;
        for (int i=0; i<nPrio; i++)
        {    
            file>>foo>>nTextureInPrio[i]>>foo;
            if (nTextureInPrio[i] > 0)
            {
                nSprite[i] = vector<int>(nTextureInPrio[i]);
                sprite[i] = vector<vector<Sprite> >(nTextureInPrio[i]);
            
                for (int j=0; j<nTextureInPrio[i]; j++)
                {
                    file>>nSprite[i][j];
                    sprite[i][j] = vector<Sprite>(nSprite[i][j]);
                }
                file>>foo;
            
                for (int j=0; j<nTextureInPrio[i]; j++)
                {
                    file>>foo;
                    texture[i].push_back(foo);
                    file>>foo;
                    Sprite s;
                    for (int k=0; k<nSprite[i][j]; k++)
                    {
                        file>>s.x>>s.y>>s.xPNG>>s.yPNG>>foo;
                        sprite[i][j][k] = s;
                    }
                }
            }
        }
        file>>foo>>nExceptions>>foo;
        exceptions = vector<int>(4*nExceptions);
        for (int i=0; i<nExceptions; i++)
        {
            file>>exceptions[4*i]>>exceptions[4*i+1]>>exceptions[4*i+2]>>exceptions[4*i+3];
        }
        file>>nEvents>>foo;
        file.ignore();
        file.ignore();
        events = vector<string>(nEvents);
        for (int i=0; i<nEvents; i++)
        {
            getline(file,events[i]);
        }
        return 0;
    }
    else
    {
        return 1;
    }
}


//////////////////////////////////////
/////////// MAP //////////////////////
//////////////////////////////////////

Map::Map(): PNG_Encoder()
{
    position = sf::Vector2f(0,0);
    size = sf::Vector2u(0,0);
}

Map::Map(sf::RenderTarget* user_target): PNG_Encoder()
{
    Map();
    target = user_target;
}

Map::Map(sf::RenderTarget* user_target, string user_fileMap): PNG_Encoder(user_fileMap)
{
    Map();
    target = user_target;
}
    
const sf::RenderTarget* Map::getTarget() const { return target;}
sf::Vector2f Map::getPosition() const { return position;}
sf::Vector2u Map::getSizeSprite() const { return sizeSprite;}
sf::Sprite Map::getSprite() const { return sprite;}

void Map::setTarget(sf::RenderTarget* user_target)
{
    target = user_target;
}
void Map::setPosition(sf::Vector2f user_position)
{
    position= user_position;
    sprite.setPosition(position);
}
void Map::setSizeSprite(sf::Vector2u user_sizeSprite)
{
    sizeSprite = user_sizeSprite;
    updateSprite();
}

void mapi::updateSprite()
{
    texture.create(sizeMap.x*sizeSprite.x,sizeMap.y*sizeSprite.y);
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<nTextureInPrio[i]; j++)
        {
            for (int k=0; k<nSprite[i][j]; k++)
            {
                texture.draw(sprite[i][j][k]);
            }
        }
    }
    sprite = texture.getTexture();
}

int Map::load()
{
    int foo = PNG_Encoder::load();
    updateSprite();
    return foo;
}
void Map::draw(float elapsedTime)
{
    target->draw(sprite);
}


ostream& operator<<(ostream& f, const Map& user_object)
{
    
    return f;
}


