#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"
#include "gamescene.h"

class Player : public Actor {
public:
    GameScene* scene;
    char letter;
    int score;
    WebSocketInput::PlayerState input;

    Player(GameScene* scene, char letter, vec2f pos);
    void update();
    void draw();

    int jumping;
    int notinground;
    int inground;
    b2Vec2 normal;
    vec2f oldPos;
};


#endif // PLAYER_H
