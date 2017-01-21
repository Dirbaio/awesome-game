#include "player.h"
#include "assets.h"


const float DOWN_FORCE = 1000.0f;
const float UP_FORCE = 1.0f;
const float SIDES_FORCE = 5.0f;

Player::Player(GameScene* scene, char letter, vec2f pos) : Actor(scene) {
    this->scene = scene;
    this->letter = letter;

    b2CircleShape circle;
    circle.m_radius = 1.0f;

    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(pos.x, pos.y);
    b2Body* body = scene->world.CreateBody(&def);
    b2FixtureDef fixtureDef;
    fixtureDef.friction = 0.1;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1;
    body->CreateFixture(&fixtureDef);

    this->setBody(body);
}

void Player::update() {
    WebSocketInput::PlayerState input = scene->getPlayerInput(letter);
    if(input == WebSocketInput::DOWN)
        body->ApplyForceToCenter(b2Vec2(0, -DOWN_FORCE), true);
    if(input == WebSocketInput::UP)
        body->ApplyForceToCenter(b2Vec2(0, UP_FORCE), true);
}

void Player::draw() {
    drawQuad(*awesome, getPosition(), 1.0f, getAngle());
}
