#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "websocketinput.h"
#include <VBE/VBE.hpp>
#include "scene.h"

class Player;
class GroundActor;
class GameScene : public Scene {
public:
    GroundActor* ground;
    map<char, Player*> players;
    WebSocketInput* input;
    vec3f center;
    vec3f tl;
    vec3f br;

    GameScene(WebSocketInput* input);
    void update();
    void draw();
    WebSocketInput::PlayerState getPlayerInput(char letter);
};
#endif // GAMESCENE_H
