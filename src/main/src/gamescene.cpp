#include "gamescene.h"
#include "groundactor.h"
#include "player.h"
#include "assets.h"

const float INITIAL_HEIGHT = 25.f;

GameScene::GameScene(WebSocketInput* i) : input(i) {
    ground = new GroundActor(this);
    addActor(ground);

    int x = 0;
    for (char c :  input->getPlayers()){
        cout << "Recovering player " << c << endl;
        vec2f pos(x++, INITIAL_HEIGHT);
        Player* p = new Player(this, c, pos);
        addActor(p);
        players[c] = p;
    }
}

void GameScene::update() {
    tl=vec3f(999999999.f,999999999.f,0.f);
    br=vec3f(-999999999.f,-999999999.f,0.f);
    for (auto p : players) {
        auto pos = p.second->getPosition();
        tl.x = min(tl.x, pos.x);
        tl.y = min(tl.y, pos.y);
        br.x = max(br.x, pos.x);
        br.y = max(br.y, pos.y);
    }
    center = tl+((br-tl)/2.f);

    ground->load(center.x);

    if(Keyboard::justPressed(Keyboard::Return)){
        if (players.find('z') == players.end()) {
            vec2f pos(center.x, center.y+INITIAL_HEIGHT);
            Player* p = new Player(this, 'z', pos);
            addActor(p);
            players['z'] = p;
        }
    }

    for (char c : input->connectedPlayers()){
        vec2f pos(center.x, center.y+INITIAL_HEIGHT);
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

float lolint(float f) {
    int x = int(f);
    if(f < 0 && f != x) x--;
    return x;
}

void GameScene::draw() {
    //Camera hacks
    float aspect = float(window->getSize().x)/window->getSize().y;
    float aspect2 = aspect * bgTexture->getSize().y / bgTexture->getSize().x;
    projection = glm::ortho(-aspect2, aspect2, -1.f, 1.f);

    float lolscroll = center.x * 0.007f;
    drawQuad(*bgTexture, vec2f(lolint(lolscroll/2)*2-lolscroll, 0), 1, 0);
    drawQuad(*bgTexture, vec2f(lolint(lolscroll/2)*2 + 2 -lolscroll, 0), 1, 0);

    //More camera hacks
    float dist = max(abs(br.x-tl.x),abs(br.y-tl.y));
    dist /=2.2;
    dist = max(dist , 14.f);
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
