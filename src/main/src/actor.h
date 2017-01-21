#ifndef ACTOR_H
#define ACTOR_H

#include<Box2D/Box2D.h>
#include "scene.h"
#include <VBE/VBE.hpp>

class Actor {
public:
    b2Body* body;
    bool removed;
    Scene* scene;

    Actor(Scene* scene);
    virtual ~Actor();

    virtual void update();
    virtual void draw();

    void remove();

    void setBody(b2Body* body);
    vec2f getPosition();
    float getAngle();
};

#endif // ACTOR_H
