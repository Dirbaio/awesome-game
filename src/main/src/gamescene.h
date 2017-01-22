#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "websocketinput.h"
#include <VBE/VBE.hpp>
#include "scene.h"
#include "particlesystem.h"

class Player;
class GroundActor;
class GameScene : public Scene {
public:
    GroundActor* ground;
    map<char, Player*> players;
    WebSocketInput* input;
    ParticleSystem* particles;
    vec2f center;
    vec2f tl;
    vec2f br;
    char winner;
    int winScreenTimer;

    GameScene(WebSocketInput* input);
    void update();
    void draw();
    WebSocketInput::PlayerState getPlayerInput(char letter);
};
#endif // GAMESCENE_H
