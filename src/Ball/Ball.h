#ifndef BALL_ADRIA
#define BALL_ADRIA

#include "Drawable.hpp"
#include "Bar/Bar.h"
#include "Comm/Comm.h"
#include "GameState.h"

// Max x axis speed range.
#define MAX_DX 3
#define MIN_DX 1

// Max y axis speed range.
#define MAX_DY 3
#define MIN_DY 1

// Fordward declaration
class Comm;

class Ball : public Drawable
{
  int dx_, dy_;
  int r_;

  bool hidden_;

  Bar *bar_;
  Comm *com_;
  GameState *state_;

  bool init_;
public:
  Ball(Screen *display, GameState *state, int x, int y, Bar *bar, Comm *com, int dx, int dy)
      : Drawable(display, x, y), dx_(dx), dy_(dy), bar_(bar), com_(com), state_(state)
  {
    hidden_ = true;
    r_ = 1;
    init_ = false;
  };

  Ball(Screen *display, int dx, int dy)
      : Drawable(display, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), dx_(dx), dy_(dy)
  {
    hidden_ = false;
    r_ = 1;
    init_ = true;
  }

  void update() override;
  void draw() override;
  void restart();
  void hide();
  void show();
  void setVals(int values[4]);

  void updateOtherScore(int score);
};

#endif