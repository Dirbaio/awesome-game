#include "actor.h"

Actor::Actor(Scene* scene) {
    this->scene = scene;
    body = nullptr;
    removed = false;
}


Actor::~Actor() {
    if(body) {
        scene->world.DestroyBody(body);
        body = NULL;
    }
}

void Actor::update() {
}

void Actor::draw() {
}


void Actor::remove() {
    removed = true;
}


void Actor::setBody(b2Body* body) {
    VBE_ASSERT(this->body == nullptr, "Do not set the body twice, wtf");
    this->body = body;
    body->SetUserData(this);
}

vec2f Actor::getPosition() {
    return vec2f(body->GetPosition().x, body->GetPosition().y);
}

float Actor::getAngle() {
    return body->GetAngle();
}
