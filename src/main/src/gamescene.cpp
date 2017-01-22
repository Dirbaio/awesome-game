#include "gamescene.h"
#include "groundactor.h"
#include "player.h"
#include "assets.h"

const float INITIAL_HEIGHT = 25.f;
const int TARGET_SCORE = 60*20;
const int WIN_SCREEN_DURATION = 15*60;

extern Window* window;

GameScene::GameScene(WebSocketInput* i)
    : input(i)
    , winner(0)
    , winScreenTimer(0)
{
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
    tl=vec2f(999999999.f,999999999.f);
    br=vec2f(-999999999.f,-999999999.f);
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

    //Determine winner
    Player* first_player = nullptr;
    if (players.size() >= 2 && !winner) {
        for (auto it : players) {
            Player* p = it.second;
            if (first_player == nullptr || p->getPosition().x > first_player->getPosition().x) {
                first_player = p;
            }
        }
        first_player->score++;

        if (first_player->score >= TARGET_SCORE) {
            winner = first_player->letter;
            winScreenTimer = 0;
        }
    }

    if (winner) {
        winScreenTimer++;
        cout << winScreenTimer << endl;
        if (winScreenTimer > WIN_SCREEN_DURATION) {
            nextScene = new GameScene(input);
        }
    }

    Scene::update();
}

float lolint(float f) {
    int x = int(f);
    if(f < 0 && f != x) x--;
    return x;
}

void GameScene::draw() {
    float aspect = float(window->getSize().x)/window->getSize().y;

    //Background
    float aspect2 = aspect * bgTexture->getSize().y / bgTexture->getSize().x;
    mat3f backgroundProjection = glm::scale(mat3f(1.f), vec2f(1/aspect2, 1.f));
    float lolscroll = center.x * 0.001f;
    quadShader.uniform("u_tex")->set(*bgTexture);
    quadShader.uniform("mvp")->set(glm::translate(backgroundProjection, vec2f(lolint(lolscroll/2)*2-lolscroll, 0)));
    quadMesh.draw(quadShader);
    quadShader.uniform("mvp")->set(glm::translate(backgroundProjection, vec2f(lolint(lolscroll/2)*2 + 2 -lolscroll, 0)));
    quadMesh.draw(quadShader);

    //Camera hacks
    float dist = max(abs(br.x-tl.x),abs(br.y-tl.y));
    dist /=2.2;
    dist = max(dist , 14.f);
    projection = glm::scale(mat3f(1.f), vec2f(1/(aspect*dist), 1/dist));
    projection = glm::translate(projection, -center);

    Scene::draw();

    if (winner) {
        projection = glm::scale(mat3f(1.f), vec2f(1/(aspect*14.f), 1/14.f));
        Texture2D* face = faces[faceIndex(winner)];
        drawQuad(*face, vec2f(0.f,-4.5f), fmin(winScreenTimer/2.f, 8.f), -(float)winScreenTimer/50.f);
        drawQuad(*winnerTexture, vec2f(0.f,6.f), fmin(winScreenTimer/20.f, 1.5f), sin(winScreenTimer/15.f)/6.f);
    }

}

WebSocketInput::PlayerState GameScene::getPlayerInput(char letter) {
    /*if (winner) {
        return WebSocketInput::NOTHING; //Ignore inputs
    }*/
    if(letter == 'z') {
        if(Keyboard::pressed(Keyboard::Down))
            return WebSocketInput::DOWN;
        if(Keyboard::pressed(Keyboard::UP))
            return WebSocketInput::UP;
    }
    return input->getPlayerState(letter);
}
