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
    vec2f center;
    vec2f tl;
    vec2f br;

    GameScene(WebSocketInput* input);
    void update();
    void draw();
    WebSocketInput::PlayerState getPlayerInput(char letter);
};
#endif // GAMESCENE_H
