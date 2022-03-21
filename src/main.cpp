#include <Arduino.h>
#include "main.h"

#include "Screen/screen.h"
#include "Ball/Ball.h"
#include "Bar/Bar.h"
#include "GameState.h"

Screen display = Screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Ball *ball;
Bar *lBar;
Comm *com;

std::vector<Ball *> startBalls;

GameState state = BEGIN_INIT;

uint32_t elapsedTimeEndSplash;
uint32_t elapsedTimeGameLoop;

void setup()
{
    pinMode(UP_BTN_PIN, INPUT);
    pinMode(DOWN_BTN_PIN, INPUT);

    Serial.begin(115200);

    display.init();

    pinMode(0, INPUT_PULLDOWN);

    com = new Comm(&state);
    lBar = new Bar(&display, 5, 2, 4, 22);
    ball = new Ball(&display, &state, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, lBar, com, 0, 0);
    com->setBall(ball);
}

void loop()
{
    com->checkNewMessages();
    com->checkTxNotReceived();

    switch (state)
    {
    case BEGIN_INIT:
    {
        // Start screen balls instantiation
        int numBalls = random(10, 28);
        for (int i = 0; i <= numBalls; i++)
        {
            int dx = random(-7, 7);
            int dy = random(-7, 7);
            startBalls.push_back(new Ball(&display, dx, dy));
        }

        ++state;
    }
    break;

    case BEGIN:
        // Pulling of board button to check game start.
        if (digitalRead(0) == 0)
        {
            com->beginGame();
            ball->restart();
            ball->show();
            ++state;
            break;
        }

        // Display starting screen.
        display.clearDisplay();
        display.centerText("Press to play", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1);
        for (Ball *b : startBalls)
        {
            b->update();
            b->draw();
        }
        display.display();

        break;
    case BEGIN_PLAYING:
        // Setup vars before the game
        lBar->setLife(MAX_HP);
        lBar->setOtherLife(MAX_HP);

        // Free start screen balls.
        for (Ball *b : startBalls)
        {
            free(b);
            b = NULL;
        }
        startBalls.clear();

        ++state;
        break;
    case PLAYING:
        // Game loop.
        if (millis() - elapsedTimeGameLoop >= GAME_LOOP_MS)
        {
            elapsedTimeGameLoop = millis();
            display.clearDisplay();

            lBar->update();
            ball->update();

            lBar->draw();
            ball->draw();

            display.display();
        }
        break;
    case END:
        display.clearDisplay();
        display.centerText((lBar->getLife() == 0 ? "Lost" : "Won"), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2);
        display.display();
        elapsedTimeEndSplash = millis();

        // Show win/loose msg and restart the game.
        ++state;
        break;
    case END_TO_BEGIN:

        // Wait some time to show end screen.
        if (millis() - elapsedTimeEndSplash >= END_SCREEN_TIME)
        {
            state = BEGIN_INIT;
        }
        break;
    default:
        state = BEGIN_INIT;
        break;
    }
}