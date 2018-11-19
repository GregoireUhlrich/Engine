#include "button.h" 
using namespace std;

//////////////////////////////////////
///////// BUTTON /////////////////////
//////////////////////////////////////
Button::Button(): Widget()
{
    target = 0;
    offsetText = 5;
    ratioPressed = 0.95;
    signal = icon = 0;
    text.setString("");
    
    properties.mode = 1;
    iMode = 0;
    properties.fileTextureIcon = vector<string>(1);
    properties.fileTextureIcon[0] = "";
    properties.textureIcon = vector<sf::Texture>(1);
    properties.spriteIcon = vector<sf::Sprite>(1);
    properties.spriteIcon[0].setPosition(position);
    properties.fillColor = vector<sf::Color>(1);
    properties.fillColor[0] = sf::Color::White; 
    properties.textColor = vector<sf::Color>(1);
    properties.textColor[0] = sf::Color::Black;
    properties.outlineColor = vector<sf::Color>(1);
    properties.outlineColor[0] = sf::Color(84,106,139);
    properties.outlineThickness = vector<int>(1);
    properties.outlineThickness[0] = 2;
    outline.setPosition(position);
    outline.setSize(size);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineColor(properties.outlineColor[0]);
    outline.setOutlineThickness(properties.outlineThickness[0]);
    updateButton();
}

Button::Button(sf::RenderTarget* user_target): Button()
{
    target = user_target;
}

Button::Button(sf::RenderTarget* user_target, PropButton user_properties): Widget(user_target)
{
    offsetText = 5;
    ratioPressed = 0.95;
    signal = 0;
    icon = 1;
    iMode = 0;
    text.setString("");
    
    if (user_properties.mode != 1 and user_properties.mode != 2)
    {
        cout<<"Bad initialization of pushButton properties: properties.mode must be 1 or 2.\n";
        *this = Button();
    }
    else
    {
        properties.mode = user_properties.mode;
        properties.fileTextureIcon = user_properties.fileTextureIcon;
        properties.textureIcon = user_properties.textureIcon;
        properties.spriteIcon = user_properties.spriteIcon;
        properties.fillColor = user_properties.fillColor;
        properties.outlineColor = user_properties.outlineColor;
        properties.textColor = user_properties.textColor;
        properties.outlineThickness = user_properties.outlineThickness;
        
        if (properties.fileTextureIcon.size() != properties.mode)
        {
            cout<<"Bad initialization of pushButton: needs 2 files for textures.\n";
            *this = Button();
        }
        else
        {
            if (properties.textureIcon.size() != properties.mode) properties.textureIcon = vector<sf::Texture> (properties.mode);
            if (properties.spriteIcon.size() != properties.mode) properties.spriteIcon = vector<sf::Sprite> (properties.mode);
            if (properties.fillColor.size() != properties.mode) properties.fillColor = vector<sf::Color> (properties.mode);
            if (properties.outlineColor.size() != properties.mode) properties.outlineColor = vector<sf::Color> (properties.mode);
            if (properties.textColor.size() != properties.mode) properties.textColor = vector<sf::Color> (properties.mode);
            if (properties.outlineThickness.size() != properties.mode) properties.outlineThickness = vector<int> (properties.mode);
            for (int i=0; i<properties.mode; i++)
            {
                if (properties.textureIcon[i].loadFromFile(properties.fileTextureIcon[i]))
                {
                    properties.spriteIcon[i].setTexture(properties.textureIcon[i]);
                    properties.spriteIcon[i].setPosition(0,0);
                    if (properties.outlineThickness[i] < 0 or properties.outlineThickness[i] > 20) properties.outlineThickness[i] = 0;
                    sf::Vector2u foo = properties.textureIcon[iMode].getSize();
                    float scale = size.y*1./foo.y;
                    properties.spriteIcon[i].setScale(scale,scale);
                }
                else cout<<"Unable to open \""<<properties.fileTextureIcon[i]<<"\"\n";
            }
            outline.setPosition(position);
            outline.setSize(size);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineColor(properties.outlineColor[0]);
            outline.setOutlineThickness(properties.outlineThickness[0]);
            text.setFillColor(properties.textColor[0]);
            updateButton();
        }
    }
}

Button::Button(sf::RenderTarget* user_target, PropButton user_properties, bool user_icon): Widget(user_target)
{
    offsetText = 5;
    ratioPressed = 0.95;
    signal = 0;
    icon = user_icon;
    iMode = 0;
    text.setString("");
    
    if (user_properties.mode != 1 and user_properties.mode != 2)
    {
        cout<<"Bad initialization of pushButton properties: properties.mode must be 1 or 2.\n";
        *this = Button();
    }
    else
    {
        properties.mode = user_properties.mode;
        properties.fileTextureIcon = user_properties.fileTextureIcon;
        properties.textureIcon = user_properties.textureIcon;
        properties.spriteIcon = user_properties.spriteIcon;
        properties.fillColor = user_properties.fillColor;
        properties.outlineColor = user_properties.outlineColor;
        properties.textColor = user_properties.textColor;
        properties.outlineThickness = user_properties.outlineThickness;
        
        if (icon and properties.fileTextureIcon.size() != properties.mode)
        {
            cout<<"Bad initialization of pushButton: needs 2 files for textures.\n";
            *this = Button();
        }
        else
        {
            if (properties.textureIcon.size() != properties.mode) properties.textureIcon = vector<sf::Texture> (properties.mode);
            if (properties.spriteIcon.size() != properties.mode) properties.spriteIcon = vector<sf::Sprite> (properties.mode);
            if (properties.fillColor.size() != properties.mode) properties.fillColor = vector<sf::Color> (properties.mode);
            if (properties.outlineColor.size() != properties.mode) properties.outlineColor = vector<sf::Color> (properties.mode);
            if (properties.textColor.size() != properties.mode) properties.textColor = vector<sf::Color> (properties.mode);
            if (properties.outlineThickness.size() != properties.mode) properties.outlineThickness = vector<int> (properties.mode);
            if (icon)
            {
                for (int i=0; i<properties.mode; i++)
                {
                    if (properties.textureIcon[i].loadFromFile(properties.fileTextureIcon[i]))
                    {
                        properties.spriteIcon[i].setTexture(properties.textureIcon[i]);
                        properties.spriteIcon[i].setPosition(0,0);
                        if (properties.outlineThickness[i] < 0 or properties.outlineThickness[i] > 20) properties.outlineThickness[i] = 0;
                        sf::Vector2u foo = properties.textureIcon[iMode].getSize();
                        float scale = size.y*1./foo.y;
                        properties.spriteIcon[i].setScale(scale,scale);
                    }
                    else cout<<"Unable to open \""<<properties.fileTextureIcon[i]<<"\"\n";
                }
            }
            outline.setPosition(position);
            outline.setSize(size);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineColor(properties.outlineColor[0]);
            outline.setOutlineThickness(properties.outlineThickness[0]);
            text.setFillColor(properties.textColor[0]);
            updateButton();
        }
    }
}

Button::Button(const Button& user_object)
{
    *this = user_object;
}

sf::Sprite Button::getSpriteIcon() const { return properties.spriteIcon[iMode];}
Text Button::getText() const { return text;}
bool Button::getSignal() { if (signal==0) return 0; signal = 0; return 1;}

void Button::setPosition(sf::Vector2f user_position)
{
    position = user_position;
    sprite.setPosition(position);
    outline.setPosition(position);
}
void Button::setSize(sf::Vector2f user_size)
{
    if (user_size.x > 0 and user_size.y > 0)
    {
        size = user_size;
        updateButton();
    }
    else cout<<"Invalid size in \"Button::setSize\": must be greater than 0.\n";
}
void Button::setText(string user_text)
{
    text.setFont(ubuntuFont);
    text.setString(user_text);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    updateButton();
}
void Button::setIcon(bool user_icon)
{
    icon = user_icon;
}

void Button::updateButton()
{
    if (icon and (int)text.getString().getSize() > 0)
    {
        sf::FloatRect foo2 = properties.spriteIcon[iMode].getLocalBounds();
        text.setPosition(foo2.width+offsetText,size.y/2-2*text.getCharacterSize()/3);
        sf::FloatRect foo3 = text.getLocalBounds();
        size.x = max(size.x,foo2.width+2*offsetText+foo3.width);
    }
    else if ((int)text.getString().getSize() > 0)
    {
        text.setPosition(offsetText,size.y/2-2*text.getCharacterSize()/3);
        sf::FloatRect foo3 = text.getLocalBounds();
        size.x = max(size.x,2*offsetText+foo3.width);
    }
    texture.create(size.x,size.y);
    
    outline.setPosition(position);
    outline.setSize(size);
    outline.setOutlineColor(properties.outlineColor[iMode]);
    outline.setOutlineThickness(properties.outlineThickness[iMode]);
    texture.clear(properties.fillColor[iMode]);
    texture.setView(sf::View(sf::FloatRect(0,0,size.x,size.y)));
    text.setFillColor(properties.textColor[iMode]);
    texture.draw(text);
    properties.spriteIcon[iMode].setPosition(0,0);
    if (icon) texture.draw(properties.spriteIcon[iMode]);
    sf::Vector2u foo = texture.getSize();
    
    if (foo.x > 0 and foo.y > 0)
    {
        texture.display();
        sf::Sprite fooSprite;
        fooSprite.setTexture(texture.getTexture());
        sprite = fooSprite;
        sprite.setPosition(position);
        sprite.setScale(1,1);
    }
}

void Button::testMouse(sf::Vector2i user_posMouse)
{
    if (enabled)
    {
        posMouse = user_posMouse;
        if (posMouse.x > position.x and posMouse.x < position.x+size.x and posMouse.y > position.y and posMouse.y < position.y+size.y)
            isMouseHere = 1;
        else
            isMouseHere = 0;
        if (!isMouseHere) isMousePressed = 0;
        
        if (properties.mode == 2 and isMouseHere)
        {
            iMode = 1;
        }
        else
        {
            iMode = 0;
        }
    }
    else
    {
        isMouseHere = isMousePressed = signal = 0;
        iMode = 0;
    }
    updateButton();
}

void Button::draw(float elapsedTime)
{
    for (int i=0; i<nAttachedWidget; i++)
        attachedWidget[i]->draw(elapsedTime);
    if (target != 0)
    {
        target->draw(outline);
        target->draw(sprite);
    }
}

Button& Button::operator=(const Button& user_object)
{
    Widget::operator=(user_object);
    ratioPressed = user_object.ratioPressed;
    iMode = user_object.iMode;
    offsetText = user_object.offsetText;
    text = user_object.text;
    signal = user_object.signal;
    icon = user_object.icon;
    
    properties.mode = user_object.properties.mode;
    properties.fileTextureIcon = user_object.properties.fileTextureIcon;
    properties.spriteIcon = user_object.properties.spriteIcon;
    properties.textureIcon = vector<sf::Texture>(properties.mode);
    for (int i=0; i<properties.mode; i++)
    {
        if(properties.fileTextureIcon[i].length() > 0 and properties.textureIcon[i].loadFromFile(properties.fileTextureIcon[i]))
            properties.spriteIcon[i].setTexture(properties.textureIcon[i]);
    }
    properties.fillColor = user_object.properties.fillColor;
    properties.outlineColor = user_object.properties.outlineColor;
    properties.textColor = user_object.properties.textColor;
    properties.outlineThickness = user_object.properties.outlineThickness;
    
    outline = user_object.outline;
    updateButton();

    return *this;
}

ostream& operator<<(ostream& f, const Button& user_object)
{
    f<<"Position: "<<user_object.position.x<<"  "<<user_object.position.y<<endl;
    f<<"Size: "<<user_object.size.x<<"  "<<user_object.size.y<<endl;
    f<<"Icon: "<<user_object.icon<<endl;
    f<<"Properties: \n";
    f<<"   mode = "<<user_object.properties.mode<<endl;
    f<<"   iMode = "<<user_object.iMode<<endl;
    f<<"   files: "<<endl;
    for (int i=0; i<user_object.properties.mode; i++)
        f<<"      "<<user_object.properties.fileTextureIcon[i]<<endl;
    f<<"   fillColors: "<<endl;
    for (int i=0; i<user_object.properties.mode; i++)
        f<<"      "<<user_object.properties.fillColor[i].r<<" "<<user_object.properties.fillColor[i].g<<" "<<user_object.properties.fillColor[i].b<<" "<<user_object.properties.fillColor[i].a<<" "<<endl;
    f<<"   outlineColors: "<<endl;
    for (int i=0; i<user_object.properties.mode; i++)
        f<<"      "<<user_object.properties.outlineColor[i].r<<" "<<user_object.properties.outlineColor[i].g<<" "<<user_object.properties.outlineColor[i].b<<" "<<user_object.properties.outlineColor[i].a<<" "<<endl;
    f<<"   outlineThicknesses: "<<endl;
    for (int i=0; i<user_object.properties.mode; i++)
        f<<"      "<<user_object.properties.outlineThickness[i]<<endl;
    return f;
}


///////////////////////////////////////////
///////// PUSHBUTTON //////////////////////
///////////////////////////////////////////

PushButton::PushButton(): Button()
{
}

PushButton::PushButton(sf::RenderTarget* user_target): PushButton()
{
    target = user_target;
}

PushButton::PushButton(sf::RenderTarget* user_target, PropButton user_properties): Button(user_target, user_properties)
{
}

PushButton::PushButton(const PushButton& user_object): Button(user_object)
{
    *this = user_object;
}

void PushButton::updateButton()
{
    if (icon and (int)text.getString().getSize() > 0)
    {
        sf::FloatRect foo2 = properties.spriteIcon[iMode].getLocalBounds();
        text.setPosition(foo2.width+offsetText,size.y/2-2*text.getCharacterSize()/3);
        sf::FloatRect foo3 = text.getLocalBounds();
        size.x = foo2.width+2*offsetText+foo3.width;
    }
    else if ((int)text.getString().getSize() > 0)
    {
        text.setPosition(offsetText,size.y/2-2*text.getCharacterSize()/3);
        sf::FloatRect foo3 = text.getLocalBounds();
        size.x = 2*offsetText+foo3.width;
    }
    texture.create(size.x,size.y);
    if (!isMousePressed)
    {
        outline.setPosition(position);
        outline.setSize(size);
    }
    else
    {
        outline.setPosition(position+size*(1-ratioPressed)*(float)0.5);
        outline.setSize(size*ratioPressed);
    }
    outline.setOutlineColor(properties.outlineColor[iMode]);
    outline.setOutlineThickness(properties.outlineThickness[iMode]);
    texture.clear(properties.fillColor[iMode]);
    texture.setView(sf::View(sf::FloatRect(0,0,size.x,size.y)));
    text.setFillColor(properties.textColor[iMode]);
    texture.draw(text);
    properties.spriteIcon[iMode].setPosition(0,0);
    if (icon) texture.draw(properties.spriteIcon[iMode]);
    sf::Vector2u foo = texture.getSize();
    
    if (foo.x > 0 and foo.y > 0)
    {
        texture.display();
        sf::Sprite fooSprite;
        fooSprite.setTexture(texture.getTexture());
        sprite = fooSprite;
        sprite.setPosition(position);
        sprite.setScale(1,1);
        if (isMousePressed)
        {
            sprite.setPosition(position+size*(1-ratioPressed)*(float)0.5);
            sprite.setScale(ratioPressed,ratioPressed);
        }
    }
}

void PushButton::mousePressed()
{
    if (isMouseHere)
    {
        isMousePressed = 1;
        updateButton();
    }
}

void PushButton::mouseReleased()
{
    if (isMousePressed)
    {
        signal = 1;
        isMousePressed = 0;
        updateButton();
    }
}

PushButton& PushButton::operator=(const PushButton& user_object)
{
    Button::operator=(user_object);

    return *this;
}
//////////////////////////////////////////////
/////////// PRESSBUTTON //////////////////////
//////////////////////////////////////////////


PressButton::PressButton(): PushButton(){}
PressButton::PressButton(sf::RenderTarget* user_target): PushButton(user_target){}
PressButton::PressButton(sf::RenderTarget* user_target, PropButton user_properties): PushButton(user_target, user_properties){}
PressButton::PressButton(const PressButton& user_object)
{
    *this = user_object;
}

bool PressButton::getSignal() const { return signal;}

void PressButton::testMouse(sf::Vector2i user_posMouse)
{
    if (enabled)
    {
        posMouse = user_posMouse;
        if (posMouse.x > position.x and posMouse.x < position.x+size.x and posMouse.y > position.y and posMouse.y < position.y+size.y)
            isMouseHere = 1;
        else
            isMouseHere = 0;
        if (!isMouseHere) isMousePressed = 0;
        if (properties.mode == 2 and isMouseHere)
        {
            iMode = 1;
        }
        else if (!signal)
        {
            iMode = 0;
        }
    }
    else
    {
        isMouseHere = isMousePressed = 0;
        iMode = signal;
    }
    updateButton();
}

void PressButton::mouseReleased()
{
    if (isMousePressed)
    {
        signal = !signal;
        iMode = signal;
        isMousePressed = 0;
        updateButton();
    }
}

PressButton& PressButton::operator=(const PressButton& user_object)
{
    Button::operator=(user_object);

    return *this;
}
