#include "widget.h" 
using namespace std;

Widget::Widget()
{
    target = 0;
    enabled = 1;
    position = sf::Vector2f(0,0);
    sprite.setPosition(position);
    size = sf::Vector2f(40,40);
    texture.create((int)size.x,(int)size.y);
    sprite.setTexture(texture.getTexture());
    posMouse = sf::Vector2i(0,0);
    posClick = sf::Vector2i(0,0);
    nKey = 0;
    key = vector<sf::Keyboard::Key >(0);
    isMouseHere = isMousePressed = 0;
    nAttachedWidget = 0;
    attachedWidget = vector<Widget* >(0);
    deltaMouseWheel = 0;
}

Widget::Widget(sf::RenderTarget* user_target): Widget()
{
    target = user_target;
}

Widget::Widget(const Widget& user_object)
{
    *this = user_object;
}

const sf::RenderTarget* Widget::getTarget() const { return target;}
sf::Vector2f Widget::getPosition() const { return position;}
sf::Vector2f Widget::getSize() const { return size;}
sf::Vector2i Widget::getPosMouse() const { return posMouse;}
sf::Vector2i Widget::getPosClick() const { return posClick;}
bool Widget::getIsMouseHere() const { return isMouseHere;}
bool Widget::getIsMousePressed() const { return isMousePressed;}
bool Widget::getEnabled() const { return enabled;}
int Widget::getDeltaMouseWheel() const { return deltaMouseWheel;}

void Widget::setTarget(sf::RenderTarget* user_target)
{
    target = user_target;
}
void Widget::setPosition(sf::Vector2f user_position)
{
    position = user_position;
}
void Widget::setSize(sf::Vector2f user_size)
{
    if (user_size.x > 0 and user_size.y > 0)
    {
        size = user_size;
    }
    else cout<<"Invalid size in \"Widget::setSize\": must be greater than 0.\n";
}
void Widget::setEnabled(bool user_enabled)
{
    enabled = user_enabled;
}

void Widget::testMouse(sf::Vector2i user_posMouse)
{
    posMouse = user_posMouse;
    if (posMouse.x > position.x and posMouse.x < position.x+size.x and posMouse.y > position.y and posMouse.y < position.y+size.y)
        isMouseHere = 1;
    else
        isMouseHere = 0;
}

void Widget::testEvent(sf::Event event)
{
    if (enabled)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            keyPressed(event.key.code);
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            keyReleased(event.key.code);
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            mousePressed();
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            mouseReleased();
        }
    }
}

void Widget::mousePressed()
{
    if (isMouseHere)
    {
        isMousePressed = 1;
    }
}

void Widget::mouseReleased()
{
    isMousePressed = 0;
}

void Widget::mouseWheel(int user_deltaMouseWheel)
{
    if (isMouseHere) deltaMouseWheel += user_deltaMouseWheel;
}

void Widget::keyPressed(sf::Keyboard::Key user_key)
{
    bool keyAlreadyPressed = 0;
    for (int i=0; i<nKey; i++)
    {
        if (key[i] == user_key)
        {
            keyAlreadyPressed = 1;
            break;
        }
    }
    if (!keyAlreadyPressed)
    {
        nKey ++;
        key.push_back(user_key);
    }
}

void Widget::keyReleased(sf::Keyboard::Key user_key)
{
    for (int i=0; i<nAttachedWidget; i++)
        attachedWidget[i]->keyReleased(user_key);
    for (int i=0; i<nKey; i++)
    {
        if (key[i] == user_key)
        {
            key.erase(key.begin()+i);
            nKey --;
            break;
        }
    }
}

void Widget::draw(float elapsedTime)
{
    if (enabled)
    {
        if (target != 0) target->draw(sprite);
        for (int i=0; i<nAttachedWidget; i++)
            attachedWidget[i]->draw(elapsedTime);
    }
}

Widget& Widget::operator=(const Widget& user_object)
{
    target = user_object.target;
    enabled = user_object.enabled;
    position = user_object.position;
    size = user_object.size;
    posMouse = user_object.posMouse;
    posClick = user_object.posClick;
    nKey = user_object.nKey;
    key = user_object.key;
    isMouseHere = user_object.isMouseHere;
    isMousePressed = user_object.isMousePressed;
    nAttachedWidget = user_object.nAttachedWidget;
    attachedWidget = user_object.attachedWidget;
    deltaMouseWheel = user_object.deltaMouseWheel;
    sprite = user_object.sprite;
    
    return *this;
}

Text::Text(): sf::Text()
{
    font.loadFromFile(ubuntuFont);
}

Text::Text(string user_string): sf::Text()
{
    setCharacterSize(20);
    font.loadFromFile(ubuntuFont);
    sf::Text::setFont(font);
    setFillColor(sf::Color::Black);
    setString(sf::String(user_string));
}

Text::Text(const sf::String& str, const sf::Font &user_font, unsigned int characterSize): sf::Text(str,font,characterSize)
{
    font = user_font;
}

void Text::setFont(string fontFile)
{
    font.loadFromFile(fontFile);
    sf::Text::setFont(font);
}
