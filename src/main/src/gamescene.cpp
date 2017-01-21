#include "gamescene.h"
#include "groundactor.h"
#include "player.h"
#include "assets.h"

GameScene::GameScene(WebSocketInput* i) : input(i) {
    ground = new GroundActor(this);
    addActor(ground);

    int x = 0;
    for (char c :  input->getPlayers()){
        cout << "Recovering player " << c << endl;
        vec2f pos(x++, 10.f);
        Player* p = new Player(this, c, pos);
        addActor(p);
        players[c] = p;
    }
}

void GameScene::update() {
    center = vec3f (0,0,0);
    if (players.size() != 0) {
        for (auto p : players) {
            auto pos = p.second->getPosition();
            center.x += pos.x;
            center.y += pos.y;
        }
        center /= players.size();
    }
    ground->load(center.x);

    if(Keyboard::justPressed(Keyboard::Return)){
        vec2f pos(center.x, center.y+10.f);
        Player* p = new Player(this, 'z', pos);
        addActor(p);
        players['z'] = p;
    }

    for (char c : input->connectedPlayers()){
        vec2f pos(center.x, center.y+10.f);
        Player* p = new Player(this, c, pos);
        addActor(p);
        players[c] = p;
    }

    for (char c : input->disconnectedPlayers()) {
        players[c]->remove();
        players.erase(c);
    }

    if(Keyboard::justPressed(Keyboard::R)){
        nextScene = new GameScene(input);
    }

    Scene::update();
}

void GameScene::draw() {
    projection = glm::translate(projection, -center);
    Scene::draw();
}

WebSocketInput::PlayerState GameScene::getPlayerInput(char letter) {
    if(letter == 'z') {
        if(Keyboard::pressed(Keyboard::Down))
            return WebSocketInput::DOWN;
        if(Keyboard::pressed(Keyboard::UP))
            return WebSocketInput::UP;
        return WebSocketInput::NOTHING;
    }
    return input->getPlayerState(letter);
}
