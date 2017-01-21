#include "player.h"
#include "assets.h"


const float DOWN_FORCE = 1000.0f;
const float UP_FORCE = 1.0f;
const float SIDES_FORCE = 5.0f;

Player::Player(GameScene* scene, char letter, vec2f pos) : Actor(scene) {
    this->scene = scene;
    this->letter = letter;

    b2CircleShape circle;
    circle.m_radius = 2.5f;

    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(pos.x, pos.y);
    b2Body* body = scene->world.CreateBody(&def);
    b2FixtureDef fixtureDef;
    fixtureDef.friction = 0.05;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1;
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
                b2Vec2 n = e->contact->GetManifold()->localNormal;
                lol += n;
                ct++;
                break;
            }
            e = e->next;
        }

        if(ct != 0) {
            b2Vec2 n = (1.0f / ct) * lol;
            body->SetLinearVelocity(body->GetLinearVelocity() + 20.0f * n);
        }
    }
}

void Player::draw() {
    Texture2D* face = faces[letter%faces.size()];
    drawQuad(*face, getPosition(), 3.0f, getAngle()/2);
}
