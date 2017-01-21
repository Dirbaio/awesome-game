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
        if(e != nullptr) {
            body->SetLinearVelocity(body->GetLinearVelocity() + b2Vec2(0, 10));
        }
    }
}

void Player::draw() {
    Texture2D* face = faces[letter%faces.size()];
    drawQuad(*face, getPosition(), 3.0f, getAngle()/2);
}
