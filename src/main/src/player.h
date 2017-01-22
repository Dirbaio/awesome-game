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

    int notinground;
    int inground;
    int jumping;
    b2Vec2 normal;
};


#endif // PLAYER_H
