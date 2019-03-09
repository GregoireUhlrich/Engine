#include "PNG_Encoder.h"
using namespace std;


//////////////////////////////////////
/////////// PNG_ENCODER //////////////
//////////////////////////////////////

PNG_Encoder::PNG_Encoder()
{
    fileMap = "";
    nPrio = 4;
    nTextureInPrio = vector<int>(nPrio);
    nSprite = vector<vector<int> >(nPrio);
    sprite = vector<vector<vector<sf::Sprite> > >(nPrio);
    iTexture = vector<vector<int> >(nPrio);
    indexSprite = vector<vector<vector<vector<int> > > >(nPrio);
    for (int i=0; i<nPrio; i++)
    {
        nTextureInPrio[i] = 0;
        nSprite[i] = vector<int>(0);
        sprite[i] = vector<vector<sf::Sprite> >(0);
        iTexture[i] = vector<int>(0);
    }
    nTextureTot = 0;
    texture = vector<sf::Texture>(0);
    fileTexture = vector<string>(0);
    
    xPixel = yPixel = 0;
    sizeImg = 0;
    modePixel = 0;
    errorPeriodicity = 0;
    errorOverFlow = 0;
    
    sizeSprite = sf::Vector2i(32,32);
    sizeMap = sf::Vector2i(0,0);
    nEvents = 0;
    events = vector<string>(0);
}

PNG_Encoder::PNG_Encoder(string user_fileMap): PNG_Encoder()
{
    fileMap = user_fileMap;
}

PNG_Encoder::PNG_Encoder(const PNG_Encoder& user_object)
{
    *this = user_object;
}

string PNG_Encoder::getFileMap() const { return fileMap;}
vector<string> PNG_Encoder::getFileTextures() const { return fileTexture;}
sf::Vector2i PNG_Encoder::getSizeSprite() const { return sizeSprite;}
sf::Vector2i PNG_Encoder::getSizeMap() const { return sizeMap;}
Bouncer PNG_Encoder::getBouncer() const { return bouncer;}
void PNG_Encoder::setSizeSprite(sf::Vector2i user_sizeSprite)
{
    sizeSprite = user_sizeSprite;
}
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
    else
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
            yPixel += 1;
            if (yPixel == sizeImg) { yPixel = 0; errorPeriodicity = 1;}
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
    int fooInt = nameTexture.find("Tileset");
    if (fooInt != -1) nameTexture.erase(0,fooInt+8);
    
    fooInt = -1;
    for (int i=0; i<nTextureTot; i++)
        if (nameTexture == fileTexture[i] && fooInt == -1)
            fooInt = i;
            
    if (fooInt == -1)
    {
        texture.push_back(sf::Texture());
        texture[nTextureTot].loadFromFile(nameTexture);
        fileTexture.push_back(nameTexture);
        nTextureTot += 1;
        return (nTextureTot-1);
    }
    return fooInt;
}

int PNG_Encoder::load()
{
    int lenFileMap = fileMap.size()-1;
    if (fileMap[lenFileMap]=='t' and fileMap[lenFileMap-1]=='x' and fileMap[lenFileMap-2]=='t')
    {
        if (loadTxt(fileMap) != 0)
            return 1;
        fileMap[lenFileMap-2] = 'p';
        fileMap[lenFileMap-1] = 'n';
        fileMap[lenFileMap] = 'g';
        return 0;
    }

    xPixel = yPixel = modePixel = 0;
    if (!img.loadFromFile(fileMap))
    {
        cout<<"Unable to open file \""<<fileMap<<"\": loading empty map.\n";
        return 1;
    }
    sf::Vector2u fooSize = img.getSize();
    sizeImg = fooSize.x;
    sizeMap.x = readInt(); sizeMap.y = readInt();
    for (int i=0; i<nPrio; i++)
        indexSprite[i] = vector<vector<vector<int> > >(sizeMap.x, vector<vector<int> >(sizeMap.y,vector<int>(2,-1)));
    int fooX, fooY, fooXPNG, fooYPNG;
    for (int i=0; i<nPrio; i++)
    {
        nTextureInPrio[i] = readInt();
        nSprite[i] = vector<int>(nTextureInPrio[i]);
        sprite[i] = vector<vector<sf::Sprite> >(nTextureInPrio[i]);
        iTexture[i] = vector<int>(nTextureInPrio[i]);
        for (int j=0; j<nTextureInPrio[i]; j++)
        {
            iTexture[i][j] = addTexture(readString());
            nSprite[i][j] = readInt();
            sprite[i][j] = vector<sf::Sprite>(nSprite[i][j]);
            for (int k=0; k<nSprite[i][j]; k++)
            {
                fooX = readInt(); fooY = readInt(); fooXPNG = readInt(); fooYPNG = readInt();
                indexSprite[i][fooX/sizeSprite.x][fooY/sizeSprite.y][0] = j;
                indexSprite[i][fooX/sizeSprite.x][fooY/sizeSprite.y][1] = k;
                sprite[i][j][k].setPosition(fooX,fooY);
                sprite[i][j][k].setTexture(texture[iTexture[i][j]]);
                sprite[i][j][k].setTextureRect(sf::IntRect(fooXPNG,fooYPNG,sizeSprite.x,sizeSprite.y));
            }
        }
    }
    bouncer.setSizeMap(sizeMap);
    /*
    int nExceptions = readInt();
    vector<int> exceptions(nPrio*nExceptions);
    for (int i=0; i<nPrio*nExceptions; i++)
        exceptions[i] = readInt();*/
        
    int nExceptions = readInt();
    int fooDir;
    for (int i=0; i<nExceptions; i++)
    {
        fooX = readInt();
        fooY = readInt();
        fooDir = readInt();
        bouncer.addException(sf::Vector2i(fooX,fooY),fooDir);
    }
    
        
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
    xPixel = yPixel = modePixel = 0;
    int nTot = nPrio+2; // nPrio times a number of textures + the name of the encoder + lx+ly
    for (int i=0; i<nPrio; i++)
    {
        nTot += nTextureInPrio[i]; // number of sprites
        for (int j=0; j<nTextureInPrio[i]; j++)
        {
            nTot += fileTexture[iTexture[i][j]].length()+1; // Size of texture names
            for (int k=0; k<nSprite[i][j]; k++)
                nTot += nPrio; // for each sprite nPrio int.
        }
    }
    nTot += 1; // nExceptions
    nTot += 3*bouncer.getNExceptions(); // exceptions
    nTot += 1; //nEvents
    for (int i=0; i<nEvents; i++)
        nTot += events[i].length()+1; // Size of events strings
    int nPixel = ceil(nTot / 2.);
    sizeImg = ceil(sqrt(nPixel));
    img.create(sizeImg,sizeImg);
    writeInt(sizeMap.x); writeInt(sizeMap.y);
    sf::Vector2f fooPos;
    sf::IntRect fooRect;
    for (int i=0; i<nPrio; i++)
    {
        writeInt(nTextureInPrio[i]);
        for (int j=0; j<nTextureInPrio[i]; j++)
        {
            writeString(fileTexture[iTexture[i][j]]);
            writeInt(nSprite[i][j]);
            for (int k=0; k<nSprite[i][j]; k++)
            {   
                fooPos = sprite[i][j][k].getPosition();
                fooRect = sprite[i][j][k].getTextureRect();
                writeInt(fooPos.x);
                writeInt(fooPos.y);
                writeInt(fooRect.left);
                writeInt(fooRect.top);
            }
        }
    }
    writeInt(bouncer.getNExceptions());
    vector<vector<vector<bool> > > passOrNot = bouncer.getPassOrNot();
    for (int i=0; i<sizeMap.x; i++)
    {
        for (int j=0; j<sizeMap.y; j++)
        {
            for (int k=0; k<nPrio; k++)
            {
                if (!passOrNot[i][j][k])
                {
                    writeInt(i); writeInt(j); writeInt(k);
                }
            }
        }
    }
        
    writeInt(nEvents);
    for (int i=0; i<nEvents; i++)
    {
        writeString(events[i]);
    }
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
    fileMap = user_object.fileMap;
    nTextureInPrio = user_object.nTextureInPrio;
    nSprite = user_object.nSprite;
    sprite = user_object.sprite;
    indexSprite = user_object.indexSprite;
    iTexture = user_object.iTexture;
    texture = user_object.texture;
    fileTexture = user_object.fileTexture;
    img = user_object.img;
    xPixel = user_object.xPixel;
    yPixel = user_object.yPixel;
    modePixel = user_object.modePixel;
    sizeImg = user_object.sizeImg;
    errorPeriodicity = user_object.errorPeriodicity;
    errorOverFlow = user_object.errorOverFlow;
    sizeSprite = user_object.sizeSprite;
    sizeMap = user_object.sizeMap;
    bouncer = user_object.bouncer;
    nEvents = user_object.nEvents;
    events = user_object.events;
    
    return *this;
}

bool PNG_Encoder::operator==(const PNG_Encoder& user_object)
{
    for (int i=0; i<nPrio; i++)
    {
        if (nTextureInPrio[i] != user_object.nTextureInPrio[i]) return 0;
        for (int j=0; j<nTextureInPrio[i]; j++)
        {
            if (nSprite[i][j] != user_object.nSprite[i][j]) return 0;
            for (int k=0; k<nSprite[i][j]; k++)
            {
                if (sprite[i][j][k].getPosition() != user_object.sprite[i][j][k].getPosition())
                    return 0;
                if (sprite[i][j][k].getTextureRect() != user_object.sprite[i][j][k].getTextureRect())
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
    for (int i=0; i<user_object.nPrio; i++)
    {
        cout<<"nTextureInPrio["<<i<<"] = "<<user_object.nTextureInPrio[i]<<endl;
        for (int j=0; j<user_object.nTextureInPrio[i]; j++)
        {
            cout<<"nSprite["<<i<<"]["<<j<<"] = "<<user_object.nSprite[i][j]<<endl;
        }
    }
    cout<<endl;
    for (int i=0; i<user_object.nPrio; i++)
        for (int j=0; j<user_object.nTextureInPrio[i]; j++)
            cout<<"Prio "<<i<<" texture "<<j<<": "<<user_object.fileTexture[user_object.iTexture[i][j]]<<endl;
    cout<<endl;
    cout<<user_object.bouncer<<endl;
    cout<<user_object.nEvents<<" nEvents\n";
    for (int i=0; i<user_object.nEvents; i++)
        cout<<user_object.events[i]<<endl;
    return f;
}


int PNG_Encoder::loadTxt(string stringFile)
{
    ifstream file(stringFile.c_str(), ios::in);
    int fooX, fooY, fooXPNG, fooYPNG;
    if (file)
    {
        string foo;
        file>>sizeMap.x>>sizeMap.y>>foo;
        for (int i=0; i<nPrio; i++)
            indexSprite[i] = vector<vector<vector<int> > >(sizeMap.x, vector<vector<int> >(sizeMap.y,vector<int>(2,-1)));
        
        bouncer.setSizeMap(sizeMap);
        for (int i=0; i<nPrio; i++)
        {    
            file>>foo>>nTextureInPrio[i]>>foo;
            if (nTextureInPrio[i] > 0)
            {
                nSprite[i] = vector<int>(nTextureInPrio[i]);
                sprite[i] = vector<vector<sf::Sprite> >(nTextureInPrio[i]);
                iTexture[i] = vector<int>(nTextureInPrio[i]);
            
                for (int j=0; j<nTextureInPrio[i]; j++)
                {
                    file>>nSprite[i][j];
                    sprite[i][j] = vector<sf::Sprite>(nSprite[i][j]);
                }
                file>>foo;
            
                for (int j=0; j<nTextureInPrio[i]; j++)
                {
                    file>>foo;
                    iTexture[i][j] = addTexture(foo);
                    file>>foo;
                    for (int k=0; k<nSprite[i][j]; k++)
                    {
                        file>>fooX>>fooY>>fooXPNG>>fooYPNG>>foo;
                        indexSprite[i][fooX/sizeSprite.x][fooY/sizeSprite.y][0] = j;
                        indexSprite[i][fooX/sizeSprite.x][fooY/sizeSprite.y][1] = k;
                        sprite[i][j][k].setPosition(fooX,fooY);
                        sprite[i][j][k].setTexture(texture[iTexture[i][j]]);
                        sprite[i][j][k].setTextureRect(sf::IntRect(fooXPNG,fooYPNG,sizeSprite.x,sizeSprite.y));
                    }
                }
            }
        }
        int nExceptions;
        file>>foo>>nExceptions>>foo;
        vector<int> exceptions(nPrio*nExceptions);
        for (int i=0; i<nExceptions; i++)
        {
            file>>exceptions[nPrio*i]>>exceptions[nPrio*i+1]>>exceptions[nPrio*i+2]>>exceptions[nPrio*i+3];
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
    return 0;
}
