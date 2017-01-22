#include "particlesystem.h"
#include "assets.h"

Particle::Particle()
    : life(1), lifeLeft(1),
      startSize(1), endSize(1),
      startCol(1.0f), endCol(1.0f),
      p(0.0f), v(0.0f), a(0.0f),
      texIndex(0) {
}

void Particle::update(float deltaTime) {
    p += v*deltaTime + a*deltaTime*deltaTime*0.5f;
    v += a*deltaTime;
    lifeLeft -= deltaTime*(1/life);
}

void Particle::render(Particle::Vertex& vtx) const {
    float size = (startSize*lifeLeft + endSize*(1-lifeLeft));
    vec4f col = (startCol*lifeLeft + endCol*(1-lifeLeft));
    vtx.pos = p;
    vtx.vel = v;
    vtx.col = col;
    vtx.size = size;
    vtx.texIndex = texIndex;
}

ParticleSystem::ParticleSystem(Scene* s)
    : Actor(s)
{

    Vertex::Format format({
                              Vertex::Attribute("a_position", Vertex::Attribute::Float, 2),
                              Vertex::Attribute("a_vel", Vertex::Attribute::Float, 2),
                              Vertex::Attribute("a_color", Vertex::Attribute::Float, 4),
                              Vertex::Attribute("a_size", Vertex::Attribute::Float, 1),
                              Vertex::Attribute("a_texIndex", Vertex::Attribute::Int, 1)
                          });

    mesh = Mesh(format, Mesh::STREAM);
    mesh.setPrimitiveType(Mesh::POINTS);
}

void ParticleSystem::update() {
    for(std::list<Particle>::iterator it = particles.begin(); it != particles.end();) {
        it->update(1.f/60.f);
        if(!it->isAlive())
            it = particles.erase(it);
        else
            ++it;
    }
}

const int PARTICLE_TEX_COUNT = 4;

void ParticleSystem::draw() {
    if(particles.size() == 0)
        return;
    std::vector<Particle::Vertex> vtxs(particles.size());
    int i = 0;
    for(std::list<Particle>::const_iterator it = particles.begin(); it != particles.end(); it++) {
        it->render(vtxs[i]);
        i++;
    }
    mesh.setVertexData(&vtxs[0], particles.size());
    particleShader.uniform("mvp")->set(projection);
    particleShader.uniform("texSize")->set(1.0f/float(PARTICLE_TEX_COUNT));
    particleShader.uniform("textureSheet")->set(particlesTexture);
    mesh.draw(particleShader);
}

void ParticleSystem::addParticle(const Particle& p) {
    particles.push_back(p);
}
