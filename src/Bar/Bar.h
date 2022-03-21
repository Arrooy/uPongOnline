#ifndef BAR_ADRIA
#define BAR_ADRIA

#include "Drawable.hpp"

#define MAX_HP 3
#define SPEED 3

#define BUTTON_REBOTE_TIMEOUT 50

class Bar : public Drawable{
    
    int w_,h_;
    int initx_,inity_;
    
    int life_;
    int otherLife_;

    uint32_t upElapsedTime;
    uint32_t downElapsedTime;

public:
    Bar(Screen *display, int x, int y, int w, int h) 
    : Drawable(display, x, y), w_(w), h_(h)
    {
        initx_ = x;
        inity_ = y;
        life_ = MAX_HP;
        otherLife_ = MAX_HP;
    }
   
    void update() override;
    void draw() override;

    bool colidesWith(Drawable *d);
    int getW(){return w_;}
    int getH(){return h_;}
    
    int getLife(){return life_;}
    void setLife(int life){life_=life;}

    int getOtherLife(){return otherLife_;}
    void setOtherLife(int life){otherLife_=life;}
};

#endif