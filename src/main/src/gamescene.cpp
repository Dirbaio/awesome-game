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
        if (players.find('z') == players.end()) {
            vec2f pos(center.x, center.y+10.f);
            Player* p = new Player(this, 'z', pos);
            addActor(p);
            players['z'] = p;
        }
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

extern Window* window;

void GameScene::draw() {
    vec3f tl(99999999.f,9999999.f,0.f), br(-999999.f,-999999.f,0.f);
    if (players.size() != 0) {
        for (auto p : players) {
            auto pos = p.second->getPosition();
            tl.x = min(tl.x, pos.x);
            tl.y = min(tl.y, pos.y);
            br.x = max(br.x, pos.x);
            br.y = max(br.y, pos.y);
        }
    }

    //Camera hacks
    float aspect = float(window->getSize().x)/window->getSize().y;
    float dist = max(abs(br.x-tl.x),abs(br.y-tl.y));
    dist /=2.2;
    dist = max(dist , 10.f);
    projection = glm::ortho(-dist*aspect, dist*aspect, -dist, dist);
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
