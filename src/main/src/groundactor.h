#ifndef GROUNDACTOR_H
#define GROUNDACTOR_H

#include "actor.h"
#include "gamescene.h"

class GroundChunk;
class GroundActor : public Actor {
public:
    vector<GroundChunk*> chunks;

    GroundActor(GameScene* scene);
    void load(int x);
    void draw();
};

#endif // GROUNDACTOR_H
