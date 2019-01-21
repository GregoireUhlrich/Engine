#ifndef INTERACTIVE_H_INCLUDED
#define INTERACTIVE_H_INCLUDED
#include "widget.h"
#include "button.h"
#include "wrapMenu.h"
#include "map.h"
#include "global.h"
#include <string>
#include <dirent.h>

class Set_PushButton: public Widget{

    private:

    int activatedButton;

    public:

    Set_PushButton(): Widget(){ activatedButton = 0;}
    ~Set_PushButton(){}

    void testEvent(sf::Event event);
    void addChoice(Widget* newWidget);
    int getChoiceSet() const { return activatedButton;}

    void setChoiceSet(int choice);
};

class TilesetDisplayer: public Widget{

    private:

    int offset;
    int heightChoice;
    int height;
    bool isImLeft;

    int yScroll;
    int yImage;

    std::string fileLeft;
    sf::RenderTexture textureLeft;
    sf::Texture imLeft;
    sf::Sprite spriteLeft;
    sf::RectangleShape outlineLeft;

    sf::Vector2u sizeTarget;
    sf::Vector2i sizeSprite;
    sf::Vector2f posImLeft;

    bool select;
    bool select2;
    sf::RectangleShape selectRect;

    public:

    TilesetDisplayer(sf::RenderTarget* w, int user_height);
    ~TilesetDisplayer(){};

    sf::Vector2i convertPosMouse(sf::Vector2i p);
    sf::Vector2i invConvertPosMouse(sf::Vector2i p);

    std::string getFileLeft() const;
    sf::Vector2i getSizeSprite() const;

    void setSizeSprite(sf::Vector2i sizeSprite);
    void setYScroll(int user_yScroll);
    void addTextureFromMap(std::string file);

    void windowResized();
    void updateButton();
    void draw(float elapsedTime);
};

struct IndicesWidget{

    int file;
    int edit;
    int mode;
    int grid;
    int layer;
    int pass;
    int prio;
    int game;
};
        
class Interactive: public Widget{

    private:

    bool wrapHasFocus;
    int nDrawable;
    std::vector<sf::Drawable*> drawable;
    IndicesWidget indices;

    std::string choice;

    IMap* map;
    StateMap state;

    public:

    Interactive();
    Interactive(sf::RenderTarget* w, IMap* user_map);
    ~Interactive(){};

    void testMouse(sf::Vector2i user_posMouse);
    void testEvent(sf::Event event);
    void windowResized();

    std::string getChoice();

    void draw(double elapsedTime);
};

#endif
