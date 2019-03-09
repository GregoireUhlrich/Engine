#include "wrapMenu.h" 
using namespace std;

WrapMenu::WrapMenu() 
{
    detached = false;
    detachedPosition = sf::Vector2f(0,0);
    properties.mode = 2;
    properties.fileTextureIcon = vector<string>(2);
    properties.fileTextureIcon[0] = "triangle-picker-grey.png";
    properties.fileTextureIcon[1] = "triangle-picker-blue.png";
    properties.fillColor = vector<sf::Color>(2);
    properties.fillColor[0] = sf::Color::White;
    properties.fillColor[1] = sf::Color(232,232,232);
    properties.outlineColor = vector<sf::Color>(2);
    properties.outlineColor[0] = sf::Color::Transparent;
    properties.outlineColor[1] = sf::Color(232,232,232);
    properties.textColor = vector<sf::Color>(2);
    properties.textColor[0] = sf::Color(84,106,139);
    properties.textColor[1] = sf::Color(84,106,139);
    properties.outlineThickness = vector<int>(2);
    properties.outlineThickness[0] = properties.outlineThickness[1] = 1;
    *this = WrapMenu(NULL,properties);
    chirality = 1;
    sizeScrollBar = sf::Vector2f(5,10);
    yMenu = lyMenu = 0;
    scrollBar.setPosition(position.x+size.x-sizeScrollBar.x-1,1);
    scrollBar.setSize(sizeScrollBar);
    scrollBar.setFillColor(sf::Color(146,161,176));
    this->updateButton();
}

WrapMenu::WrapMenu(sf::RenderTarget* user_target) 
{
    detached = false;
    detachedPosition = sf::Vector2f(0,0);
    properties.mode = 2;
    properties.fileTextureIcon = vector<string>(2);
    properties.fileTextureIcon[0] = "triangle-picker-grey.png";
    properties.fileTextureIcon[1] = "triangle-picker-blue.png";
    properties.fillColor = vector<sf::Color>(2);
    properties.fillColor[0] = sf::Color::White;
    properties.fillColor[1] = sf::Color(232,232,232);
    properties.outlineColor = vector<sf::Color>(2);
    properties.outlineColor[0] = sf::Color::Transparent;
    properties.outlineColor[1] = sf::Color(232,232,232);
    properties.textColor = vector<sf::Color>(2);
    properties.textColor[0] = sf::Color(84,106,139);
    properties.textColor[1] = sf::Color(84,106,139);
    properties.outlineThickness = vector<int>(2);
    properties.outlineThickness[0] = properties.outlineThickness[1] = 1;
    *this = WrapMenu(user_target, properties);
    chirality = 1;
    yMenu = lyMenu = 0;
    sizeScrollBar = sf::Vector2f(5,10);
    scrollBar.setPosition(position.x+size.x-sizeScrollBar.x-1,1);
    scrollBar.setSize(sizeScrollBar);
    scrollBar.setFillColor(sf::Color(146,161,176));
    this->updateButton();
}

WrapMenu::WrapMenu(sf::RenderTarget* user_target, PropButton user_properties): Button(user_target, user_properties, 1)
{
    detached = false;
    detachedPosition = sf::Vector2f(0,0);
    heightWidget = size.y;
    widthWidget = size.x;
    setScale(0.5*properties.scaleIcon);
    chirality = 1;
    choice = "";
    yMenu = lyMenu = 0;
    sizeScrollBar = sf::Vector2f(5,10);
    scrollBar.setPosition(position.x+size.x-sizeScrollBar.x-1,1);
    scrollBar.setSize(sizeScrollBar);
    scrollBar.setFillColor(sf::Color(146,161,176));
}   

WrapMenu::WrapMenu(const WrapMenu& user_object)
{
    *this = user_object;
}

string WrapMenu::getChoice() 
{
    if (choice != "")
    {
        string foo = choice;
        choice = "";
        return foo;
    }
    return "";
} 

string WrapMenu::getCurrentChoice() const
{
    string foo = "";
    for (int i=0; i<nAttachedWidget; i++)
    {
        if (attachedWidget[i]->getIsMouseHere())
        {
            foo = attachedWidget[i]->getCurrentChoice();
            return getText().getString().toAnsiString()+": "+foo;
        }
    }
    return "";
}

void WrapMenu::setDetached(bool user_detached)
{
    detached = user_detached;
}
void WrapMenu::setDetachedPosition(sf::Vector2f user_detachedPosition)
{
    detachedPosition = user_detachedPosition;
}
void WrapMenu::setDetachedTarget(sf::RenderTarget* user_target)
{
    detachedTarget = user_target;
}

void WrapMenu::testMouse(sf::Vector2i user_posMouse)
{
    posMouse = user_posMouse;
    bool fooBool = isMouseHere;
    bool isMouseOnWidget = 0;
    sf::Vector2i effPosMouse = posMouse;
    effPosMouse.x -= attachedSprite.getPosition().x;
    effPosMouse.y -= attachedSprite.getPosition().y-yMenu;
    if (detached)
    {
        effPosMouse.x += detachedPosition.x+position.x;
        effPosMouse.y += detachedPosition.y+position.y-size.y;
    }
    if (isMouseHere)
    {
        for (int i=0; i<nAttachedWidget; i++)
        {
            attachedWidget[i]->testMouse(effPosMouse);
            if (attachedWidget[i]->getIsMouseHere())
                isMouseOnWidget = 1;
        }
    }
    if (posMouse.x >= position.x and posMouse.x < position.x+size.x and posMouse.y >= position.y and posMouse.y < position.y+size.y)
        isMouseHere = 1;
    else if(!isMouseOnWidget)
        isMouseHere = 0;
    if (isMouseHere and properties.mode == 2)
    {
        iMode = 1;
    }
    else iMode = 0;
    if (!isMouseHere)
        isMousePressed = 0;
    if (!isMouseHere) yMenu = 0;
    if (isMouseHere != fooBool) this->updateButton();
}

void WrapMenu::mouseReleased()
{
    if (isMousePressed)
    {
        string foo = "";
        for (int i=0; i<nAttachedWidget; i++)
        {
            if (isMouseHere and attachedWidget[i]->getIsMouseHere())
            {
                attachedWidget[i]->mouseReleased();
                foo = attachedWidget[i]->getChoice();
                choice = getText().getString().toAnsiString()+": "+foo;
            }
            else attachedWidget[i]->mouseReleased();
        }
        isMouseHere = 0;
        isMousePressed = 0;
        iMode = 0;
        updateButton();
    }
}

void WrapMenu::addChoice(string newChoice, int type)
{
    Widget* fooButton;
    PropButton newProperties;
    bool inserted = false;
    switch(type)
    {
        case -1: // Add a simple Button, sort by name
        newProperties.mode = 2;
        newProperties.fillColor = vector<sf::Color>(2);
        newProperties.fillColor[0] = sf::Color::White;
        newProperties.fillColor[1] = sf::Color(232,232,232);
        newProperties.outlineColor = vector<sf::Color>(2);
        newProperties.outlineColor[0] = sf::Color::Transparent;
        newProperties.outlineColor[1] = sf::Color(232,232,232);
        newProperties.textColor = vector<sf::Color>(2);
        newProperties.textColor[0] = sf::Color(84,106,139);
        newProperties.textColor[1] = sf::Color(84,106,139);
        newProperties.outlineThickness = vector<int>(2);
        newProperties.outlineThickness[0] = newProperties.outlineThickness[1] = 0;
        fooButton = new Button(&attachedTexture, newProperties, 0);
        fooButton->setPosition(sf::Vector2f(0,nAttachedWidget*heightWidget));
        fooButton->setText(newChoice);
        for (int i=0; i<nAttachedWidget; i++)
        {
            if (fooButton->getText().getString() < attachedWidget[i]->getText().getString())
            {
                inserted = true;
                attachedWidget.insert(attachedWidget.begin()+i,fooButton);
                break;
            }
        }
        if (!inserted)
            attachedWidget.insert(attachedWidget.begin(), fooButton);
        //attachedWidget.push_back(fooButton);
        nAttachedWidget += 1;
        break;

        case 0: // Add a simple button
        newProperties.mode = 2;
        newProperties.fillColor = vector<sf::Color>(2);
        newProperties.fillColor[0] = sf::Color::White;
        newProperties.fillColor[1] = sf::Color(232,232,232);
        newProperties.outlineColor = vector<sf::Color>(2);
        newProperties.outlineColor[0] = sf::Color::Transparent;
        newProperties.outlineColor[1] = sf::Color(232,232,232);
        newProperties.textColor = vector<sf::Color>(2);
        newProperties.textColor[0] = sf::Color(84,106,139);
        newProperties.textColor[1] = sf::Color(84,106,139);
        newProperties.outlineThickness = vector<int>(2);
        newProperties.outlineThickness[0] = newProperties.outlineThickness[1] = 0;
        fooButton = new Button(&attachedTexture, newProperties, 0);
        fooButton->setPosition(sf::Vector2f(0,nAttachedWidget*heightWidget));
        fooButton->setText(newChoice);
        attachedWidget.push_back(fooButton);
        nAttachedWidget += 1;
        break;

        case 1: // Add a Side WrapMenu
        fooButton = new WrapMenuSide(&attachedTexture);
        fooButton->setText(newChoice);
        fooButton->setPosition(sf::Vector2f(0,nAttachedWidget*heightWidget));
        fooButton->addChoice("Choice 1");
        fooButton->addChoice("Choice 2");
        fooButton->addChoice("Choice 3");
        fooButton->addChoice("Choice 4");
        attachedWidget.push_back(fooButton);
        nAttachedWidget += 1;
        break;

        default:
        cout<<"Wrong type given to \"void WrapMenu::addChoice(string newChoice, int type)\": must be -1 (sorted buton) 0 (button) or 1 (sideMenu).\n";
        return;
    }
    sf::Vector2f fooSize = fooButton->getSize();
    if (fooSize.x > 3./4*widthWidget)
    {
        widthWidget = 4./3*fooSize.x;
    }
    for (int i=0; i<nAttachedWidget; i++)
    {
        attachedWidget[i]->setSize(sf::Vector2f(widthWidget, heightWidget));
    }
    lyMenu = min(nAttachedWidget*heightWidget,(int)(2*((int)target->getSize().y-position.y)/3.));
    lyMenu = floor(lyMenu*1./heightWidget)*heightWidget;

    sf::Vector2f fooPos = scrollBar.getPosition();
    fooPos.x = max((int)size.x, widthWidget+30)-sizeScrollBar.x;
    scrollBar.setPosition(fooPos);

    this->updateButton();
}

void WrapMenu::addChoice(Widget* newWidget)
{
    newWidget->setPosition(sf::Vector2f(0,nAttachedWidget*heightWidget));
    newWidget->setDetached(true);
    newWidget->setDetachedPosition(sf::Vector2f(position.x,position.y+size.y));
    attachedWidget.push_back(newWidget);
    newWidget->setTarget(&attachedTexture);
    newWidget->setDetachedTarget(target);
    nAttachedWidget += 1;
    sf::Vector2f fooSize = attachedWidget[nAttachedWidget-1]->getSize();
    if (fooSize.x > 3./4*widthWidget)
    {
        widthWidget = 4./3*fooSize.x;
    }
    for (int i=0; i<nAttachedWidget; i++)
    {
        attachedWidget[i]->setSize(sf::Vector2f(widthWidget, heightWidget));
    }
    lyMenu = min(nAttachedWidget*heightWidget,(int)(2*((int)target->getSize().y-position.y)/3.));
    lyMenu = floor(lyMenu*1./heightWidget)*heightWidget;

    sf::Vector2f fooPos = scrollBar.getPosition();
    fooPos.x = max((int)size.x, widthWidget+30)-sizeScrollBar.x;
    scrollBar.setPosition(fooPos);
    this->updateButton();
}

void WrapMenu::updateButton()
{
    Button::updateButton();
    if (nAttachedWidget > 0) attachedTexture.create(widthWidget,lyMenu);
    for (int i=0; i<nAttachedWidget; i++)
        attachedWidget[i]->setPosition(sf::Vector2f(0,i*heightWidget));
    if (nAttachedWidget > 0 and heightWidget*nAttachedWidget > lyMenu){
        if (deltaMouseWheel != 0)
        {
            deltaMouseWheel *= -1*heightWidget;
            if (yMenu + deltaMouseWheel < 0) deltaMouseWheel = -yMenu;
            if (yMenu + deltaMouseWheel > nAttachedWidget*heightWidget-lyMenu)
                deltaMouseWheel = nAttachedWidget*heightWidget-lyMenu-yMenu;
            yMenu += deltaMouseWheel;
            deltaMouseWheel = 0;
        }

        sizeScrollBar.y = lyMenu*lyMenu/(heightWidget*nAttachedWidget);
        scrollBar.setSize(sizeScrollBar);
        sf::Vector2f fooPos = scrollBar.getPosition();
        fooPos.y = position.y+lyMenu*yMenu/(nAttachedWidget*heightWidget);
        scrollBar.setPosition(fooPos);
    }
}

void WrapMenu::draw(float elapsedTime)
{
    if (target != 0)
    {
        target->draw(outline);
        target->draw(sprite);  
        if (isMouseHere and nAttachedWidget > 0)
        {
            attachedTexture.clear();
            attachedTexture.setView(sf::View(sf::FloatRect(0,yMenu,widthWidget,lyMenu)));
            for (int i=0; i<nAttachedWidget; i++)
                attachedWidget[i]->draw(elapsedTime);
            attachedTexture.display();
            attachedSprite = sf::Sprite();
            attachedSprite.setTexture(attachedTexture.getTexture());
            attachedSprite.setPosition(position.x,position.y+size.y);
            target->draw(attachedSprite);
            if (heightWidget*nAttachedWidget > lyMenu)
            {
                updateButton();
                target->draw(scrollBar);
            }   
        }
    }
    else
    {
        cout<<"Target = 0!\n";
    }
}

WrapMenu& WrapMenu::operator=(const WrapMenu& user_object)
{
    Button::operator=(user_object);
    heightWidget = user_object.heightWidget;
    widthWidget = user_object.widthWidget;
    choice = user_object.choice;

    return *this;
}

ostream& operator<<(ostream& f, const WrapMenu& user_object)
{

    return f;
}

WrapMenuSide::WrapMenuSide(): WrapMenu()
{
    properties.fileTextureIcon[0] = "triangle-picker-grey-rotated.png";
    properties.fileTextureIcon[1] = "triangle-picker-blue-rotated.png";
    for (int i=0; i<properties.mode; i++)
    {
        if (properties.textureIcon[i].loadFromFile(properties.fileTextureIcon[i]))
        {
            properties.spriteIcon[i].setTexture(properties.textureIcon[i]);
            properties.spriteIcon[i].setPosition(0,0);
            if (properties.outlineThickness[i] < 0 or properties.outlineThickness[i] > 20) properties.outlineThickness[i] = 0;
            sf::Vector2u foo = properties.textureIcon[iMode].getSize();
            if (properties.scaleIcon == 0)
            {
                float scale = size.y*1./foo.y;
                properties.scaleIcon = scale;
                properties.spriteIcon[i].setScale(scale,scale);
            }
        }
        else cout<<"Unable to open \""<<properties.fileTextureIcon[i]<<"\"\n";
    }
}
WrapMenuSide::WrapMenuSide(sf::RenderTarget* user_target): WrapMenu(user_target)
{
    properties.fileTextureIcon[0] = "triangle-picker-grey-rotated.png";
    properties.fileTextureIcon[1] = "triangle-picker-blue-rotated.png";
    for (int i=0; i<properties.mode; i++)
    {
        if (properties.textureIcon[i].loadFromFile(properties.fileTextureIcon[i]))
        {
            properties.spriteIcon[i].setTexture(properties.textureIcon[i]);
            properties.spriteIcon[i].setPosition(0,0);
            if (properties.outlineThickness[i] < 0 or properties.outlineThickness[i] > 20) properties.outlineThickness[i] = 0;
            sf::Vector2u foo = properties.textureIcon[iMode].getSize();
            if (properties.scaleIcon == 0)
            {
                float scale = size.y*1./foo.y;
                properties.scaleIcon = scale;
                properties.spriteIcon[i].setScale(scale,scale);
            }
        }
        else cout<<"Unable to open \""<<properties.fileTextureIcon[i]<<"\"\n";
    }
}
WrapMenuSide::WrapMenuSide(sf::RenderTarget* user_target, PropButton user_properties): WrapMenu(user_target, user_properties)
{
    properties.fileTextureIcon[0] = "triangle-picker-grey-rotated.png";
    properties.fileTextureIcon[1] = "triangle-picker-blue-rotated.png";
    for (int i=0; i<properties.mode; i++)
    {
        if (properties.textureIcon[i].loadFromFile(properties.fileTextureIcon[i]))
        {
            properties.spriteIcon[i].setTexture(properties.textureIcon[i]);
            properties.spriteIcon[i].setPosition(0,0);
            if (properties.outlineThickness[i] < 0 or properties.outlineThickness[i] > 20) properties.outlineThickness[i] = 0;
            sf::Vector2u foo = properties.textureIcon[iMode].getSize();
            if (properties.scaleIcon == 0)
            {
                float scale = size.y*1./foo.y;
                properties.scaleIcon = scale;
                properties.spriteIcon[i].setScale(scale,scale);
            }
        }
        else cout<<"Unable to open \""<<properties.fileTextureIcon[i]<<"\"\n";
    }
}
WrapMenuSide::WrapMenuSide(const WrapMenu& user_object): WrapMenu(user_object)
{
    properties.fileTextureIcon[0] = "triangle-picker-grey-rotated.png";
    properties.fileTextureIcon[1] = "triangle-picker-blue-rotated.png";
    for (int i=0; i<properties.mode; i++)
    {
        if (properties.textureIcon[i].loadFromFile(properties.fileTextureIcon[i]))
        {
            properties.spriteIcon[i].setTexture(properties.textureIcon[i]);
            properties.spriteIcon[i].setPosition(0,0);
            if (properties.outlineThickness[i] < 0 or properties.outlineThickness[i] > 20) properties.outlineThickness[i] = 0;
            sf::Vector2u foo = properties.textureIcon[iMode].getSize();
            if (properties.scaleIcon == 0)
            {
                float scale = size.y*1./foo.y;
                properties.scaleIcon = scale;
                properties.spriteIcon[i].setScale(scale,scale);
            }
        }
        else cout<<"Unable to open \""<<properties.fileTextureIcon[i]<<"\"\n";
    }
}

void WrapMenuSide::addChoice(string newChoice, int type)
{
    Widget* fooButton;
    PropButton newProperties;
    bool inserted = false;
    switch(type)
    {
        case -1: // Add a simple Button, sort by name
        newProperties.mode = 2;
        newProperties.fillColor = vector<sf::Color>(2);
        newProperties.fillColor[0] = sf::Color::White;
        newProperties.fillColor[1] = sf::Color(232,232,232);
        newProperties.outlineColor = vector<sf::Color>(2);
        newProperties.outlineColor[0] = sf::Color::Transparent;
        newProperties.outlineColor[1] = sf::Color(232,232,232);
        newProperties.textColor = vector<sf::Color>(2);
        newProperties.textColor[0] = sf::Color(84,106,139);
        newProperties.textColor[1] = sf::Color(84,106,139);
        newProperties.outlineThickness = vector<int>(2);
        newProperties.outlineThickness[0] = newProperties.outlineThickness[1] = 0;
        fooButton = new Button(&attachedTexture, newProperties, 0);
        fooButton->setPosition(sf::Vector2f(0,nAttachedWidget*heightWidget));
        fooButton->setText(newChoice);
        for (int i=0; i<nAttachedWidget; i++)
        {
            if (fooButton->getText().getString() < attachedWidget[i]->getText().getString())
            {
                inserted = true;
                attachedWidget.insert(attachedWidget.begin()+i,fooButton);
                break;
            }
        }
        if (!inserted)
            attachedWidget.insert(attachedWidget.end(), fooButton);
        //attachedWidget.push_back(fooButton);
        nAttachedWidget += 1;
        break;

        case 0: // Add a simple button
        newProperties.mode = 2;
        newProperties.fillColor = vector<sf::Color>(2);
        newProperties.fillColor[0] = sf::Color::White;
        newProperties.fillColor[1] = sf::Color(232,232,232);
        newProperties.outlineColor = vector<sf::Color>(2);
        newProperties.outlineColor[0] = sf::Color::Transparent;
        newProperties.outlineColor[1] = sf::Color(232,232,232);
        newProperties.textColor = vector<sf::Color>(2);
        newProperties.textColor[0] = sf::Color(84,106,139);
        newProperties.textColor[1] = sf::Color(84,106,139);
        newProperties.outlineThickness = vector<int>(2);
        newProperties.outlineThickness[0] = newProperties.outlineThickness[1] = 0;
        fooButton = new Button(&attachedTexture, newProperties, 0);
        fooButton->setPosition(sf::Vector2f(0,nAttachedWidget*heightWidget));
        fooButton->setText(newChoice);
        attachedWidget.push_back(fooButton);
        nAttachedWidget += 1;
        break;

        case 1: // Side WrapMenu
        fooButton = new WrapMenuSide(&attachedTexture);
        fooButton->setText(newChoice);
        fooButton->setPosition(sf::Vector2f(0,nAttachedWidget*heightWidget));
        fooButton->addChoice("Choice 1");
        fooButton->addChoice("Choice 2");
        fooButton->addChoice("Choice 3");
        fooButton->addChoice("Choice 4");
        attachedWidget.push_back(fooButton);
        nAttachedWidget += 1;
        break;

        default:
        cout<<"Wrong type given to \"void WrapMenu::addChoice(string newChoice, int type)\": must be 0 (button) or 1 (sideMenu).\n";
        return;
    }
    sf::Vector2f fooSize = fooButton->getSize();
    if (fooSize.x > 3./4*widthWidget)
    {
        widthWidget = 4./3*fooSize.x;
    }
    for (int i=0; i<nAttachedWidget; i++)
    {
        attachedWidget[i]->setSize(sf::Vector2f(widthWidget, heightWidget));
    }
    lyMenu = min(nAttachedWidget*heightWidget,(int)(2*((int)target->getSize().y-position.y)/3));
    lyMenu = floor(lyMenu*1./heightWidget)*heightWidget;
    sf::Vector2f fooPos = scrollBar.getPosition();
    fooPos.x = max((int)size.x, widthWidget+30)-sizeScrollBar.x;
    scrollBar.setPosition(fooPos);
    this->updateButton();
}

void WrapMenuSide::addChoice(Widget* newWidget)
{
    newWidget->setPosition(sf::Vector2f(0,nAttachedWidget*heightWidget));
    newWidget->setTarget(&attachedTexture);
    newWidget->setDetached(true);
    newWidget->setDetachedPosition(sf::Vector2f(position.x+size.x,position.y));
    newWidget->setDetachedTarget(target);
    attachedWidget.push_back(newWidget);
    nAttachedWidget += 1;
    sf::Vector2f fooSize = attachedWidget[nAttachedWidget-1]->getSize();
    if (fooSize.x > 3./4*widthWidget)
    {
        widthWidget = 4./3*fooSize.x;
    }
    for (int i=0; i<nAttachedWidget; i++)
    {
        attachedWidget[i]->setSize(sf::Vector2f(widthWidget, heightWidget));
    }
    lyMenu = min(nAttachedWidget*heightWidget,(int)(2*((int)target->getSize().y-position.y)/3));
    lyMenu = floor(lyMenu*1./heightWidget)*heightWidget;
    sf::Vector2f fooPos = scrollBar.getPosition();
    fooPos.x = max((int)size.x, widthWidget+30)-sizeScrollBar.x;
    scrollBar.setPosition(fooPos);
    this->updateButton();
}

void WrapMenuSide::updateButton()
{
    Button::updateButton();
    if (nAttachedWidget > 0) attachedTexture.create(widthWidget,lyMenu);
    for (int i=0; i<nAttachedWidget; i++)
        attachedWidget[i]->setPosition(sf::Vector2f(0,i*heightWidget));
    if (nAttachedWidget > 0 and heightWidget*nAttachedWidget > lyMenu){
        if (deltaMouseWheel != 0)
        {
            deltaMouseWheel *= -1*heightWidget;
            if (yMenu + deltaMouseWheel < 0) deltaMouseWheel = -yMenu;
            if (yMenu + deltaMouseWheel > nAttachedWidget*heightWidget-lyMenu)
                deltaMouseWheel = nAttachedWidget*heightWidget-lyMenu-yMenu;
            yMenu += deltaMouseWheel;
            deltaMouseWheel = 0;
        }
        sizeScrollBar.y = lyMenu*lyMenu/(heightWidget*nAttachedWidget);
        scrollBar.setSize(sizeScrollBar);
        sf::Vector2f fooPos = scrollBar.getPosition();
        fooPos.y = position.y+lyMenu*yMenu/(nAttachedWidget*heightWidget);
        fooPos.x = position.x+size.x+attachedWidget[0]->getSize().x-sizeScrollBar.x;
        scrollBar.setPosition(fooPos);
    }
}

void WrapMenuSide::draw(float elapsedTime)
{
    if (target != 0)
    {
        target->draw(outline);
        target->draw(sprite);  
        if (isMouseHere and nAttachedWidget > 0)
        {
            attachedTexture.clear();
            attachedTexture.setView(sf::View(sf::FloatRect(0,yMenu,widthWidget,lyMenu)));
            for (int i=0; i<nAttachedWidget; i++)
                attachedWidget[i]->draw(elapsedTime);
            attachedTexture.display();
            attachedSprite = sf::Sprite();
            attachedSprite.setTexture(attachedTexture.getTexture());
            if (detached) attachedSprite.setPosition(detachedPosition.x+position.x+size.x,detachedPosition.y+position.y);
            else attachedSprite.setPosition(position.x+size.x,position.y);
            if (detached and detachedTarget) detachedTarget->draw(attachedSprite);
            else target->draw(attachedSprite);
            if (heightWidget*nAttachedWidget > lyMenu)
            {
                updateButton();
                target->draw(scrollBar);
            }   
        }
    }
    else
    {
        cout<<"Target = 0!\n";
    }
}
