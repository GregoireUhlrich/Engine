#include "wrapMenu.h" 
using namespace std;

WrapMenu::WrapMenu() 
{
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
    updateButton();
}

WrapMenu::WrapMenu(sf::RenderTarget* user_target) 
{
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
    updateButton();
}

WrapMenu::WrapMenu(sf::RenderTarget* user_target, PropButton user_properties): Button(user_target, user_properties, 1)
{
    heightWidget = size.y;
    widthWidget = size.x;
    choice = "";
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

void WrapMenu::testMouse(sf::Vector2i user_posMouse)
{
    posMouse = user_posMouse;
    bool isMouseOnWidget = 0;
    if (isMouseHere)
    {
        for (int i=0; i<nAttachedWidget; i++)
        {
            attachedWidget[i]->testMouse(posMouse);
            if (attachedWidget[i]->getIsMouseHere())
                isMouseOnWidget = 1;
        }
    }
    if (posMouse.x > position.x and posMouse.x < position.x+size.x and posMouse.y > position.y and posMouse.y < position.y+size.y)
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
    updateButton();
}

void WrapMenu::mouseReleased()
{
    for (int i=0; i<nAttachedWidget; i++)
    {
        if (attachedWidget[i]->getIsMouseHere())
        {
            choice = attachedWidget[i]->getChoice();
            break;
        }
    }
    isMouseHere = 0;
    isMousePressed = 0;
}

void WrapMenu::addChoice(string newChoice, int type)
{
    Widget* fooButton;
    PropButton newProperties;
    switch(type)
    {
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
        fooButton = new Button(target, newProperties, 0);
        fooButton->setPosition(sf::Vector2f(position.x,position.y-1+size.y+nAttachedWidget*heightWidget));
        fooButton->setText(newChoice);
        attachedWidget.push_back(fooButton);
        nAttachedWidget += 1;
        break;

        case 1: // Side WrapMenu
        //attachedWidget.push_back();
        //nAttachedWidget + 1;
        break;

        default:
        cout<<"Wrong type given to \"void WrapMenu::addChoice(string newChoice, int type)\": must be 0 (button) or 1 (sideMenu).\n";
        return;
    }
    sf::Vector2f fooSize = attachedWidget[nAttachedWidget-1]->getSize();
    if (fooSize.x > 3./4*widthWidget)
    {
        widthWidget = 4./3*fooSize.x;
    }
    for (int i=0; i<nAttachedWidget; i++)
    {
        attachedWidget[i]->setSize(sf::Vector2f(widthWidget, heightWidget+1));
    }
    updateButton();
}

void WrapMenu::draw(float elapsedTime)
{
    if (target != 0)
    {
        target->draw(outline);
        target->draw(sprite);  
        if (isMouseHere)
            for (int i=0; i<nAttachedWidget; i++)
                attachedWidget[i]->draw(elapsedTime);
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

