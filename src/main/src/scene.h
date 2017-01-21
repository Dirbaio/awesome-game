#ifndef SCENE_H
#define SCENE_H

#include<list>
#include<Box2D/Box2D.h>

using namespace std;

class Actor;
class Scene : public b2ContactListener {
public:
    Scene* nextScene;
    list<Actor*> actors;
    b2World world;

    Scene();
    void switchScene(Scene* newScene);
    virtual void update();
    virtual void draw();
    void addActor(Actor* actor);
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};


#endif // SCENE_H
