#include "Ball.h"

void Ball::updateOtherScore(int score)
{
    bar_->setOtherLife(score);
}

void Ball::update()
{
    if(hidden_)return;

    x_ = x_ + dx_;
    y_ = y_ + dy_;
    
    // Check colision with the bar
    if (!init_ && bar_->colidesWith(static_cast<Drawable *>(this)))
    {
        dx_ *= -1;
    }

    // Check latera colision in init_ mode. 
    if(init_ && (x_ <= 0 || x_ >= SCREEN_WIDTH - r_))
    {
        dx_ *= -1;
    }

    // Check top/bottom colision
    if (y_ <= 0 || y_ >= SCREEN_HEIGHT - r_)
    {
        dy_ *= -1;
    }
    
    // Init balls dont need the rest of the code...
    if(init_) return;

    // Check lateral colision aka point scored
    if (x_ <= 0)
    {   
        bar_->setLife(bar_->getLife() - 1);
        com_->lostPoint(bar_->getLife());
        if(bar_->getLife() == 0)
        {
            // Game over. Increment game state.
            hide();
            *state_ = END;
        }else{
            restart();
        }
    }

    if (x_ >= SCREEN_WIDTH)
    {
        com_->portal(x_, y_, dx_, dy_);
        hide();
    }
}

void Ball::show()
{
    hidden_ = false;    
}

void Ball::setVals(int values[4])
{
    x_ = values[0] - MAX_DX;
    y_ = values[1];

    dx_ = -constrain(values[2], MIN_DX, MAX_DX);
    dy_ =  constrain(values[3], MIN_DY, MAX_DY);
}

void Ball::hide()
{
    hidden_ = true;
    x_ = SCREEN_WIDTH / 2;
    y_ = SCREEN_HEIGHT / 2;
    dx_ = 0;
    dy_ = 0;
}

void Ball::restart()
{
    x_ = bar_->getX() + bar_->getW() + 5 + r_;
    y_ = random(r_, SCREEN_HEIGHT - r_);

    dx_ = (int)(random(MIN_DX, MAX_DX));
    dy_ = (int)(random(MIN_DY, MAX_DY));
}

void Ball::draw()
{
    if (!hidden_)
        display_->drawPixel(x_, y_, SSD1306_WHITE);
}