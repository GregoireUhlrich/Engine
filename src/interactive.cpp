#include "interactive.h"

using namespace std;


Interactive::Interactive(): Widget()
{
    nDrawable = 0;
    choice = "";
    drawable = vector<sf::Drawable*>(0);
}

Interactive::Interactive(sf::RenderTarget* w, IMap* user_map): Interactive()
{
    target = w;
    map = user_map;
    state = map->getState();
    size = sf::Vector2f(w->getSize().x, 110);
    texture.create((int)size.x,(int)size.y);
    texture.clear(sf::Color::White);
    texture.display();
    sf::Sprite sprite2;
    sprite2.setTexture(texture.getTexture());
    sprite = sprite2;
    wrapHasFocus = false;

    indices.file = 0;
    indices.edit = 1;
    indices.mode = 2;
    indices.prio = 3;
    indices.layer = 4;
    indices.grid = 5;
    indices.pass = 6;
    indices.game = 7;
    indices.tileset = 8;

    attachedWidget.push_back(new WrapMenu(target));
    int x0 = 10;
    int y0 = 10;
    sf::Vector2f posWidget(x0,y0);
    attachedWidget[0]->setPosition(posWidget);
    attachedWidget[0]->setText("File");
    attachedWidget[0]->addChoice("New Map");
    Widget* loadMenu = new WrapMenuSide(target);
    loadMenu->setText("Load Map");
    DIR * rep = opendir(mapDirectory.c_str()); 
    if (rep != NULL) 
    { 
        struct dirent * ent; 
  
        while ((ent = readdir(rep)) != NULL) 
        { 
            if (string(ent->d_name) != "." && string(ent->d_name) != "..") loadMenu->addChoice(string(ent->d_name)); // Type = -1 sorting file by name
        } 
        closedir(rep); 
    }
    attachedWidget[0]->addChoice(loadMenu);
    attachedWidget[0]->addChoice("Save Map");
    attachedWidget[0]->addChoice("Quit");
    nAttachedWidget++;

    int deltaXMenu = 10 + attachedWidget[0]->getSize().x;
    posWidget.x += deltaXMenu;

    attachedWidget.push_back(new WrapMenu(target));
    attachedWidget[1]->setPosition(posWidget);
    attachedWidget[1]->setText("Edit");
    attachedWidget[1]->addChoice("Change Size Map");
    attachedWidget[1]->addChoice("New Event");
    Widget* wrapEvent = new WrapMenuSide(target);
    wrapEvent->setText("Edit Events");
    Widget* wrapScenario = new WrapMenuSide(target);
    wrapScenario->setText("Edit Scenario");
    attachedWidget[1]->addChoice(wrapEvent);
    attachedWidget[1]->addChoice(wrapScenario);
    nAttachedWidget++;

    int posX = 100;
    int posY = 60;
    PropButton properties;
    properties.mode = 2;
    properties.fileTextureIcon = vector<string>(2);
    properties.outlineColor = vector<sf::Color>(2);
    properties.fillColor = vector<sf::Color>(2);
    properties.outlineThickness = vector<int>(2);
    properties.fileTextureIcon[0] = iconDirectory+"add-grey.png";
    properties.fileTextureIcon[1] = iconDirectory+"add-green.png";
    properties.outlineThickness[0] = 0;
    properties.outlineThickness[1] = 1;
    properties.outlineColor[0] = sf::Color(110,110,110);
    properties.outlineColor[1] = sf::Color(84,106,139);
    properties.fillColor[0] = sf::Color::White;
    properties.fillColor[1] = sf::Color::White;
    
    nAttachedWidget++;
    Widget* foo;
    Widget* set = new Set_PushButton;
    foo = new PressButton(target, properties);
    foo->setPosition(sf::Vector2f(posX,posY));
    set->addChoice(foo);
    properties.fileTextureIcon[0] = iconDirectory+"move-grey.png";
    properties.fileTextureIcon[1] = iconDirectory+"move-blue.png";
    foo = new PressButton(target, properties);
    foo->setPosition(sf::Vector2f(posX+50,posY));
    set->addChoice(foo);
    properties.fileTextureIcon[0] = iconDirectory+"select-grey.png";
    properties.fileTextureIcon[1] = iconDirectory+"select-yellow.png";
    foo = new PressButton(target, properties);
    foo->setPosition(sf::Vector2f(posX+100,posY));
    set->addChoice(foo);
    properties.fileTextureIcon[0] = iconDirectory+"erase-grey.png";
    properties.fileTextureIcon[1] = iconDirectory+"erase-red.png";
    foo = new PressButton(target, properties);
    foo ->setPosition(sf::Vector2f(posX+150,posY));
    set->addChoice(foo);
    set->setChoiceSet(1); // SET MOVE MODE 
    attachedWidget.push_back(set);

    posX = 665;
    nAttachedWidget++;
    set = new Set_PushButton;
    foo = new PressButton(target, properties);
    foo->setPosition(sf::Vector2f(posX+50,posY));
    foo->setText("0");
    foo->setIcon(false);
    foo->setSize(sf::Vector2f(40,40));
    foo->setTextCentered(true);
    set->addChoice(foo);
    foo = new PressButton(target, properties);
    foo->setPosition(sf::Vector2f(posX+100,posY));
    foo->setText("1");
    foo->setIcon(false);
    foo->setSize(sf::Vector2f(40,40));
    foo->setTextCentered(true);
    set->addChoice(foo);
    foo = new PressButton(target, properties);
    foo->setPosition(sf::Vector2f(posX+150,posY));
    foo->setText("2");
    foo->setIcon(false);
    foo->setSize(sf::Vector2f(40,40));
    foo->setTextCentered(true);
    set->addChoice(foo);
    foo = new PressButton(target, properties);
    foo->setPosition(sf::Vector2f(posX+200,posY));
    foo->setText("3");
    foo->setIcon(false);
    foo->setSize(sf::Vector2f(40,40));
    foo->setTextCentered(true);
    set->addChoice(foo);
    attachedWidget.push_back(set);

    properties.fileTextureIcon[0] = iconDirectory+"layers-grey.png";
    properties.fileTextureIcon[1] = iconDirectory+"layers-blue.png";
    nAttachedWidget++;
    attachedWidget.push_back(new PressButton(target, properties));
    attachedWidget[4]->setPosition(sf::Vector2f(posX,posY));
    
    posX = 445;
    nAttachedWidget++;
    properties.fileTextureIcon[0] = iconDirectory+"grid-grey.png";
    properties.fileTextureIcon[1] = iconDirectory+"grid-blue.png";
    attachedWidget.push_back(new PressButton(target, properties));
    attachedWidget[5]->setPosition(sf::Vector2f(posX,posY));
    nAttachedWidget++;
    properties.fileTextureIcon[0] = iconDirectory+"forbidden-grey.png";
    properties.fileTextureIcon[1] = iconDirectory+"forbidden-red.png";
    attachedWidget.push_back(new PressButton(target, properties));
    attachedWidget[6]->setPosition(sf::Vector2f(posX+50,posY));
    posX = 960;
    nAttachedWidget++;
    properties.fileTextureIcon[0] = iconDirectory+"run-green.png";
    properties.fileTextureIcon[1] = iconDirectory+"run-green.png";
    properties.fillColor[0] = sf::Color::White;
    properties.fillColor[1] = sf::Color(226,242,229);
    properties.textColor = vector<sf::Color>(2);
    properties.textColor[0] = sf::Color(97,184,114);
    properties.textColor[1] = sf::Color(97,184,114);
    properties.outlineColor[1] = sf::Color::Transparent;
    attachedWidget.push_back(new PushButton(target,properties));
    attachedWidget[7]->setPosition(sf::Vector2f(posX,posY));
    attachedWidget[7]->setText("Run game");
    attachedWidget[7]->setRatioPressed(0.95);

    sf::Vector2i fooSizeIm(320,3200);
    int fooThickness = 5;
    //nAttachedWidget++;
    //drawable.push_back(new wrapMenuTextureUX(window,map,"Texture",0,ly+1,fooSizeIm.x+2*fooThickness,40,0,0));
    //drawable.push_back(new wrapMenuTextureUX(window,map,"Texture",0,ly+40+1,fooSizeIm.x+2*fooThickness,40,0,1));
    
    //map->setState(moving);
    attachedWidget[6]->setPressed(1);
    
    int nStaticText = 3;
    int characterSize = 20;
    posX = 30;
    posY = 68;
    vector<Text*> textDrawable(0);
    for (int i=0; i<nStaticText; i++)
    {
        textDrawable.push_back(new Text());
        nDrawable++;
    }
    textDrawable[0]->setFont(ubuntuFont);
    textDrawable[0]->setString("Action");
    textDrawable[0]->setCharacterSize(characterSize);
    textDrawable[0]->setPosition(posX,posY+40/2-characterSize);
    textDrawable[0]->setFillColor(sf::Color(84,106,139));
    textDrawable[1]->setFont(ubuntuFont);
    textDrawable[1]->setString("Display");
    textDrawable[1]->setCharacterSize(characterSize);
    textDrawable[1]->setPosition(posX+320,posY+40/2-characterSize);
    textDrawable[1]->setFillColor(sf::Color(84,106,139));
    textDrawable[2]->setFont(ubuntuFont);
    textDrawable[2]->setString("Layers");
    textDrawable[2]->setCharacterSize(characterSize);
    textDrawable[2]->setPosition(posX+565,posY+40/2-characterSize);
    textDrawable[2]->setFillColor(sf::Color(84,106,139));
    for (int i=0; i<nStaticText; i++)
        drawable.push_back(textDrawable[i]);
    
    int nStaticShape = 4;
    int l = 35;
    posX = 320;
    posY = 80;
    vector<sf::RectangleShape*> shapeDrawable(0);
    for (int i=0; i<nStaticShape; i++)
    {
        shapeDrawable.push_back(new sf::RectangleShape());
        nDrawable++;
    }
    shapeDrawable[0]->setSize(sf::Vector2f(2, l));
    shapeDrawable[0]->setPosition(posX,posY-l/2);
    shapeDrawable[0]->setFillColor(sf::Color(232,232,232));
    shapeDrawable[1]->setSize(sf::Vector2f(2, l));
    shapeDrawable[1]->setPosition(posX+245,posY-l/2);
    shapeDrawable[1]->setFillColor(sf::Color(232,232,232));
    shapeDrawable[2]->setSize(sf::Vector2f(2,l));
    shapeDrawable[2]->setPosition(posX+610,posY-l/2);
    shapeDrawable[2]->setFillColor(sf::Color(232,232,232));
    shapeDrawable[3]->setPosition(0, size.y);
    int sizeLimit = 1;
    shapeDrawable[3]->setSize(sf::Vector2f(size.x,sizeLimit));
    shapeDrawable[3]->setFillColor(sf::Color(217,217,217));

    for (int i=0; i<nStaticShape; i++)
        drawable.push_back(shapeDrawable[i]);

    attachedWidget.push_back(new TilesetDisplayer(target, target->getSize().y-size.y-sizeLimit));
    vector<string> fooTexture = map->getFileTextures();
    for (int i=0; i<fooTexture.size(); i++)
        attachedWidget[nAttachedWidget]->addTextureFromMap(fooTexture[i]);
    nAttachedWidget++;
    
    posY = 28;
    posX = 350;
    Text* textMap = new Text();
    textMap->setFont(ubuntuFont);
    textMap->setString("Map: "+map->getFileMap());
    textMap->setCharacterSize(characterSize);
    textMap->setStyle(sf::Text::Bold);
    textMap->setPosition(round(target->getSize().x-posX),round(posY+40/2-characterSize));
    textMap->setFillColor(sf::Color(84,106,139));
    drawable.push_back(textMap);
    nDrawable++;
    
    string fooString = attachedWidget[nAttachedWidget-1]->getFileLeft();
    int fooInt = fooString.find("/");
    fooString.erase(0,fooInt+1);
    fooInt = fooString.find("/");
    fooString.erase(0,fooInt+1);

    Text* textTexture = new Text();
    textTexture->setFont(ubuntuFont);
    textTexture->setString("Texture: "+fooString);
    textTexture->setCharacterSize(characterSize);
    textTexture->setStyle(sf::Text::Bold);
    textTexture->setPosition(round(target->getSize().x-posX),round(posY+40+40/2-characterSize));
    textTexture->setFillColor(sf::Color(84,106,139));
    drawable.push_back(textTexture);
    nDrawable++;
    
    sf::RectangleShape* posShape = new sf::RectangleShape();
    posShape->setSize(sf::Vector2f(2,size.y));
    posShape->setPosition(target->getSize().x-posX-15,0);
    posShape->setFillColor(sf::Color(232,232,232));
    drawable.push_back(posShape);
    nDrawable++;
}

void Interactive::testMouse(sf::Vector2i user_posMouse)
{
    posMouse = user_posMouse;
    if (posMouse.x >= position.x and posMouse.x < position.x+size.x and posMouse.y >= position.y and posMouse.y < position.y+size.y)
        isMouseHere = 1;
    else
        isMouseHere = 0;

    bool wrapDeployed = false;
    for (int i=0; i<2; i++) // 2 wrap Menus
    {
        attachedWidget[i]->testMouse(posMouse);
        if (attachedWidget[i]->getIsMouseHere())
        {
            wrapDeployed = true;
            break;
        }
    }
    if (!wrapDeployed)
    {
        for (int i=2; i<nAttachedWidget; i++)
        {
            attachedWidget[i]->testMouse(posMouse);
            if (attachedWidget[i]->getIsMouseHere())
            {
                break;
            }
        }
    }
}

void Interactive::testEvent(sf::Event event)
{
    for (int i=0; i<nAttachedWidget; i++)
        attachedWidget[i]->testEvent(event);
    if (attachedWidget[indices.mode]->getChoiceSet() != state)
    {
        state = (StateMap)attachedWidget[indices.mode]->getChoiceSet();
        map->setState(state);
    }
    map->setAllPrio(1-attachedWidget[indices.layer]->getSignal());
    map->setPrio(attachedWidget[indices.prio]->getChoiceSet());
    map->setGrid(attachedWidget[indices.grid]->getSignal());
    map->setPass(attachedWidget[indices.pass]->getSignal());
    string foo = attachedWidget[indices.file]->getChoice();
    if (foo != "")
    {
        if (foo == "File: Quit") choice = "Quit";
        else if (foo[6]=='L' and foo[7]=='o')
        {
            foo.erase(foo.begin(), foo.begin()+16);
            cout<<"Loading "<<foo<<endl;
            map->setFileMap(mapDirectory+foo);
            map->load();
        }
        else if (foo == "File: Save Map")
        {
            cout<<"Saving map.\n";
            map->save();
            sf::Vector2f posWidget = attachedWidget[0]->getPosition();
            delete attachedWidget[0];
            attachedWidget[0] = new WrapMenu(target);
            attachedWidget[0]->setPosition(posWidget);
            attachedWidget[0]->setText("File");
            attachedWidget[0]->addChoice("New Map");
            Widget* loadMenu = new WrapMenuSide(target);
            loadMenu->setText("Load Map");
            DIR * rep = opendir(mapDirectory.c_str()); 
            if (rep != NULL) 
            { 
                struct dirent * ent; 
          
                while ((ent = readdir(rep)) != NULL) 
                { 
                    if (string(ent->d_name) != "." && string(ent->d_name) != "..") loadMenu->addChoice(string(ent->d_name)); // Type = -1 sorting file by name
                } 
                closedir(rep); 
            }
            attachedWidget[0]->addChoice(loadMenu);
            attachedWidget[0]->addChoice("Save Map");
            attachedWidget[0]->addChoice("Quit");
        }
    }

    foo = attachedWidget[indices.tileset]->getChoice();
    if (foo != "")
    {
        foo.erase(foo.begin(), foo.begin()+23);
        if (foo[0] == ' ')
            foo.erase(foo.begin());
        attachedWidget[indices.tileset]->setFileLeft(tilesetDirectory+foo);
    }
}

void Interactive::windowResized()
{
    size = sf::Vector2f(target->getSize().x, size.y);
    texture.create((int)size.x,(int)size.y);
    texture.clear(sf::Color::White);
    texture.display();
    sf::Sprite sprite2;
    sprite2.setTexture(texture.getTexture());
    sprite = sprite2;
}

string Interactive::getChoice()
{
    if (choice == "Quit")
    {
        choice = "";
        return "Quit";
    }
    else return "";
}

void Interactive::draw(double elapsedTime)
{
    sf::Vector2i nSpriteToPull = attachedWidget[indices.tileset]->getNSpriteToPull();
    if (nSpriteToPull.x > 0 and nSpriteToPull.y > 0)
    {
        sf::Vector2i posSpriteToPull = attachedWidget[indices.tileset]->getPosSpriteToPull();
        map->setGhost(tilesetDirectory+attachedWidget[indices.tileset]->getFileLeft(), nSpriteToPull, posSpriteToPull);
        attachedWidget[indices.tileset]->resetSelection();
        attachedWidget[indices.mode]->setChoiceSet(0);
        map->setState(ADD);
    }
    if (enabled)
    {
        if (size.x != target->getSize().x);
        {
            windowResized();
        }

        target->draw(sprite);
        for (int i=0; i<nDrawable; i++)
            target->draw(*drawable[i]);
        for (int i=0; i<nAttachedWidget; i++)
            attachedWidget[nAttachedWidget-1-i]->draw(elapsedTime);
    }
}

void Set_PushButton::testEvent(sf::Event event)
{
    Widget::testEvent(event);
    for (int i=0; i<nAttachedWidget; i++)
    {
        attachedWidget[i]->testEvent(event);
        if (attachedWidget[i]->getSignal() and i != activatedButton)
        {
            if (activatedButton >= 0 and activatedButton < nAttachedWidget)
            {
                attachedWidget[activatedButton]->setPressed(false);
            }
            activatedButton = i;
        }
    }
}

void Set_PushButton::addChoice(Widget* newWidget)
{
    attachedWidget.push_back(newWidget);
    nAttachedWidget++;
    if (nAttachedWidget == 1)
        attachedWidget[0]->setPressed(1);
}

void Set_PushButton::setChoiceSet(int choice)
{
    for (int i=0; i<nAttachedWidget; i++)
    {
        if (i == choice)
            attachedWidget[i]->setPressed(true);
        else 
            attachedWidget[i]->setPressed(false);
    }
}

TilesetDisplayer::TilesetDisplayer(sf::RenderTarget* w, int user_height): Widget(w)
{
    sizeTarget = target->getSize();
    select = select2 = 0;
    selectRect.setFillColor(sf::Color::Transparent);
    selectRect.setOutlineColor(sf::Color(84,106,139));
    selectRect.setOutlineThickness(5.);

    nSpriteToPull = sf::Vector2i(0,0);
    posSpriteToPull = sf::Vector2i(0,0);
    choice = "";
    string default_tileset = "base.png";
    isImLeft = false;
    isImRight = false;
    offset = 5;
    heightChoice = 40;
    height = user_height;
    yImage = 0;
    sizeSprite = sf::Vector2i(32,32);
    yScroll = 32;
    fileLeft = "";
    fileRight = "";
    position = sf::Vector2f(0,target->getSize().y-height);
    outlineLeft.setFillColor(sf::Color::Transparent);
    outlineLeft.setOutlineColor(sf::Color(84,106,139));
    outlineLeft.setPosition(position.x+offset, position.y+offset+2*heightChoice);
    outlineLeft.setOutlineThickness(offset);
    outlineRight.setFillColor(sf::Color::Transparent);
    outlineRight.setOutlineColor(sf::Color(84,106,139));
    outlineRight.setOutlineThickness(offset);
    size.x = 0;
    size.y = height;
    if (imLeft.loadFromFile(default_tileset))
    {
        isImLeft = true;
        fileLeft = default_tileset;
        sf::Vector2u foo = imLeft.getSize();
        size.x = 2*offset+foo.x;
        textureLeft.create(foo.x, height-2*offset-2*heightChoice);
        outlineLeft.setSize(sf::Vector2f(foo.x,height-2*offset-2*heightChoice));
        sf::Sprite sprite;
        sprite.setTexture(imLeft);
        textureLeft.clear(sf::Color::White);
        textureLeft.draw(sprite);
        textureLeft.display();
        spriteLeft.setTexture(textureLeft.getTexture());
        spriteLeft.setPosition(position.x+offset, position.y+offset+2*heightChoice);
    }
    if (size.x == 0) size.x = 250;
    attachedWidget.push_back(new WrapMenuSide(target));
    attachedWidget[0]->setSize(sf::Vector2f(size.x, heightChoice));
    attachedWidget[0]->setText("Load tileset from file");
    attachedWidget[0]->setPosition(sf::Vector2f(0,position.y));
    attachedWidget[0]->setTextCentered(true);
    DIR * rep = opendir(tilesetDirectory.c_str()); 
  
    if (rep != NULL) 
    { 
        struct dirent * ent; 
  
        while ((ent = readdir(rep)) != NULL) 
        { 
            if (string(ent->d_name) != "." && string(ent->d_name) != "..") attachedWidget[0]->addChoice(string(ent->d_name), -1); // Type = -1 sorting file by name
        } 
        closedir(rep); 
    }
    attachedWidget.push_back(new WrapMenuSide(target));
    attachedWidget[1]->setSize(sf::Vector2f(size.x, heightChoice));
    attachedWidget[1]->setText("Load tileset from map");
    attachedWidget[1]->setPosition(sf::Vector2f(0,position.y+heightChoice));
    attachedWidget[1]->setTextCentered(true);
    nAttachedWidget = 2;
}

sf::Vector2i TilesetDisplayer::convertPosMouse(sf::Vector2i p)
{
    sf::Vector2i posTopLeft;
    sf::View view = textureLeft.getView();
    sf::Vector2f center = view.getCenter();
    sf::Vector2f fooSize = view.getSize();
    posTopLeft.x = center.x-fooSize.x/2;
    posTopLeft.y = center.y-fooSize.y/2;
    
    p.x += posTopLeft.x - offset;
    p.y += posTopLeft.y - (position.y+2*heightChoice+offset);
    
    return p;
}

sf::Vector2i TilesetDisplayer::invConvertPosMouse(sf::Vector2i p)
{
    sf::Vector2i posTopLeft;
    sf::View view = textureLeft.getView();
    sf::Vector2f center = view.getCenter();
    sf::Vector2f fooSize = view.getSize();
    posTopLeft.x = center.x-fooSize.x/2;
    posTopLeft.y = center.y-fooSize.y/2;
    
    p.x -= posTopLeft.x - offset;
    p.y -= posTopLeft.y - (position.y+2*heightChoice+offset);
    
    return p;
}

string TilesetDisplayer::getChoice()
{
    if (choice == "") return "";
    string foo = choice;
    choice = "";
    return foo;
}

void TilesetDisplayer::mouseReleased()
{
    attachedWidget[0]->mouseReleased();
    attachedWidget[1]->mouseReleased();
    string foo = attachedWidget[0]->getChoice();
    if (foo != "")
        choice = foo;
    else
    {
        foo = attachedWidget[1]->getChoice();
        if (foo != "")
            choice = foo;
    }
    isMousePressed = 0;
    isMouseHere = 0;
}

string TilesetDisplayer::getFileLeft() const
{
    return fileLeft;
}

sf::Vector2i TilesetDisplayer::getSizeSprite() const { return sizeSprite;}

sf::Vector2i TilesetDisplayer::getNSpriteToPull() const { return nSpriteToPull;}

sf::Vector2i TilesetDisplayer::getPosSpriteToPull() const { return posSpriteToPull;}

void TilesetDisplayer::resetSelection()
{
    nSpriteToPull = sf::Vector2i(0,0);
    select = select2 = 0;
    isMousePressed = 0;
}

void TilesetDisplayer::setFileLeft(string file)
{
    if (imLeft.loadFromFile(file))
    {
        isImLeft = true;
        fileLeft = file;
        sf::Vector2u foo = imLeft.getSize();
        size.x = 2*offset+foo.x;
        textureLeft.create(foo.x, height-2*offset-2*heightChoice);
        outlineLeft.setSize(sf::Vector2f(foo.x,height-2*offset-2*heightChoice));
        sf::Sprite sprite;
        sprite.setTexture(imLeft);
        textureLeft.clear(sf::Color::White);
        textureLeft.draw(sprite);
        textureLeft.display();
        spriteLeft.setTexture(textureLeft.getTexture());
        spriteLeft.setPosition(position.x+offset, position.y+offset+2*heightChoice);
        attachedWidget[0]->setSize(sf::Vector2f(size.x, heightChoice));
        attachedWidget[1]->setSize(sf::Vector2f(size.x, heightChoice));
        updateButton();
    }
    else cout<<"Unable to load image \""<<file<<"\".\n";
}

void TilesetDisplayer::setSizeSprite(sf::Vector2i user_sizeSprite) { sizeSprite = user_sizeSprite;}
void TilesetDisplayer::setYScroll(int user_yScroll) { yScroll = user_yScroll;}

void TilesetDisplayer::addTextureFromMap(string file)
{
    attachedWidget[1]->addChoice(file);
}

void TilesetDisplayer::windowResized()
{
    sf::Vector2u foo = imLeft.getSize();
    height += target->getSize().y - sizeTarget.y;
    sizeTarget = target->getSize();
    textureLeft.create(foo.x, height-2*offset-2*heightChoice);
    outlineLeft.setSize(sf::Vector2f(foo.x,height-2*offset-2*heightChoice));
    sf::Sprite sprite;
    sprite.setTexture(imLeft);
    textureLeft.clear(sf::Color::White);
    textureLeft.draw(sprite);
    textureLeft.display();
    spriteLeft = sf::Sprite();
    spriteLeft.setTexture(textureLeft.getTexture());
    spriteLeft.setPosition(position.x+offset, position.y+offset+2*heightChoice);
}

void TilesetDisplayer::updateButton()
{
    if (!isImLeft) return;
    if (sizeTarget.y != target->getSize().y and isImLeft)
    {
        windowResized();
    }
    if (attachedWidget[0]->getIsMouseHere() or attachedWidget[1]->getIsMouseHere())
        deltaMouseWheel = 0;
    if (deltaMouseWheel != 0)
    {
        deltaMouseWheel = deltaMouseWheel*yScroll;
        float lyView = height-2*heightChoice;
        float ly = imLeft.getSize().y;
        if (yImage-deltaMouseWheel < 0) deltaMouseWheel = yImage;
        else if (yImage-deltaMouseWheel+lyView-2*offset > ly) deltaMouseWheel = yImage+lyView-ly-2*offset;
        yImage -= deltaMouseWheel;
        posClick.y += deltaMouseWheel;
        sf::View view = textureLeft.getView();
        view.move(sf::Vector2f(0,-deltaMouseWheel));
        textureLeft.setView(view);
    }
    if (isMouseHere or select)
    {
        if (isMousePressed)
        {
            sf::Vector2i effPosClick = convertPosMouse(posClick);
            sf::Vector2i effectivePosMouse = convertPosMouse(posMouse);
            sf::Vector2u fooSize = imLeft.getSize();
            effPosClick.x = floor((effPosClick.x*1.)/sizeSprite.x)*sizeSprite.x;
            effPosClick.y = floor((effPosClick.y*1.)/sizeSprite.y)*sizeSprite.y;
            if (!select || (select && select2))
            {
                select2 = 0;
                select = 1;
                nSpriteToPull = sf::Vector2i(0,0);
            }
            else if (select)
            {
                int fooInt[2];
                fooInt[0] = ceil((effectivePosMouse.x*1.)/sizeSprite.x)*sizeSprite.x;
                fooInt[1] = ceil((effectivePosMouse.y*1.)/sizeSprite.y)*sizeSprite.y;
        
                int xmin = min(effPosClick.x, fooInt[0]);
                int xmax = max(effPosClick.x, fooInt[0]);
                int ymin = min(effPosClick.y, fooInt[1]);
                int ymax = max(effPosClick.y, fooInt[1]);
                xmin = max(xmin, 0);
                xmax = min(xmax, (int)fooSize.x);
                ymin = max(ymin, 0);
                ymax = min(ymax, (int)fooSize.y);

                selectRect.setPosition(xmin, ymin);
                selectRect.setSize(sf::Vector2f(xmax-xmin, ymax-ymin));
            }
            posClick = invConvertPosMouse(effPosClick);
        }
        else if (select)
        {
            sf::Vector2i effPosClick = convertPosMouse(posClick);
            sf::Vector2i effectivePosMouse = convertPosMouse(posMouse);
            sf::Vector2u fooSize = imLeft.getSize();
            select2 = 1;
            sf::Vector2f foo;
            sf::Vector2f foo2;
            foo = selectRect.getPosition();
            foo2 = selectRect.getSize();
            float xmin, ymin, xmax, ymax;
            xmin = min(floor((float)(foo.x)/sizeSprite.x)*sizeSprite.x,(float)fooSize.x-sizeSprite.x);
            xmax = max(xmin+sizeSprite.x,(float)round((foo.x+foo2.x)/sizeSprite.x)*sizeSprite.x);
            xmax = min(xmax,(float)fooSize.x);
            ymin = min(floor(((float)foo.y)/sizeSprite.y)*sizeSprite.y,(float)fooSize.y-sizeSprite.y);
            ymax = max(ymin+sizeSprite.y,(float)round((foo.y+foo2.y)/sizeSprite.y)*sizeSprite.y);
            ymax = min(ymax,(float)fooSize.y);
            selectRect.setPosition(xmin,ymin);
            selectRect.setSize(sf::Vector2f(xmax-xmin, ymax-ymin));

            nSpriteToPull.x = round((xmax-xmin)/sizeSprite.x);
            nSpriteToPull.y = round((ymax-ymin)/sizeSprite.y);
            posSpriteToPull.x = xmin;
            posSpriteToPull.y = ymin;
        }
    }
    sf::Sprite sprite;
    sprite.setTexture(imLeft);
    textureLeft.clear(sf::Color::White);
    textureLeft.draw(sprite);
    if (select or isMousePressed) textureLeft.draw(selectRect);
    textureLeft.display();
    spriteLeft = sf::Sprite();
    spriteLeft.setPosition(position.x+offset, position.y+offset+2*heightChoice);
    spriteLeft.setTexture(textureLeft.getTexture());
    deltaMouseWheel = 0;  
}

void TilesetDisplayer::draw(float elapsedTime)
{
    updateButton();
    string foo = attachedWidget[0]->getCurrentChoice();
    if (foo == "")
        foo = attachedWidget[1]->getCurrentChoice();
    if (foo != "")
    {
        foo.erase(foo.begin(), foo.begin()+23);
        if (foo[0] == ' ')
            foo.erase(foo.begin());
        foo = tilesetDirectory+foo;

        if (foo != fileRight and imRight.loadFromFile(foo))
        {
            isImRight = true;
            fileRight = foo;
            sf::Vector2u fooVec = imRight.getSize();
            size.x = 2*offset+fooVec.x;
            textureRight.create(fooVec.x, height-2*offset);
            outlineRight.setSize(sf::Vector2f(fooVec.x,height-2*offset));
            outlineRight.setPosition(sizeTarget.x-fooVec.x-offset,position.y+offset);
            sf::Sprite sprite;
            sprite.setTexture(imRight);
            textureRight.clear(sf::Color::White);
            textureRight.draw(sprite);
            textureRight.display();
            spriteRight = sf::Sprite();
            spriteRight.setTexture(textureRight.getTexture());
            spriteRight.setPosition(sizeTarget.x-fooVec.x-offset, position.y+offset);
        }
        else if (foo != fileRight) cout<<"Unable to open image \""<<foo<<"\".\n";
        else if (foo == fileRight and not isImRight) isImRight = true;
    }
    else isImRight = false;
    if (isImLeft)
    {
        target->draw(spriteLeft);
        target->draw(outlineLeft);
    }
    if (isImRight)
    {
        target->draw(spriteRight);
        target->draw(outlineRight);
    }
    for (int i=0; i<nAttachedWidget; i++)
        attachedWidget[i]->draw(elapsedTime);
}

