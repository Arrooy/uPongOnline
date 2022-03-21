#ifndef DRAWABLE_ADRIA
#define DRAWABLE_ADRIA

#include "Screen/screen.h"

class Drawable {

protected:
  int x_, y_;

public:
  Screen* display_;

  Drawable(Screen *display, int x, int y): x_(x), y_(y), display_(display){};
  ~Drawable(){};

  virtual void update();
  virtual void draw();
  int getY(){ return y_; }
  int getX(){ return x_; }
};
#endif