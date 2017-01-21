#include "scene.h"
#include<VBE/VBE.hpp>
#include "actor.h"

const float GRAVITY = 100.0f;


Scene::Scene() : world(b2Vec2(0.0f, -GRAVITY)) {
    world.SetContactListener(this);
    nextScene = nullptr;
}

void Scene::switchScene(Scene* newScene) {
    VBE_ASSERT(nextScene == nullptr, "Do not switch scene two times");
    nextScene = newScene;
}

void Scene::update() {
    float32 timeStep = 1.0f / 60.f;
    int32 velocityIterations = 10;
    int32 positionIterations = 8;
    world.Step(timeStep, velocityIterations, positionIterations);

    for(Actor* actor : actors)
        actor->update();

    for(auto it = actors.begin(); it != actors.end();) {
        auto it2 = it;
        it2++;
        if((*it)->removed) {
            actors.erase(it);
            delete *it;
        }
        it = it2;
    }
}

void Scene::draw() {
    for(Actor* actor : actors)
        actor->draw();
}

void Scene::addActor(Actor* actor) {
    actors.push_back(actor);
}

void Scene::BeginContact(b2Contact* contact) {
}

void Scene::EndContact(b2Contact* contact){
}

void Scene::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
}

void Scene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
}
