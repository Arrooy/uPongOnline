#include "Bar/Bar.h"

bool Bar::colidesWith(Drawable *d)
{
    // No colision for sure. Not in y range.
    if(d->getY() <= y_ || d->getY() >= y_ + h_) return false;


    if(d->getX() <= x_ + w_)
    {
        // Bounce
        return true;
    }

    return false;
}

void Bar::update()
{
    if(digitalRead(UP_BTN_PIN) == 1 && millis() - upElapsedTime >= BUTTON_REBOTE_TIMEOUT)
    {
        y_ -= SPEED;
        upElapsedTime = millis();
    }

    if(digitalRead(DOWN_BTN_PIN) == 1 && millis() - downElapsedTime >= BUTTON_REBOTE_TIMEOUT)
    {
        y_ += SPEED;
        downElapsedTime = millis();
    }

    y_ = constrain(y_, 0,(SCREEN_HEIGHT - h_));
}

void Bar::draw()
{
    display_->drawRect(x_, y_, w_, h_, SSD1306_WHITE);
    
    // Display hp:
    display_->centerCursor("3:3",SCREEN_WIDTH / 2, 0, 1);
    display_->printf("%d:%d", life_, otherLife_);
}