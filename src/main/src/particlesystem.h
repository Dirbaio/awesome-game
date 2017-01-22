#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include "actor.h"


class Particle {
    public:
        struct Vertex {
                vec2f pos;
                vec2f vel;
                vec4f col;
                float size;
                int texIndex;
        };
        Particle();
        bool isAlive() {return (lifeLeft>0);}
        void update(float deltaTime);
        void render(Particle::Vertex& vtx) const;

        float life; //Tiempo de vida en segundos. No cambia
        float lifeLeft; //Tiempo que queda de vida. 1..0
        float startSize, endSize;
        vec4f startCol, endCol;
        vec2f p, v, a;
        int texIndex;
};

class ParticleSystem : public Actor{
public:
    ParticleSystem(Scene* s);
    void update();
    void draw();
    void addParticle(const Particle& p);
    int getParticleCount() const { return particles.size(); }

private:
    std::list<Particle> particles;
    Mesh mesh;
};

#endif // PARTICLESYSTEM_H
