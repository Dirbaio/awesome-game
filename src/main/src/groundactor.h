#ifndef GROUNDACTOR_H
#define GROUNDACTOR_H

#include "actor.h"
#include "gamescene.h"
#include "perlinnoise.h"

class GroundChunk;
class GroundActor : public Actor {
public:
    vector<GroundChunk*> chunks;
    PerlinNoise pn;

    GroundActor(GameScene* scene);
    ~GroundActor();
    void load(int x);
    void draw();
};

#endif // GROUNDACTOR_H
