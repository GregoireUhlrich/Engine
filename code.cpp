#ifndef WRAPMENU_H_INCLUDED
#define WRAPMENU_H_INCLUDED
#include <iostream>
#include <vector>
#include "button.h"

class WrapMenu{

    private:
    
    int a;
   
    public:
    
    WrapMenu();
    WrapMenu(const WrapMenu& user_object);
    ~WrapMenu(){};
    
    int getA() const;
    
    void setA(int user_a);
        
    WrapMenu& operator=(const WrapMenu& user_object);
    friend std::ostream& operator<<(std::ostream& f, const WrapMenu& user_object);
};
        
#endif

        
#include "WrapMenu.h" 

WrapMenu::WrapMenu()
{

}

WrapMenu::WrapMenu(const WrapMenu& user_object)
{
    a = user_object.a;
}
    
int WrapMenu::getA() const { return a;}

void WrapMenu::setA(int user_a)
{
    a = user_a;
}

WrapMenu& WrapMenu::operator=(const WrapMenu& user_object)
{
    *this = WrapMenu(user_object);

    return *this;
}

ostream& operator<<(ostream& f, const WrapMenu& user_object)
{
    f<<"{"<<endl<<"a = "<<user_object.a<<endl<<"}"<<endl;
    return f;
}

