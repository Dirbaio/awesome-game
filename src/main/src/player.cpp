#include "player.h"
#include "assets.h"


const float DOWN_FORCE = 2000.0f;
const float UP_FORCE = 0.5f;
const float SIDES_FORCE = 5.0f;

Player::Player(GameScene* scene, char letter, vec2f pos) : Actor(scene) {
    this->scene = scene;
    this->letter = letter;

    b2CircleShape circle;
    circle.m_radius = 1.f;

    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(pos.x, pos.y);
    b2Body* body = scene->world.CreateBody(&def);
    b2FixtureDef fixtureDef;
    fixtureDef.friction = 0.05;
    fixtureDef.shape = &circle;
    fixtureDef.density = 3;
    body->CreateFixture(&fixtureDef);

    this->setBody(body);
}

void Player::update() {
    WebSocketInput::PlayerState input = scene->getPlayerInput(letter);
    if(input == WebSocketInput::DOWN)
        body->ApplyForceToCenter(b2Vec2(0, -DOWN_FORCE), true);

    if(input == WebSocketInput::UP) {
        b2ContactEdge* e = body->GetContactList();
        int ct = 0;
        b2Vec2 lol(0, 0);
        while(e != nullptr) {
            if(e->contact->IsTouching()) {
                b2WorldManifold wm;
                e->contact->GetWorldManifold(&wm);
                b2Vec2 n = wm.normal;
                b2Vec2 d = body->GetPosition() - wm.points[0]; // lolol
                float dotito = b2Dot(n, d);
                if(dotito < 0) n = -1.0f * n;
                lol += n;
                ct++;
            }
            e = e->next;
        }

        if(ct != 0) {
            b2Vec2 n = (1.0f / ct) * lol;
            body->SetLinearVelocity(body->GetLinearVelocity() + 20.0f * n);
        }
    }

    float distance = getPositionVec3().x-scene->br.x;

    float communism = 60.f;
    communism += -fabs(distance*10.f);
    body->ApplyForceToCenter(b2Vec2(communism,0.f), true);

}

void Player::draw() {
    Texture2D* face = faces[faceIndex(letter)];
    drawQuad(*face, getPosition(), 1.3f, getAngle()/4);
}
