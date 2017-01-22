#include "gamescene.h"
#include "groundactor.h"
#include "player.h"
#include "assets.h"
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine

const float INITIAL_HEIGHT = 25.f;
const int TARGET_SCORE = 60*30;
const int WIN_SCREEN_DURATION = 10*60;

extern Window* window;

GameScene::GameScene(WebSocketInput* i)
    : input(i)
    , winner(0)
    , winScreenTimer(0)
{
    ground = new GroundActor(this);
    addActor(ground);
    particles = new ParticleSystem(this);
    addActor(particles);

    vector<char> existingPlayers = input->getPlayers();
    shuffle(existingPlayers.begin(), existingPlayers.end(), default_random_engine(time(NULL)));
    int x = 0;
    for (char c : existingPlayers){
        cout << "Recovering player " << c << endl;
        vec2f pos(x, INITIAL_HEIGHT);
        Player* p = new Player(this, c, pos);
        addActor(p);
        players[c] = p;
        x+=3;
    }
}

void GameScene::update() {
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

    if (players.empty()) {
        center = vec2f(0.f,0.f);
    } else {
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

    }


    winScreenTimer++;
    if (winner) {
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
    vec2f camera = center;
    if (players.empty()) camera.y = 1000;

    float dist = max(abs(br.x-tl.x),abs(br.y-tl.y));
    dist /=2.3;
    dist = max(dist , 16.f);
    projection = glm::scale(mat3f(1.f), vec2f(1/(aspect*dist), 1/dist));
    projection = glm::translate(projection, -camera);

    Scene::draw();

    //GUI
    if (players.empty()) {
        projection = glm::scale(mat3f(1.f), vec2f(1/(aspect*14.f), 1/14.f));
        float rot = sin(winScreenTimer/30.f)/6.f;
        float zoom = 4.f*(1.f+sin(winScreenTimer/15.f)/10.f);
        drawQuad(*titleTexture, vec2f(0.f,2.f), zoom, rot);
        float aweScale = 1.f - 0.2*fabs(cos(winScreenTimer/15.f));
        //projection = glm::scale(projection, vec2f(1.f,aweScale));
        drawQuad(*awesomeTexture, vec2f(28.f - (winScreenTimer/18.f),-12.5f + fabs(sin(winScreenTimer/15.f)*2.f)), vec2f(3.f,3.f*aweScale), 0.f);
    } else if (winner) {
        projection = glm::scale(mat3f(1.f), vec2f(1/(aspect*14.f), 1/14.f));
        Texture2D* face = faces[faceIndex(winner)];
        drawQuad(*face, vec2f(0.f,-4.5f), fmin(winScreenTimer/2.f, 8.f), -(float)winScreenTimer/50.f);
        drawQuad(*winnerTexture, vec2f(0.f,6.f), fmin(winScreenTimer/20.f, 1.5f), sin(winScreenTimer/15.f)/6.f);
    } else {
        projection = glm::translate(mat3f(1.f), vec2f(-1,1));
        projection = glm::scale(projection, vec2f(1/(aspect), 1));
        float y = 0;
        for (auto it : players) {
            Player *p = it.second;
            float progress = p->score/(float)TARGET_SCORE;
            Texture2D* face = faces[faceIndex(p->letter)];

            drawQuad(*face, vec2f(0.17f,-0.17f - y), 0.1f, -(float)winScreenTimer/40.f);

            vec2f begin(0.32f, -0.13f - y);
            vec2f endDone(0.32f + (0.5f*progress), -0.21f - y);
            vec2f endTotal(0.32f + (0.5f), -0.21f - y);
            vec4f colorDone(0.1f,0.8f,0.1f,0.8f);
            vec4f colorTotal(0.9f,0.9f,0.9f,0.5f);
            drawRect(colorTotal, begin, endTotal);
            drawRect(colorDone, begin, endDone);

            y += 0.19;
        }

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
