#ifndef GAMESTATE
#define GAMESTATE

enum GameState
{
    BEGIN_INIT = 0,
    BEGIN,
    BEGIN_PLAYING,
    PLAYING,
    END,
    END_TO_BEGIN
};

inline GameState operator++ (GameState& d) {
    d = static_cast<GameState>((static_cast<int>(d) + 1));
    return d;
}
#endif